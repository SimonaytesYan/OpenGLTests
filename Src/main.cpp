#include <X11/Xlib.h>
#include <X11/X.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <stdio.h>

const size_t height  = 500;
const size_t width   = 500;
const size_t x_pos   = 0;
const size_t y_pos   = 0;
const size_t border  = 15;

#define RED_CONSOLE  "\033[0;31m"
#define DEFAULT_CONSOLE "\033[0m\n"
#define GREEN_CONSOLE "\033[0;32m"

void DrawAQuad() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1., 1., -1., 1., 1., 20.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

    glBegin(GL_QUADS);
        glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
        glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
        glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
        glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
    glEnd();
} 

Window CreateWindow(Display* display, int screen, Window root_win, XSetWindowAttributes* swa, XVisualInfo *visual_info)
{
    Window window = 0;

    window = XCreateWindow(display, root_win, x_pos, y_pos, width, height, border, 
                           visual_info->depth, InputOutput, 
                           visual_info->visual, CWColormap | CWEventMask, swa);
    
    XMapWindow(display, window);
    XStoreName(display, window, "Gay!!!");

    return window;
}

void Run(Display* display, Window* window, GLXContext gl_context)
{
    XEvent            event;
    XWindowAttributes gwa;
    
    // Event lopp
    while (XNextEvent(display, &event) == 0)
    {
        switch(event.type)
        {
            case Expose:
            {
                XGetWindowAttributes(display, *window, &gwa);
                glViewport(0, 0, gwa.width, gwa.height);
        	    DrawAQuad(); 
                glXSwapBuffers(display, *window);
                break;
            }

            case KeyPress:
                return;
        }        
    }
}

void Destruct(Display* display, Window* window, GLXContext gl_context)
{
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, gl_context);
    XUnmapWindow(display, *window);
    XDestroyWindow(display, *window);
    XCloseDisplay(display);
}

int CreateDisplay(Display** display, int* screen, Window* root_win)
{
    Display* new_display = XOpenDisplay(NULL);

    if (!new_display)
    {
        printf(RED_CONSOLE "Cant open display\n" DEFAULT_CONSOLE);
        return -1;
    }

    *screen   = DefaultScreen(new_display);
    *root_win = RootWindow   (new_display, *screen);

    *display = new_display;

    return 0;
}

int main()
{
    // X11 variables
    Display*              display  = nullptr;
    int                   screen   = -1;
    Window                root_win = 0;
    XSetWindowAttributes  swa;

    CreateDisplay(&display, &screen, &root_win);

    // OpenGL variables
    XVisualInfo *visual_info  = nullptr;
    GLint        attr[]       = { GLX_RGBA, GLX_DEPTH_SIZE, 24, 
                                  GLX_DOUBLEBUFFER, None };
    Colormap   colormap;
    GLXContext gl_context;  

    // Creating window
    visual_info = glXChooseVisual(display, screen, attr);
    if (!visual_info)
    {
        printf(RED_CONSOLE "Cant choose visual\n" DEFAULT_CONSOLE);
        return 0;
    }

    colormap = XCreateColormap(display, root_win, visual_info->visual, AllocNone);
    swa.colormap   = colormap;
    swa.event_mask = KeyPressMask | ExposureMask;
    Window window = CreateWindow(display, screen, root_win, &swa, visual_info);

    // Creating context
    gl_context = glXCreateContext(display, visual_info, NULL, GL_TRUE);
    glXMakeCurrent(display, window, gl_context);

    // ?
    glEnable(GL_DEPTH_TEST);

    Run(display, &window, gl_context);

    Destruct(display, &window, gl_context);
}