#include "gwResourceManager.h"
#include "gwResource.h"

using namespace gwe::singleton;

gwe::singleton::gwResourceManager::gwResourceManager()
{
    std::cout << "[SINGLETON] \"ResourceManager\" created." << std::endl;
}

gwe::singleton::gwResourceManager::~gwResourceManager()
{
    while(mResources.size() != 0)
    {
        std::map<std::string, gwResource*>::iterator it(mResources.begin());
        delete it->second;
    }
    /*for(std::map<std::string, gwResource*>::iterator it(mResources.begin()); it != mResources.end(); it++)
        delete it->second;*/
    std::cout << "[SINGLETON] \"ResourceManager\" deleted." << std::endl;
}

bool gwe::singleton::gwResourceManager::addResource(gwResource* resource)
{
    if(mResources.find(resource->getName()) != mResources.end())
    {
        std::cout << "[ERROR] A resource called \"" << resource->getName() << "\" is already existing, instruction skipped." << std::endl;
        return false;
    }
    mResources[resource->getName()] = resource;
    return true;
}

gwe::gwResource* gwe::singleton::gwResourceManager::getResource(const std::string &resourceName) const
{
    if(mResources.find(resourceName) == mResources.end())
    {
        std::cout << "[ERROR] Can't find Resource \"" << resourceName << "\"" << std::endl;
        return nullptr;
    }
    return mResources.at(resourceName);
}

bool gwe::singleton::gwResourceManager::removeResource(const std::string &resourceName)
{
    if(mResources.find(resourceName) == mResources.end())
    {
        std::cout << "[ERROR] Can't find remove Resource \"" << resourceName << "\", instruction skipped." << std::endl;
        return false;
    }
    mResources.erase(resourceName);
    return true;
}

/*Shader* RessourceManager::getShader(const std::string &shaderName)
{
    if(mShaders.find(shaderName) == mShaders.end())
    {
        LOG("Can't find Shader \""+shaderName+"\"", LOG_ERROR, 1);
        return nullptr;
    }
    return mShaders.at(shaderName);
}

Shader* RessourceManager::addShader(const std::string &shaderName, const std::string &vertex, const std::string &fragment, const std::string &geometry)
{
    if(mShaders.find(shaderName) == mShaders.end())
    {
        Shader* tmpShader = new Shader(shaderName, vertex, fragment, geometry);
        if(!tmpShader->load())
            return nullptr;
        mShaders[shaderName] = tmpShader;
    }
    else
        LOG("Shader \""+shaderName+"\" already created", LOG_WARNING, 1);
    return mShaders[shaderName];
}

Texture* RessourceManager::getTexture(const std::string &textureName)
{
    if(mTextures.find(textureName) == mTextures.end())
    {
        LOG("Can't find Texture \""+textureName+"\"", LOG_ERROR, 1);
        return nullptr;
    }
    return mTextures.at(textureName);
}

Texture* RessourceManager::addTexture(const std::string &textureName, const std::string &filePath)
{
    if(mTextures.find(textureName) == mTextures.end())
    {
        Texture* tmpTexture = new Texture(textureName, filePath);
        if(!tmpTexture->load())
        {
            LOG("Can't load texture", LOG_ERROR, 1);
            return nullptr;
        }
        mTextures[textureName] = tmpTexture;
    }
    else
        LOG("Texture \""+textureName+"\" already created", LOG_WARNING, 1);
    return mTextures[textureName];
}



Mesh* RessourceManager::getMesh(const std::string &meshName)
{
    if(mMeshs.find(meshName) == mMeshs.end())
    {
        LOG("Can't find Mesh \""+meshName+"\"", LOG_ERROR, 1);
        return nullptr;
    }
    return mMeshs.at(meshName);
}


Mesh* RessourceManager::addMesh(const std::string &meshName, const std::string &filePath)
{
    if(mMeshs.find(meshName) == mMeshs.end())
    {
        Mesh* tmpMesh = new Mesh(meshName, filePath);
        if(!tmpMesh->load())
            return nullptr;
        mMeshs[meshName] = tmpMesh;
    }
    else
        LOG("Mesh \""+meshName+"\" already created", LOG_WARNING, 1);
    return mMeshs[meshName];
}

AnimatedMeshData* RessourceManager::addAnimatedMesh(const std::string &meshName, const std::string &filePath)
{
    if(mAnimatedMeshsData.find(meshName) != mAnimatedMeshsData.end())
    {
        LOG("Animated Mesh \""+meshName+"already created", LOG_WARNING, 1);
        return mAnimatedMeshsData[meshName];
    }

    AnimatedMeshData* data = new AnimatedMeshData;
    data->name = meshName;

    TiXmlDocument document(filePath.c_str());
    if(!document.LoadFile())
    {
        LOG("Can't load \""+filePath+"\"", LOG_ERROR, 1);
        return nullptr;
    }

    std::string dir = "";
    std::string tmpStr = "";
    for(unsigned int i(0); i < filePath.size(); i++)
    {
        tmpStr += filePath[i];
        if(filePath[i] == '/')
            dir = tmpStr;
    }
    TiXmlElement *model = document.RootElement()->FirstChildElement("Model");
    data->mesh = model->GetText();
    this->addMesh(data->mesh, dir+data->mesh);
    int animCount = std::stoi(document.RootElement()->FirstChildElement("Anim_Count")->GetText());


    TiXmlElement *lastAnim = model;
    for(unsigned int i(0); i < animCount ; i++)
    {
        TiXmlElement* anim = lastAnim->NextSiblingElement("Animation");
        AnimationData animData;
        animData.name = anim->Attribute("name");
        animData.duration = std::stof(anim->Attribute("duration"));
        int frameCount = std::stoi(anim->Attribute("frames"));
        animData.frames.reserve(frameCount);

        TiXmlElement* frameElement = anim->FirstChildElement();
        for(int i(0); i < frameCount; i++)
        {
            animData.frames.push_back(frameElement->GetText());
            this->addMesh(frameElement->GetText(), dir+frameElement->GetText());
            frameElement = frameElement->NextSiblingElement();
        }

        data->animations.push_back(animData);
        lastAnim = anim;

    }
    mAnimatedMeshsData[meshName] = data;

    std::cout << "Main Mesh : " << data->mesh << std::endl;
    std::cout << "Name : " << data->name << std::endl;
    std::cout << "Nb Anims : " << data->animations.size() << std::endl;
    for(unsigned int i(0); i < data->animations.size(); i++)
    {
        std::cout << "Animation " << i << " : " << data->animations[i].name << " Duration : " << data->animations[i].duration << std::endl;
        std::cout << "Nb Frames : " << data->animations[i].frames.size() << std::endl;
        for(int j(0); j < data->animations[i].frames.size(); j++)
            std::cout << "Frame " << j << "\"" << data->animations[i].frames[i] << "\"" << std::endl;
    }



    return data;
}

AnimatedMeshData* RessourceManager::getAnimatedMesh(const std::string &meshName)
{
    if(mAnimatedMeshsData.find(meshName) == mAnimatedMeshsData.end())
    {
        LOG("Can't find Mesh \""+meshName+"\"", LOG_ERROR, 1);
        return nullptr;
    }
    return mAnimatedMeshsData.at(meshName);
}*/

