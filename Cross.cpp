#include "GLPlatform.h"
#include "Cross.h"
#include "utils.h"


namespace CrossGame
{

    void Cross::draw()
    {
        // Визначення властивостей матеріалу:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, GraphUtils::shininess);
        // Запис поточної матриці в стек
        // (збереження вмісту поточної матриці для подальшого використання):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() + getYSize() / 2, getZCenter());

        glRotatef(90, 1, 0, 0);

        float s = getXSize(); //Запам'ятати переданий розмір
        float sW = s / 2; //Довжина лінії хреста
        float sH = s / 10; //Ширина лінії хреста
        float sG = s / 7;//Глибина


        glRotatef(45, 0, 0, 1); //Повертаємо хрестик на 45 градусів

        for (int i = 0; i < 2; i++) // В циклі дублюємо код але з поворотом  Малюємо прямокутник з зміщенням 90 градусів що б вийшов хрестик
        {
            glBegin(GL_QUADS);
            glNormal3d(0, 0, 1);
            glVertex3d(-sW, -sG, sH);
            glVertex3d(sW, -sG, sH);
            glVertex3d(sW, sG, sH);
            glVertex3d(-sW, sG, sH);
            //Бічна площина 1
            glNormal3d(0, 1, 0);
            glVertex3d(-sW, sG, sH);
            glVertex3d(sW, sG, sH);
            glVertex3d(sW, sG, -sH);
            glVertex3d(-sW, sG, -sH);
            //Бічна площина 2
            glNormal3d(0, -1, 0);
            glVertex3d(-sW, -sG, sH);
            glVertex3d(sW, -sG, sH);
            glVertex3d(sW, -sG, -sH);
            glVertex3d(-sW, -sG, -sH);

            //Кришка з верху 3
            glNormal3d(0, 0, -1);
            glVertex3d(-sW, -sG, -sH);
            glVertex3d(sW, -sG, -sH);
            glVertex3d(sW, sG, -sH);
            glVertex3d(-sW, sG, -sH);

            //Кришка front
            glNormal3d(1, 0, 0);
            glVertex3d(sW, -sG, -sH);
            glVertex3d(sW, -sG, sH);
            glVertex3d(sW, sG, sH);
            glVertex3d(sW, sG, -sH);


            //Кришка back
            glNormal3d(-1, 0, 0);
            glVertex3d(-sW, -sG, -sH);
            glVertex3d(-sW, -sG, sH);
            glVertex3d(-sW, sG, sH);
            glVertex3d(-sW, sG, -sH);
            glEnd();
            glRotatef(90, 0, 0, 1); //Повернути на 90 градумів усе наступне що буде намальоване
        }

        // Відновлення поточної матриці зі стека:
        glPopMatrix();
    }

}
