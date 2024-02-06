#ifndef SYM_GRAPHIC
#define SYM_GRAPHIC

#include <stdlib.h>

struct Color
{
    float r; 
    float g; 
    float b;

    Color()
    {
        r = rand() / (double)RAND_MAX;
        g = rand() / (double)RAND_MAX;
        b = rand() / (double)RAND_MAX;
    }

    Color(float _r, float _g, float _b)
    {
        r = _r;
        g = _g;
        b = _b;
    }
};

struct Object
{
    Object()
    { }

    virtual void Draw() = 0;
};

//struct Vec2
//{
//    
//}

struct Point : Object
{
    float x;
    float y;

    Color color;

    Point(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    Point()
    {
        x = 0;
        y = 0;
    }

    void Dump()
    {
        printf("%lg %lg\n", x, y);
    }

    void Draw() override
    {
        glBegin(GL_POINTS);

        glColor3f(color.r, color.g, color.b);
        glVertex2f(x, y);

        glEnd();
    }
};

struct Block : Object
{
    static int counter;

    size_t id;
    Color  color;
    Point  position;
    Point  size;

    Block()
    {
        id = Block::counter++;
    }

    Block(Point pos, Point sz, Color clr)
    {
        position = pos;
        size     = sz;
        color    = clr;

        id = Block::counter++;
    }

    void Say()
    {
        printf("I`m block number %d\n", id);
    }

    bool InsideP(Point p)
    {
        return position.x          <= p.x && p.x <= position.x + size.x  &&
               position.y - size.y <= p.y && p.y <= position.y;
    }

    void Dump()
    {
        printf("pos   = %lg %lg\n", position.x, position.y);
        printf("size  = %lg %lg\n", size.x, size.y);
        printf("color = %lg %lg %lg\n", color.r, color.g, color.b);
        printf("id    = %d\n", id);
    }

    void Draw() override
    {
        glBegin(GL_QUADS);
        
        glColor3f(color.r, color.g, color.b);
        
        glVertex3f(position.x, position.y, 0);
        glVertex3f(position.x, position.y - size.y, 0);
        glVertex3f(position.x + size.x, position.y - size.y, 0);
        glVertex3f(position.x + size.x, position.y, 0);

        glEnd();
    }
};

#endif // SYM_GRAPHIC