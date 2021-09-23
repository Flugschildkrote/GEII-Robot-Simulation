#include "gwSkybox.h"
#include "gwResourceManager.h"
#include "gwSkyboxMesh.h"
#include "gwTexture.h"
#include "gwShader.h"


using namespace gwe;

gwe::gwSkybox::gwSkybox(const std::string &name,
                        const std::string &shader,
                        const std::string &up,
                        const std::string &down,
                        const std::string &left,
                        const std::string &right,
                        const std::string &forward,
                        const std::string &back,
                        gwSkyboxMesh* mesh) : gwResource(name), mMesh(mesh), mSourceFile("")
{
    mShader = dynamic_cast<gwShader*>(singleton::gwResourceManager::getInstance()->getResource(shader));
    mTexturesID[4] = dynamic_cast<gwTexture*>(singleton::gwResourceManager::getInstance()->getResource(up))->getID();
    mTexturesID[5] = dynamic_cast<gwTexture*>(singleton::gwResourceManager::getInstance()->getResource(down))->getID();
    mTexturesID[3] = dynamic_cast<gwTexture*>(singleton::gwResourceManager::getInstance()->getResource(left))->getID();
    mTexturesID[1] = dynamic_cast<gwTexture*>(singleton::gwResourceManager::getInstance()->getResource(right))->getID();
    mTexturesID[2] = dynamic_cast<gwTexture*>(singleton::gwResourceManager::getInstance()->getResource(forward))->getID();
    mTexturesID[0] = dynamic_cast<gwTexture*>(singleton::gwResourceManager::getInstance()->getResource(back))->getID();

    for(unsigned int i(0); i < 6; i++)
    {
        glBindTexture(GL_TEXTURE_2D, mTexturesID[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    load();
}

gwe::gwSkybox::gwSkybox(const std::string &name, gwShader* shader,  const std::string &sourceFile) : gwResource(name),mShader(shader), mMesh(nullptr), mSourceFile(sourceFile)
{
    for(unsigned int i(0); i < 6; i++)
        mTexturesID[0] = 0;
}

gwe::gwSkybox::~gwSkybox()
{
    free();
}

void gwe::gwSkybox::render(const glm::mat4 &MVP)
{
    if(mShader == nullptr || (!isLoaded()))
        return;

    glDisable(GL_DEPTH_TEST);

        glUseProgram(mShader->getProgramID());
            glUniformMatrix4fv(glGetUniformLocation(mShader->getProgramID(), "MVP"), 1 , GL_FALSE, glm::value_ptr(MVP));

            glUniform1i(glGetUniformLocation(mShader->getProgramID(), "picture[0]"), 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mTexturesID[0]);

            glUniform1i(glGetUniformLocation(mShader->getProgramID(), "picture[1]"), 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mTexturesID[1]);

            glUniform1i(glGetUniformLocation(mShader->getProgramID(), "picture[2]"), 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mTexturesID[2]);

            glUniform1i(glGetUniformLocation(mShader->getProgramID(), "picture[3]"), 3);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, mTexturesID[3]);

            glUniform1i(glGetUniformLocation(mShader->getProgramID(), "picture[4]"), 4);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, mTexturesID[4]);

            glUniform1i(glGetUniformLocation(mShader->getProgramID(), "picture[5]"), 5);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, mTexturesID[5]);

                glBindVertexArray(mMesh->getVao());
                    glDrawArrays(mMesh->getDrawMode(), 0, mMesh->getVerticesCount());
                glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    glEnable(GL_DEPTH_TEST);
}

bool gwe::gwSkybox::load(void)
{
    if(isLoaded())
        return false;

    if(mSourceFile != "")
    {

        tinyxml2::XMLDocument doc;
        int Valid = 0;

        if(doc.LoadFile(mSourceFile.c_str()) != tinyxml2::XML_SUCCESS)
            Valid = -1;
        else
        {
            tinyxml2::XMLElement* first = doc.RootElement();
            if(validElement(first, "Skybox"))
            {
                tinyxml2::XMLElement* textureRoot = first->FirstChildElement();
                std::map<std::string, int> localMap;
                localMap["Back"] = 0;
                localMap["Right"] = 1;
                localMap["Front"] = 2;
                localMap["Left"] = 3;
                localMap["Up"] = 4;
                localMap["Down"] = 5;
                std::string dir = "";
                std::string buffer = "";
                for(unsigned int i(0); i < mSourceFile.size(); i++)
                {
                    buffer += mSourceFile[i];
                    if(mSourceFile[i] == '/')
                        dir = buffer;
                }
                if(validElement(textureRoot, "Textures"))
                {
                    tinyxml2::XMLElement* textureElement = textureRoot->FirstChildElement();
                    for(short i(0); i < 6; i++)
                    {
                        if(textureElement == 0)
                        {
                            Valid = -2;
                            break;
                        }
                        if(localMap.find(textureElement->Value()) == localMap.end())
                        {
                            Valid = -2;
                            break;
                        }
                        gwTexture* texture = new gwTexture(getName()+"_"+textureElement->Value(), dir+textureElement->GetText());
                        if(!texture->load())
                        {
                            Valid = -3;
                            break;
                        }

                        mTexturesID[localMap[textureElement->Value()]] = texture->getID();
                        textureElement = textureElement->NextSiblingElement();
                    }
                    tinyxml2::XMLElement*boxSize = textureRoot->NextSiblingElement();
                    if(validElement(boxSize, "BoxSize"))
                    {
                        std::stringstream ss(boxSize->GetText());
                        float f;
                        if(!(ss >> f))
                            Valid = -2;
                        else
                        {
                            mMesh = new gwSkyboxMesh(getName()+ "_Mesh", f);
                            if(!mMesh->load())
                                Valid = -3;
                        }
                    }
                    else
                        Valid = -2;
                }
                else
                    Valid = -2;
            }
            else
                Valid = -2;
        }

        switch(Valid)
        {
        case 0:
            for(unsigned int i(0); i < 6; i++)
            {
                glBindTexture(GL_TEXTURE_2D, mTexturesID[i]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            break;
        case -1 :
            std::cout << "[ERROR][SKYBOX] Unable to load file \"" << mSourceFile << "\""<< std::endl;
            return false;
        case -2:
            std::cout << "[ERROR][SKYBOX] Error while parsing file \"" << mSourceFile << "\"" << std::endl;
            return false;
        case -3:
            std::cout << "[ERROR][SKYBOX] Error while loading required textures" << std::endl;
            return false;
        default :
            break;
        }

                    //return false;
    }
    gwResource::load();
    return true;
}

bool gwe::gwSkybox::free(void)
{
    if(!isLoaded())
        return false;
    return gwResource::free();
}

bool gwe::gwSkybox::validElement(tinyxml2::XMLElement*element, const std::string &value)
{
    if(element != 0)
        if(std::string(element->Value()) == value)
            return true;
    return false;
}
