#include "gwShader.h"

using namespace gwe;
/*gwShader::gwShader(void): Ressource("", SRC_SHADER), mVertexID(0),mGeometryID(0), mFragmentID(0), mProgramID(0),
mVertexSource(""),mGeometrySource(""), mFragmentSource(""), mUseGeometry(true)
{

}*/



/*gwShader::gwShader(const std::string &prefixName) : Ressource(prefixName, SRC_SHADER), mVertexID(0), mGeometryID(0), mFragmentID(0), mProgramID(0),
mVertexSource(prefixName+".vert"), mGeometrySource(prefixName+".geom"), mFragmentSource(prefixName+".frag"), mUseGeometry(true)
{

}*/

gwe::gwShader::gwShader(const std::string &name, const std::string &vertexSource,const std::string &fragmentSource, const std::string &geometrySource):
gwResource(name), mVertexID(0), mGeometryID(0), mFragmentID(0),
mProgramID(0), mVertexSource(vertexSource), mGeometrySource(geometrySource),mFragmentSource(fragmentSource), mUseGeometry(false)
{
    if(mGeometrySource.size() != 0)
        mUseGeometry = true;
}

gwe::gwShader::~gwShader()
{
    free();
}

GLuint gwe::gwShader::getProgramID() const { return mProgramID; }

bool gwe::gwShader::load(void)
{
    if(isLoaded())
    {
        std::cout << "[WARNING][SHADER] Can't load \"" << getName() << "\", shader is already loaded. Instruction skipped." << std::endl;
        return false;
    }

    if(glIsShader(mVertexID) == GL_TRUE)
        glDeleteShader(mVertexID);

    if(glIsShader(mFragmentID) == GL_TRUE)
        glDeleteShader(mFragmentID);

    if(glIsShader(mGeometryID) == GL_TRUE)
        glDeleteShader(mGeometryID);

    if(glIsProgram(mProgramID) == GL_TRUE)
        glDeleteProgram(mProgramID);

    if(!compileShader(mVertexID,GL_VERTEX_SHADER,mVertexSource))
    {
        std::cout << "[ERROR][SHADER] Unable to compile \"" << mVertexSource << "\" in \"" << getName() << "\"." << std::endl;
        return false;
    }
    std::cout << "[INFO][SHADER] Shader \"" << mVertexSource << "\" in \"" << getName() << "\" compiled." << std::endl;

    if(!compileShader(mFragmentID,GL_FRAGMENT_SHADER,mFragmentSource))
    {
        std::cout << "[ERROR][SHADER] Unable to compile \"" << mFragmentSource << "\" in \"" << getName() << "\"." << std::endl;
        return false;
    }
    std::cout << "[INFO][SHADER] Shader \"" << mFragmentSource << "\" in \"" << getName() << "\" compiled." << std::endl;

    if(mUseGeometry)
    {
        if(!compileShader(mGeometryID,GL_GEOMETRY_SHADER,mGeometrySource))
        {
            std::cout << "[ERROR][SHADER] Unable to compile \"" << mGeometrySource << "\" in \"" << getName() << "\"." << std::endl;
            return false;
        }
        std::cout << "[INFO][SHADER] Shader \"" << mGeometrySource << "\" in \"" << getName() << "\" compiled." << std::endl;
    }
    else
        std::cout << "[INFO][SHADER] Shader \"" << getName() << "\": geometry disabled." << std::endl;

    mProgramID = glCreateProgram();

    glAttachShader(mProgramID,mVertexID);
    glAttachShader(mProgramID,mFragmentID);
    if(mUseGeometry)
        glAttachShader(mProgramID,mGeometryID);

    glBindAttribLocation(mProgramID,0,"in_Vertex");
    glBindAttribLocation(mProgramID,1,"in_Colors");
    glBindAttribLocation(mProgramID,2,"in_TexCoord0");
    glBindAttribLocation(mProgramID,3,"in_Normals");
    glBindAttribLocation(mProgramID,4,"in_TextureID");

    glLinkProgram(mProgramID);

    GLint erreurLink(0);
    glGetProgramiv(mProgramID,GL_LINK_STATUS,&erreurLink);

    if(erreurLink != GL_TRUE)
    {
        GLint tailleErreur(0);
        glGetProgramiv(mProgramID,GL_INFO_LOG_LENGTH,&tailleErreur);

        char *erreur = new char[tailleErreur+1];

        glGetProgramInfoLog(mProgramID,tailleErreur,&tailleErreur,erreur);
        erreur[tailleErreur] = '\0';

        std::cout << "[ERROR][SHADER] Shader \"" << getName() << "\" : " << erreur << std::endl;
        delete[] erreur;
        glDeleteProgram(mProgramID);
        return false;
    }
    gwResource::load();
    return true;
}

bool gwe::gwShader::free(void)
{
    if(!isLoaded())
    {
        std::cout << "[WARNING][SHADER] Can't free \"" << getName() << "\", shader is not loaded yet. Instruction skipped." << std::endl;
        return false;
    }

    if(glIsShader(mVertexID) == GL_TRUE)
        glDeleteShader(mVertexID);
    if(glIsShader(mFragmentID) == GL_TRUE)
        glDeleteShader(mFragmentID);
    if(glIsShader(mGeometryID) == GL_TRUE)
        glDeleteShader(mGeometryID);
    if(glIsProgram(mProgramID) == GL_TRUE)
        glDeleteProgram(mProgramID);
    gwResource::free();
    return true;
}

bool gwe::gwShader::compileShader(GLuint &shader,GLenum type,std::string &fichierSource)
{
    shader = glCreateShader(type);
    if(shader == 0)
    {
        std::cout << "[ERROR][SHADER] Shader \"" << getName() << "\" : Unknown shader type" << std::endl;
        return false;
    }

    std::ifstream fichier(fichierSource.c_str());
    if(!fichier)
    {
        std::cout << "[ERROR][SHADER] Shader \"" << getName() << "\" : Unable to open file \"" << fichierSource << "\"." << std::endl;
        glDeleteShader(shader);
        return false;
    }

    /*Stockage des lignes du shader*/

    std::string ligne;
    std::string codeSource;

    while(getline(fichier,ligne))
        codeSource += ligne + '\n';

    fichier.close();

    const GLchar* chaineCodeSource = codeSource.c_str();

    glShaderSource(shader,1,&chaineCodeSource,0);
    glCompileShader(shader);

    GLint erreurCompilation(0);
    glGetShaderiv(shader,GL_COMPILE_STATUS,&erreurCompilation);

    if(erreurCompilation != GL_TRUE)
    {
        GLint tailleErreur(0);
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&tailleErreur);

        char *erreur = new char[tailleErreur + 1];
        glGetShaderInfoLog(shader,tailleErreur,&tailleErreur,erreur);
        erreur[tailleErreur] = '\0';

        std::cout << "[ERROR][SHADER] Shader \"" << getName() << "\" : " << erreur << std::endl;

        delete[] erreur;
        glDeleteShader(shader);
        return false;
    }
    return true;
}
