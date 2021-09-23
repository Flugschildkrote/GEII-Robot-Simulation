#ifndef gwTexture_H_INCLUDED
#define gwTexture_H_INCLUDED

#include "gwGlobalHeader.h"
#include "gwResource.h"

namespace gwe {

class gwTexture : public gwResource
{
public:
    gwTexture(const std::string &name, const std::string &file = "");
    gwTexture(const std::string &name, int width ,int height , GLenum format, GLenum internalFormat);

    ~gwTexture(void);

    virtual bool load(void);
    virtual bool free(void);

    GLint getID(void);

    //void chargergwTextureVide(void);
   // void setImgFile(const std::string &file);
    SDL_Surface* getFlippedPicture(SDL_Surface *srcPicture) const;
    SDL_Surface* getSDL_Surface(void);
protected :
   // gwTexture();
    //gwTexture(std::string name, const std::string &file);
    //gwTexture(gwTexture const &gwTextureACopier);
    //gwTexture(const std::string &name, int largeur,int hauteur,GLenum format,GLenum formatInterne,bool gwTextureVide);
    //gwTexture& operator=(gwTexture const &gwTextureACopier);
    //~gwTexture();

    GLuint mId;
    unsigned int mWidth, mHeight;
    GLenum mFormat;
    GLenum mInternalFormat;
    bool mIsEmptyTexture;
    std::string mFile;
    SDL_Surface* getSurface;

    //int mLargeur;
   // int mHauteur;
   // GLenum mFormat;
   // GLenum mFormatInterne;
   // bool mgwTextureVide;
    //SDL_Surface mImageInverse;
};

}
#endif // gwTexture_H_INCLUDED
