#include <iostream>
#include "MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    MainWindow window("GlowWorm Engine (Rev 1.0)", 800, 600);
    window.init();
    while(window.render());

    return 0;
}
