#include <X11/Xlib.h>
#include <X11/X.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <stdio.h>

#include "../Libs/Graphic.h"

const size_t height  = 500;
const size_t width   = 500;
const size_t x_pos   = 0;
const size_t y_pos   = 0;
const size_t border  = 15;

int Block::counter = 0;

const float rot_angle = 15;

#define RED_CONSOLE  "\033[0;31m"
#define DEFAULT_CONSOLE "\033[0m\n"
#define GREEN_CONSOLE "\033[0;32m"

void Draw() 
{
    glClearColor(0.7, 0.5, 0.5, 0);
    // GL_COLOR_BUFFER_BIT - clear screen color
    // GL_DEPTH_BUFFER_BIT - clear screen depth
    glClear(GL_COLOR_BUFFER_BIT);

    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1., 1., -1., 1., 1., 20.);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

    glBegin(GL_QUADS);
        glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
        glVertex3f( .75, -.75, 0.);
        glVertex3f( .75,  .75, 0.);
        glVertex3f(-.75,  .75, 0.);
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

void Redraw(Display* display, Window* window, Block blocks[], size_t block_number)
{
    glLoadIdentity();
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
        
    for (int i = 0; i < block_number; i++)
        blocks[i].Draw();
                
    glXSwapBuffers(display, *window);

}

void Run(Display* display, Window* window, GLXContext gl_context)
{
    Block blocks[100];

    Point size(0.2, 0.2);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Point pos((i - 5) / 5., (j - 4) / 5.);
            //Color color(rand() / (double)RAND_MAX, rand() / (double)RAND_MAX, rand() / (double)RAND_MAX);
            Color color(0.5, 0.7, 0.5);


            blocks[i* 10 + j] = Block(pos, size, color);
        }
    }
    
    XEvent            event;
    XWindowAttributes gwa;

    // Event loop
    while (XNextEvent(display, &event) == 0)
    {
        switch(event.type)
        {
            case Expose:
            {
                XGetWindowAttributes(display, *window, &gwa);
                glViewport(0, 0, gwa.width, gwa.height);
                Redraw(display, window, blocks, sizeof(blocks) / sizeof(Block));
                break;
            }

            case ButtonPress:
            {
                Point pos(event.xbutton.x, event.xbutton.y);

                pos.x -= gwa.width  / 2;
                pos.x /= gwa.width  / 2;
                pos.y  = gwa.height / 2 - pos.y;
                pos.y /= gwa.height / 2;

                for (int i = 0; i < sizeof(blocks) / sizeof(Block); i++)
                {
                    if (blocks[i].InsideP(pos))
                    {                     
                        blocks[i].color = Color();
                        break;
                    }
                }

                Redraw(display, window, blocks, sizeof(blocks) / sizeof(Block));

                break;
            }

            case KeyPress:
            {
                Redraw(display, window, blocks, sizeof(blocks) / sizeof(Block));
                break;
            }
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
    swa.event_mask = KeyPressMask | ExposureMask | ButtonPressMask;
    Window window = CreateWindow(display, screen, root_win, &swa, visual_info);

    // Creating context
    gl_context = glXCreateContext(display, visual_info, NULL, GL_TRUE);
    glXMakeCurrent(display, window, gl_context);

    // ?
    //glEnable(GL_DEPTH_TEST);

    Run(display, &window, gl_context);

    Destruct(display, &window, gl_context);
}