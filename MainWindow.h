#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Window.h"
#include "Physics.h"

class MainWindow : public Window
{
public:
    MainWindow(const std::string &title, const int &width, const int &height);
    virtual ~MainWindow();
    virtual bool init();
    virtual bool render();
protected:
    uint32_t mLastTime = 0;
    uint32_t referenceTime = 0;
    int frameCount = 0;
    float deltaTime = 0;
    GLDebugDrawer *mDebugDrawer;

};

#endif // MAINWINDOW_H
