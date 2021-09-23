#ifndef WINDOW_H
#define WINDOW_H

#include "gwGlobalHeader.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"

class Window
{
public:
    Window(const std::string &title, const int &width, const int &height);
    virtual ~Window();
    virtual bool init(void);
    virtual bool render(void) = 0;

    std::string title(void);
    int width(void);
    int height(void);
    bool closed(void);

protected:
    std::string mTitle;
    int mWidth;
    int mHeight;
    SDL_Window *mSDL_Window;
    SDL_GLContext mGL_Context;
    //SDL_Event mEvent;
    bool mClosed;

    void update(void);
    void delete_SDL_Window(void);
};

#endif // WINDOW_H
