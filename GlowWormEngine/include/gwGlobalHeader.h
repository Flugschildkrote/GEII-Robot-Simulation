#ifndef GWGLOBALHEADER_H_INCLUDED
#define GWGLOBALHEADER_H_INCLUDED

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif // BUFFER_OFFSET

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <istream>
#include <set>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"

#ifdef _WIN32
#include "glad/glad.h"
#else
#define GL3_PROTOTYPES 1
#include "GL3/gl3.h"
#endif // _WIN32

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#pragma warning(push, 0)
#include "tinyxml2.h"
#pragma warning(pop)

namespace gwe
{
    namespace singleton
    {
        class gwGameManager;
        class gwResourceManager;
        class gwInputManager;
    }

    enum GW_COMPONENT_TYPE
    {
        GW_COMPONENT_CAMERA = 0,
        GW_COMPONENT_MATERIAL = 1,
        GW_COMPONENT_LIGHT = 2,
        GW_COMPONENT_MESH_RENDERER = 3,
        GW_COMPONENT_3DCOLLIDER  = 4,
        GW_COMPONENT_COUNT = 5
    };

    enum GW_LIGHT_TYPE
    {
        GW_LIGHT_DIRECTIONAL = 0,
        GW_LIGHT_POINT = 1
    };

    enum GW_LIGHT_ATTENUATION
    {
        GW_CONSTANT_ATTENUATION = 0,
        GW_LINEAR_ATTENUATION = 1,
        GW_EXP_ATTENUATION = 2
    };

    enum GW_LINK_STATE
    {
        GW_LINK_ADD = 0,
        GW_LINK_REMOVE = 1,
        GW_LINK_DENIED = 2
    };

    enum GW_SPACE_REFERENCE
    {
        GW_LOCAL_SPACE = 0,
        GW_PARENT_SPACE = 1,
        GW_WORLD_SPACE = 2
    };

    enum GW_RENDERER_TYPE
    {
        GW_SCREEN_RENDERER =0,
        GW_OFF_SCREEN_RENDERER = 1
    };


    class gwResource;
        class gwShader;
        class gwTexture;
        class gwMesh;
            class gwBoxMesh;
                class gwSkyboxMesh;
            class gwStaticMesh;

    class gwSkybox;
    class gwScene;
    class gwGameObject;
    class gwTransform;
    class gwSceneRenderer;

    class gwComponent;
        class gwCamera;
        class gwMaterial;
        class gwMeshRenderer;
        class gwLight;

    class gwSize
    {
    private :
        float mSize[2];
    public :
        gwSize(void) : mSize{0, 0} { }
        gwSize(float w, float h) : mSize{w, h} { }
        ~gwSize(void) { }

        gwSize& operator=(const gwSize &other)
        {
            if(&other != this)
            {
                mSize[0] = other.mSize[0];
                mSize[1] = other.mSize[1];
            }
            return *this;
        }
        gwSize operator*(const float &value) { return gwSize(mSize[0]*value, mSize[1]*value); }
        gwSize operator/(const float &value) { return gwSize(mSize[0]/value, mSize[1]/value); }
        void operator*=(const float &value) { mSize[0]*=value; mSize[1]*=value; }
        void operator/=(const float &value) { mSize[0]/=value; mSize[1]/= value; }
        bool operator==(const gwSize &other) { return (bool)((mSize[0] == other.mSize[0]) & (mSize[1] == other.mSize[1])); }
        bool operator!=(const gwSize &other) { return (bool)((mSize[0] != other.mSize[0]) | (mSize[1] != other.mSize[1])); }

        float getW(void) const { return mSize[0]; }
        float getH(void) const { return mSize[1]; }
        void setSize(float w, float h) { mSize[0] = w; mSize[1] = h; }
        void setWidth(float w) { mSize[0] = w; }
        void setHeight(float h) { mSize[1] = h; }
    };

}

#endif // GWGLOBALHEADER_H_INCLUDED
