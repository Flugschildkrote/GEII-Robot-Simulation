#include "Window.h"


namespace
{

    void GLAPIENTRY
        MessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
    {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
    }

    // During init, enable debug output
  
}


Window::Window(const std::string &title, const int &width, const int &height) : mTitle(title), mWidth(width), mHeight(height),  mSDL_Window(0), mGL_Context(0),  mClosed(true)
{

}

Window::~Window()
{
    delete_SDL_Window();
}

bool Window::init(void)
{
    if(mClosed)
    {
        mSDL_Window = SDL_CreateWindow(mTitle.c_str()  , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE|SDL_RENDERER_PRESENTVSYNC);
        if(mSDL_Window == 0)
        {
            std::cout << "Error while creating the window \"" << mTitle << "\" : " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        mGL_Context = SDL_GL_CreateContext(mSDL_Window);
        if(mGL_Context == 0)
        {
            std::cout << SDL_GetError() << std::endl;
            SDL_DestroyWindow(mSDL_Window);
            mSDL_Window = 0;
            return false;
        }

        #ifdef _WIN32
        
        if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
        {
            std::cout << "Erreur lors de l'initialisation de GLAD : " << std::endl;
            SDL_GL_DeleteContext(mGL_Context);
            SDL_DestroyWindow(mSDL_Window);
            SDL_Quit();
            return false;
        }
        #endif // _WIN32

        std::cout << "Opengl Version : " << glGetString(GL_VERSION) << std::endl;
        std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;

        glEnable(GL_DEPTH_TEST);
        if(SDL_GL_SetSwapInterval(1) < 0)
        {
            std::cout << "Impossible d'activer le synchro verticale :" << SDL_GetError() << std::endl;
        }
        mClosed = false;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
   // SDL_MaximizeWindow(mSDL_Window);
    return true;
}

std::string Window::title(void) { return mTitle; }
int Window::width(void) { return mWidth; }
int Window::height(void) { return mHeight; }
bool Window::closed(void) { return mClosed; }

void Window::update(void)
{

}

void Window::delete_SDL_Window(void)
{
    if(mGL_Context != 0)
        SDL_GL_DeleteContext(mGL_Context);
    if(mSDL_Window != 0)
        SDL_DestroyWindow(mSDL_Window);
    mGL_Context = 0;
    mSDL_Window = 0;
    SDL_Quit();
    mClosed = true;
}
