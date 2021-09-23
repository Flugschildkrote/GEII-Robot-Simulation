#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include "gwGlobalHeader.h"
#include "gwResource.h"

namespace gwe {

class gwShader : public gwResource
{
public:
    gwShader(const std::string &name, const std::string &vertexSource,const std::string &fragmentSource, const std::string &geometrySource = "");
    ~gwShader(void);

   // gwShader(const std::string &prefixName);
   // gwShader(gwShader const &gwShaderACopier);
    //gwShader& operator=(gwShader const &gwShaderACopier);

    GLuint getProgramID(void) const;
    virtual bool load(void);
    virtual bool free(void);

protected :
   // gwShader(const std::string &name, const std::string &vertexSource, const std::string &fragmentSource, const std::string &geometrySource = "");
   // ~gwShader();

    //virtual bool load(void);

    bool compileShader(GLuint &shader, GLenum type , std::string &srcFile);


    GLuint mVertexID;
    GLuint mGeometryID;
    GLuint mFragmentID;

    GLuint mProgramID;

    std::string mVertexSource;
    std::string mGeometrySource;
    std::string mFragmentSource;
    bool mUseGeometry;
};

}
#endif // SHADER_H_INCLUDED
