#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "gwGlobalHeader.h"
#include "gwSingleton.h"

namespace gwe { namespace singleton {


class gwInputManager : public gwSingleton<gwInputManager>
{
    friend class gwSingleton<gwInputManager>;
public:
    void updateEvent();
    bool quit() const;

    bool getKey(const SDL_Scancode &key) const ;
    bool keyPressed(const SDL_Scancode &key);
    bool getMouseButton(const Uint8 mouseButton)const;
    bool mouseMoved() const ;

    int getX() const;
    int getY() const;
    int getXRel() const;
    int getYRel() const;

    bool sizeChanged(void) const { return mSizeChanged; }
    gwSize getWindowSize(void) const { return mWindowSize; }
    gwSize getPreviousWindowSize(void) const { return mPreviousSize; }
    SDL_Event* sdlEvent(void) { return &mEvents; }

    void showCursor(bool show) const;
    void lockCursor(bool lock) const;
protected :
    gwInputManager();
    ~gwInputManager();

    SDL_Event mEvents;

    bool mTouches[SDL_NUM_SCANCODES];
    bool mKeyPressed[SDL_NUM_SCANCODES];
    bool mKeyReleased[SDL_NUM_SCANCODES];
    bool mMouseButtons[8];
    bool mSizeChanged;
    gwSize mWindowSize;
    gwSize mPreviousSize;

    int mX;
    int mY;
    int mXRel;
    int mYRel;

    bool mQuit;
};

}}
#endif // INPUT_H_INCLUDED
