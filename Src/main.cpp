#include <X11/Xlib.h>

#include <stdio.h>

const size_t height  = 500;
const size_t width   = 500;
const size_t x_pos   = 0;
const size_t y_pos   = 0;
const size_t border  = 15;

#define RED_CONSOLE  "\033[0;31m"
#define DEFAULT_CONSOLE "\033[0m\n"
#define GREEN_CONSOLE "\033[0;32m"

int main()
{
    Display* display  = nullptr;
    int      screen   = -1;;
    Window   root_win = 0;

    Window window = 0;
    XEvent event;

    display = XOpenDisplay("NULL");

    if (!display)
    {
        printf(RED_CONSOLE "Cant open display\n" DEFAULT_CONSOLE);
        return 0;
    }

    screen = DefaultScreen(display);

    root_win = RootWindow(display, screen);

    window = XCreateSimpleWindow(display, root_win, 
                                 x_pos, y_pos, width, height, border, 
                                 BlackPixel(display, screen), 
                                 WhitePixel(display, screen));
    
    XMapWindow(display, window);
    
    // Event lopp
    while (XNextEvent(display, &event) == 0)
    {
        
    }

    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}