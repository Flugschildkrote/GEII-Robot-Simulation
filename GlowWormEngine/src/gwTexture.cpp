#include "gwTexture.h"

using namespace gwe;
/*gwTexture::gwTexture(): Ressource(""), mId(0),mFile(""),mLargeur(0),mHauteur(0),mFormat(0),mFormatInterne(0),mgwTextureVide(false)
{

}*/

gwe::gwTexture::gwTexture(const std::string &name, const std::string &file) : gwResource(name), mId(0), mWidth(0), mHeight(0),mFormat(0),mInternalFormat(0),mIsEmptyTexture(false), mFile(file)
{

}

gwe::gwTexture::gwTexture(const std::string &name, int width ,int height , GLenum format, GLenum internalFormat) : gwResource(name), mId(0), mWidth(width), mHeight(height), mFormat(format), mInternalFormat(internalFormat), mIsEmptyTexture(true), mFile("")
{

}

gwe::gwTexture::~gwTexture()
{
    free();
}

bool gwe::gwTexture::load(void)
{
    if(isLoaded())
    {
        std::cout << "[WARNING][TEXTURE] Can't load \"" << getName() << "\", texture is already loaded. Instruction skipped." << std::endl;
        return false;
    }

    if(!mIsEmptyTexture)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        SDL_Surface *picture = IMG_Load(mFile.c_str());

        if(picture == 0)
        {
            std::cout << "Erreur : " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_Surface *flippedPicture = getFlippedPicture(picture);
        SDL_FreeSurface(picture);

        if(glIsTexture(mId) == GL_TRUE)
            glDeleteTextures(1,&mId);

        glGenTextures(1,&mId);
        glBindTexture(GL_TEXTURE_2D,mId);

        GLenum imgIntFormat(0);
        GLenum imgFormat(0);

        if(flippedPicture->format->BytesPerPixel == 3)
        {
            imgIntFormat = GL_RGB;
            if(flippedPicture->format->Rmask == 0xff)
                imgFormat = GL_RGB;
            else
                imgFormat = GL_BGR;
        }
        else if(flippedPicture->format->BytesPerPixel == 4)
        {
            imgIntFormat = GL_RGBA;
            if(flippedPicture->format->Rmask == 0xff)
                imgFormat = GL_RGBA;
            else
                imgFormat = GL_BGRA;
        }
        else
        {
            std::cout << "[ERROR][TEXTURE] \"" << getName() << "\" : Unable to load picture : unknown format." << std::endl;
            SDL_FreeSurface(flippedPicture);
            return false;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, imgIntFormat, flippedPicture->w, flippedPicture->h, 0, imgFormat, GL_UNSIGNED_BYTE, flippedPicture->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D,0);
        SDL_FreeSurface(flippedPicture);
    }
    else
    {
        if(glIsTexture(mId) == GL_TRUE)
            glDeleteTextures(1, &mId);
        glGenTextures(1, &mId);

        glBindTexture(GL_TEXTURE_2D, mId);
            glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    gwResource::load();
    return true;
}

bool gwe::gwTexture::free(void)
{
    if(!isLoaded())
    {
        std::cout << "[WARNING][TEXTURE] Can't free \"" << getName() << "\", texture is not loaded yet. Instruction skipped." << std::endl;
        return false;
    }

    if(glIsTexture(mId) == GL_TRUE)
        glDeleteTextures(1, &mId);
    gwResource::free();
    return true;
}

/*gwTexture::gwTexture(gwTexture const &gwTextureACopier) : Ressource(gwTextureACopier.mName)
{
    mFile = gwTextureACopier.mFile;
    mLargeur = gwTextureACopier.mLargeur;
    mHauteur = gwTextureACopier.mHauteur;
    mFormat = gwTextureACopier.mFormat;
    mFormatInterne = gwTextureACopier.mFormatInterne;
    mgwTextureVide = gwTextureACopier.mgwTextureVide;

    if(mgwTextureVide && glIsgwTexture(textureACopier.mId) == GL_TRUE)
    chargergwTextureVide();
    else if(glIsgwTexture(gwTextureACopier.mId) == GL_TRUE)
    load();
}*/

/*gwTexture::gwTexture(const std::string &name, int largeur,int hauteur, GLenum format,GLenum formatInterne,bool gwTextureVide) :
Ressource(name), mId(0), mFile(""), mLargeur(largeur),mHauteur(hauteur),mFormat(format), mFormatInterne(formatInterne),mgwTextureVide(gwTextureVide)
{

}*/

/*gwTexture& gwTexture::operator=(gwTexture const &gwTextureACopier)
{
    mFile = gwTextureACopier.mFile;
    mName = gwTextureACopier.mName;
    mLargeur = gwTextureACopier.mLargeur;
    mHauteur = gwTextureACopier.mHauteur;
    mFormat = gwTextureACopier.mFormat;
    mFormatInterne = gwTextureACopier.mFormatInterne;
    mgwTextureVide = gwTextureACopier.mgwTextureVide;

    if(mgwTextureVide == true && glIsgwTexture(gwTextureACopier.mId)  == GL_TRUE)
        chargergwTextureVide();
    else if(glIsgwTexture(gwTextureACopier.mId)  == GL_TRUE)
    load();

    return *this;
}*/

/*void gwTexture::chargergwTextureVide()
{
    if(glIsgwTexture(mId)  == GL_TRUE)
        glDeletegwTextures(1, &mId);

    glGengwTextures(1,&mId);

    glBindgwTexture(GL_gwTexture_2D,mId);
        glTexImage2D(GL_gwTexture_2D,0,mFormatInterne, mLargeur, mHauteur, 0,mFormat, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_gwTexture_2D, GL_gwTexture_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_gwTexture_2D, GL_gwTexture_MAG_FILTER, GL_NEAREST);
    glBindgwTexture(GL_gwTexture_2D, 0);
}*/

/*gwTexture::~gwTexture()
{
    glDeletegwTextures(1,&mId);
}*/

/*bool gwTexture::load()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    SDL_Surface *img = IMG_Load(mFile.c_str());

    if(img == 0)
    {
        std::cout << "Erreur : " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Surface *imageInverse = inverserImage(img);
    SDL_FreeSurface(img);

    if(glIsgwTexture(mId) == GL_TRUE)
        glDeletegwTextures(1,&mId);

    glGengwTextures(1,&mId);
    glBindgwTexture(GL_gwTexture_2D,mId);

    GLenum imgIntFormat(0);
    GLenum imgFormat(0);

    if(imageInverse->format->BytesPerPixel == 3)
    {
        imgIntFormat = GL_RGB;

        if(imageInverse->format->Rmask == 0xff)
        {
            imgFormat = GL_RGB;
        }
        else
        {
            imgFormat = GL_BGR;
        }
    }
    else if(imageInverse->format->BytesPerPixel == 4)
    {
        imgIntFormat = GL_RGBA;
        if(imageInverse->format->Rmask == 0xff)
        {
            imgFormat = GL_RGBA;
        }
        else
        {
            imgFormat = GL_BGRA;
        }
    }

    else
    {
        std::cout << "Erreur : format de l'image inconnu" << std::endl;
        SDL_FreeSurface(imageInverse);
        return false;
    }

    glTexImage2D(GL_gwTexture_2D,0,imgIntFormat,imageInverse->w,imageInverse->h,0,imgFormat,GL_UNSIGNED_BYTE,imageInverse->pixels);

    glTexParameteri(GL_gwTexture_2D,GL_gwTexture_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_gwTexture_2D,GL_gwTexture_MAG_FILTER,GL_NEAREST);

    glBindgwTexture(GL_gwTexture_2D,0);

    SDL_FreeSurface(imageInverse);
    return true;
}*/

GLint gwe::gwTexture::getID() { return mId; }

/*void gwTexture::setImgFile(const std::string &file)
{
    mFile = file;
}*/

SDL_Surface* gwe::gwTexture::getFlippedPicture(SDL_Surface *srcPicture) const
{
    SDL_Surface *imageInversee = SDL_CreateRGBSurface(0,srcPicture->w, srcPicture->h, srcPicture->format->BitsPerPixel,
                                                      srcPicture->format->Rmask, srcPicture->format->Gmask,
                                                      srcPicture->format->Bmask, srcPicture->format->Amask);


    unsigned char* pixelsSources = (unsigned char*) srcPicture->pixels;
    unsigned char* pixelsInverses = (unsigned char*) imageInversee->pixels;


    for(int i = 0; i < srcPicture->h; i++)
    {
        for(int j = 0; j < srcPicture->w * srcPicture->format->BytesPerPixel; j++)
        pixelsInverses[(srcPicture->w * srcPicture->format->BytesPerPixel * (srcPicture->h - 1 - i)) + j] =
        pixelsSources[(srcPicture->w * srcPicture->format->BytesPerPixel * i) + j];
    }
    return imageInversee;
}
