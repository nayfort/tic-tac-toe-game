// utils.cpp

#include <time.h> // для отримання поточного часу
#include "GLPlatform.h"
#include "utils.h"

namespace GraphUtils
{
    // Визначення кольорів:
    float diffWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambiWhite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float specWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    float diffBlue[] = { 0.0f, 0.0f, 0.6f, 1.0f };
    float ambiBlue[] = { 0.1f, 0.1f, 0.2f, 1.0f };
    float specBlue[] = { 0.2f, 0.2f, 0.8f, 1.0f };

    float diffGray[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    float ambiGray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float specGray[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    float diffRed[] = { 0.6f, 0.0f, 0.0f, 1.0f };
    float ambiRed[] = { 0.2f, 0.1f, 0.1f, 1.0f };
    float specRed[] = { 0.8f, 0.2f, 0.2f, 1.0f };

    float diffYellow[] = { 0.9f, 0.9f, 0, 1.0f };
    float ambiYellow[] = { 0.2f, 0.2f, 0.1f, 1.0f };
    float specYellow[] = { 1.0f, 1.0f, 0.2f, 1.0f };

    float diffGreen[] = { 0, 0.5f, 0, 1.0f };
    float ambiGreen[] = { 0.1f, 0.2f, 0.1f, 1.0f };
    float specGreen[] = { 0.2f, 0.7f, 0.2f, 1.0f };

    float diffOrange[] = { 0.9f, 0.2f, 0, 1.0f };
    float ambiOrange[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float specOrange[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    float diffLightBlue[] = { 0, 0.6f, 0.9f, 1.0f };
    float ambiLightBlue[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float specLightBlue[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    float diffViolet[] = { 0.5f, 0, 0.5f, 1.0f };
    float ambiViolet[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float specViolet[] = { 0.8f, 0.8f, 0.8f, 1.0f };



    // Малювання паралелепіпеда
    void parallelepiped(float length, float width, float height)
    {
        glBegin(GL_QUAD_STRIP);
        //грань 1 || YZ, x<0
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-length / 2, -width / 2, -height / 2);
        glVertex3f(-length / 2, -width / 2, height / 2);
        glVertex3f(-length / 2, width / 2, -height / 2);
        glVertex3f(-length / 2, width / 2, height / 2);

        //грань 2 || ZX, y>0
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(length / 2, width / 2, -height / 2);
        glVertex3f(length / 2, width / 2, height / 2);

        //грань 3 || YZ, x>0
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(length / 2, -width / 2, -height / 2);
        glVertex3f(length / 2, -width / 2, height / 2);

        //грань 4 || ZX y<0
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-length / 2, -width / 2, -height / 2);
        glVertex3f(-length / 2, -width / 2, height / 2);
        glEnd();

        glBegin(GL_QUADS);
        //грань 5 || YX, z>0
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-length / 2, -width / 2, height / 2);
        glVertex3f(-length / 2, width / 2, height / 2);
        glVertex3f(length / 2, width / 2, height / 2);
        glVertex3f(length / 2, -width / 2, height / 2);

        //грань 6  || YX, z<0
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-length / 2, -width / 2, -height / 2);
        glVertex3f(-length / 2, width / 2, -height / 2);
        glVertex3f(length / 2, width / 2, -height / 2);
        glVertex3f(length / 2, -width / 2, -height / 2);
        glEnd();
    }

    // Відображення рядка тексту вказаним шрифтом у вказаній позиції
    void drawString(void* font, const char* text, float x, float y)
    {
        if (!text) // нульовий указівник
        {
            return;
        }
        // Встановлення позиції тексту:
        glRasterPos2f(x, y);
        while (*text)
        {
            // Рядок виводиться посимвольно:
            glutBitmapCharacter(font, *text);
            text++;
        }
    }
}
