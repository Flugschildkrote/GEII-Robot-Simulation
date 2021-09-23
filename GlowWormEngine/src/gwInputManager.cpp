#include "gwInputManager.h"
using namespace gwe::singleton;

gwe::singleton::gwInputManager::gwInputManager():mSizeChanged(false),mWindowSize(0,0),mPreviousSize(0,0), mX(0),mY(0),mXRel(0),mYRel(0),mQuit(false)
{
    for(int i(0); i< SDL_NUM_SCANCODES; i++)
    {
        mTouches[i] = false;
        mKeyPressed[i] = false;
        mKeyReleased[i] = false;
    }
    for(int i(0); i < 8; i++)
        mMouseButtons[i] = false;
    std::cout << "[SINGLETON] \"InputManager\" created." << std::endl;
}

gwe::singleton::gwInputManager::~gwInputManager(){ std::cout << "[SINGLETON] \"InputManager\" deleted." << std::endl; }

void gwe::singleton::gwInputManager::updateEvent()
{
    mXRel = 0;
    mYRel = 0;
    mSizeChanged = false;


    while(SDL_PollEvent(&mEvents))
    {
        switch(mEvents.type)
        {
        case SDL_KEYDOWN:
            mTouches[mEvents.key.keysym.scancode] = true;
            break;

        case SDL_KEYUP:
            mTouches[mEvents.key.keysym.scancode] = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mMouseButtons[mEvents.button.button] = true;
            break;

        case SDL_MOUSEBUTTONUP:
            mMouseButtons[mEvents.button.button] = false;
            break;

        case SDL_MOUSEMOTION:
            mX = mEvents.motion.x;
            mY = mEvents.motion.y;

            mXRel += mEvents.motion.xrel;
            mYRel += mEvents.motion.yrel;
            break;

        case SDL_WINDOWEVENT:
            if(mEvents.window.event == SDL_WINDOWEVENT_CLOSE)
                mQuit = true;
            else if(mEvents.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                mSizeChanged = true;
                mPreviousSize = mWindowSize;
                std::cout << "Previous Size : " << mPreviousSize.getW() << " ; " << mPreviousSize.getH() << std::endl;
                mWindowSize.setSize(mEvents.window.data1, mEvents.window.data2);
            }
            break;
        default:
            break;
        }
    }

    if((mTouches[SDL_SCANCODE_LALT] | mTouches[SDL_SCANCODE_RALT]) && mTouches[SDL_SCANCODE_F4])
        mQuit = true;
}

bool gwe::singleton::gwInputManager::quit() const { return mQuit; }
bool gwe::singleton::gwInputManager::getKey(const SDL_Scancode &key) const { return mTouches[key]; }
bool gwe::singleton::gwInputManager::getMouseButton(const Uint8 mouseButton) const { return mMouseButtons[mouseButton]; }

bool gwe::singleton::gwInputManager::mouseMoved() const
{
    if(mXRel == 0 && mYRel == 0)
        return false;

    return true;
}

int gwe::singleton::gwInputManager::getX() const { return mX; }
int gwe::singleton::gwInputManager::getY() const { return mY; }
int gwe::singleton::gwInputManager::getXRel() const { return mXRel; }
int gwe::singleton::gwInputManager::getYRel() const { return mYRel; }

void gwe::singleton::gwInputManager::showCursor(bool show) const
{
    if(show)
        SDL_ShowCursor(SDL_ENABLE);
    else
        SDL_ShowCursor(SDL_DISABLE);
}

void gwe::singleton::gwInputManager::lockCursor(bool lock) const
{
    if(lock)
        SDL_SetRelativeMouseMode(SDL_TRUE);
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);
}
