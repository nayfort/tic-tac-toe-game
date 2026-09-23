// Scene.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "GLPlatform.h"
#include <stdio.h>
#include "Scene.h"
#include "utils.h"
#include "Board.h"
#include <iostream>

namespace CrossGame
{
    using namespace GraphUtils;


    Scene::Scene(float xStep, float zStep)
    {
        this->xStep = xStep;
        this->zStep = zStep;
        // Здійснюємо ініціалізацію параметрів перед першою грою:
        initialize();
    }

    Scene::~Scene()
    {
        // Видаляємо всі фігури:
        for (std::size_t i = 0; i < shapes.size(); i++)
        {
            delete shapes[i];
        }
    }


    // Перерахування індексу масиву fields в координату x
    float Scene::allocX(int i)
    {
        return  xStep * i - (N - 1) * xStep / 2;
    }

    // Перерахування індексу масиву fields в координату z
    float Scene::allocZ(int i)
    {
        return  zStep * i - (M - 1) * zStep / 2;
    }



    // Ініціалізація даних (виконується спочатку, а потім з кожним оновленням гри):
    void Scene::initialize()
    {
        stalemate = false; //початковий стан
        Turn = true; //Ходять спочатку хрестики
        for (auto* shape : shapes) delete shape;
        shapes.clear();
        button = -1;
        startedAt = std::chrono::steady_clock::now();

        float* col1D = diffLightBlue;
        float* col1A = ambiLightBlue;
        float* col1S = specLightBlue;


        // Додаємо дошку
        shapes.push_back(new Board(0.0f, -0.0345f, 0.0f, N * xStep, 0.07f, M * zStep, diffOrange, ambiOrange, specOrange));


        //Формуємо лінії
        shapes.push_back(new Board(xStep / 2, 0.04f, 0.0f, 0.05f, 0.07f, M * zStep, col1D, col1A, col1S));
        shapes.push_back(new Board(-xStep / 2, 0.04f, 0.0f, 0.05f, 0.07f, M * zStep, col1D, col1A, col1S));

        shapes.push_back(new Board(0.0f, 0.03f, zStep / 2, N * xStep, 0.07f, 0.05f, col1D, col1A, col1S));
        shapes.push_back(new Board(0.0f, 0.03f, -zStep / 2, N * xStep, 0.07f, 0.05f, col1D, col1A, col1S));




        distZ = -2;
        angleX = -10;
        angleY = 30;
        time = 0;
        finish = false;

    }

    // Пошук клітинки, найблищої до позиції курсору миші:
    bool Scene::findNearest(int x, int y, int& x1, int& z1)
    {
        int viewport[4];
        int iMin = -1, jMin = -1;
        double mvMatrix[16], projMatrix[16];
        double minDist = 2000;

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {

                // Світові x, y, z поточної клітинки:
                double wx = allocX(j);
                double wy = 0.1;
                double wz = allocZ(i);

                // Заповнюємо масив viewport поточною областю перегляду:
                glGetIntegerv(GL_VIEWPORT, viewport);

                // Заповнюємо масиви поточними матрицями:
                glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
                glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

                // Світові x, y, z координати, що обертаються:
                double dx, dy, dz;

                // Отримуємо координати точки, на яку спроектовано клітинка:
                gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
                dy = viewport[3] - dy - 1; // dy необхідно перерахувати
                double d = (x - dx) * (x - dx) + (y - dy) * (y - dy); // квадрат відстані
                if (d < minDist) // знайшли ближню клітинку
                {
                    minDist = d;
                    iMin = i;
                    jMin = j;
                }
            }
        }
        if (minDist < 1000) // знайшли найближчу клітинку
        {
            x1 = jMin;
            z1 = iMin;
            return true;
        }
        else
        {
            return false;
        }
    }

    // Оброблювач події, пов'язаної з перемалюванням вікна
    void Scene::on_paint()
    {
        char text[128]; // Масив символів,
        // Заповнення масиву символів відповідно до стану гри:
        if (finish)
        {
            std::string win = "";

            if (!stalemate) // не Нічия
            {
                if (Turn) win = player1.name; else win = player2.name;
                snprintf(text, sizeof(text), "Game over. Time: %d sec. player: %s won   F2 - Restart game   Esc - Exit", time, win.c_str());
            }
            else //Нічия
                snprintf(text, sizeof(text), "Game over. Time: %d sec.  Draw   F2 - Restart game   Esc - Exit", time);


        }
        else
        {
            snprintf(text, sizeof(text), "F2 - Restart game   Esc - Exit              Time: %d sec.", time);
        }
        // Встановлюємо область перегляду таку, щоб вона вміщувала все вікно:
        glViewport(0, 0, (GLsizei)width, (GLsizei)height);

        // Ініціалізуємо параметри матеріалів і джерела світла:
        float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // колір фонового освітлення
        float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // колір дифузного освітлення
        float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// колір дзеркального відображення
        float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };// розташування джерела світла

        // Встановлюємо параметри джерела світла:
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        // Визначаємо блакитний колір для очищення:

        if (stalemate) //Нічия
        {
            glClearColor((GLclampf)0.1, (GLclampf)0.1, (GLclampf)0.1, (GLclampf)0);
        }
        else
            if (finish) // Гру завершено і Хтось переміг
            {
                glClearColor((GLclampf)0.1, (GLclampf)0.1, (GLclampf)0.3, (GLclampf)0);
            }
            else
            {
                glClearColor((GLclampf)0.0, (GLclampf)0.0, (GLclampf)0.0, (GLclampf)0);
            }

        // Очищуємо буфери:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Для відображення тексту, краще використовувати ортографічну проекцію:
        glOrtho(0, width, 0, height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1, 1, 0); // жовтий текст
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, text, 10, 10);



        //Блок виведення імен і статистики поряд
        char Plt1[256];
        char Plt2[256];
        snprintf(Plt1, sizeof(Plt1), "%s X: %d", player1.name.c_str(), player1.victory);
        snprintf(Plt2, sizeof(Plt2), "%s O: %d", player2.name.c_str(), player2.victory);
        glColor3f(1, 0.7, 0.9);
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, Plt1, 10, height - 30);
        glColor3f(0.7, 0.5, 1);
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, Plt2, 10, height - 65);

        glPopMatrix();

        // Включаємо режим роботи з матрицею проекцій:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Задаємо усічений конус видимості в лівосторонній системі координат,
        // 60 - кут видимості в градусах по осі у,
        // width/height - кут видимості уздовж осі x,
        // 1 и 100 - відстань від спостерігача до площин відсікання по глибині:
        gluPerspective(60, width / height, 1, 100);

        // Включаємо режим роботи з видовою матрицею:
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, distZ);	// камера з початку координат зсувається на distZ,

        glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // потім обертається по осі Oy
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // потім обертається по осі Ox
        glEnable(GL_DEPTH_TEST);	// включаємо буфер глибини (для відсікання невидимих частин зображення)

        // Включаємо режим для установки освітлення:
        glEnable(GL_LIGHTING);

        // Додаємо джерело світла "0 (їх може бути до 8), зараз воно світить з "очей":
        glEnable(GL_LIGHT0);

        // Малюємо усі фігури:
        for (std::size_t i = 0; i < shapes.size(); i++)
        {
            shapes[i]->draw();
        }

        // Вимикаємо все, що включили:
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        if (page < 2) //Опрацьовувати якщо номер сторінки менше двух
        {
            //Переносимо виведення тексту поверх обєктів.
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            // Для відображення тексту, краще використовувати ортографічну проекцію:
            glOrtho(0, 1, 0, 1, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glColor3f(1, 1, 0); // жовтий текст


            if (page == 0) //Сторінка 0
            {
                char text1[128]; // Масив символів,
                // Заповнення масиву символів відповідно до стану гри:
                snprintf(text1, sizeof(text1), "Enter a name Player 1, (only EN) Next : Press enter");
                drawString(GLUT_BITMAP_TIMES_ROMAN_24, text1, 0.01, 0.8f);
                drawString(GLUT_BITMAP_TIMES_ROMAN_24, player1.name.c_str(), 0.01, 0.7f);
            }



            if (page == 1) //Сторінка 1
            {
                char text1[128]; // Масив символів,
                // Заповнення масиву символів відповідно до стану гри:
                snprintf(text1, sizeof(text1), "Enter a name Player 2, (only EN) Next : Press enter");
                drawString(GLUT_BITMAP_TIMES_ROMAN_24, text1, 0.01, 0.8f);
                drawString(GLUT_BITMAP_TIMES_ROMAN_24, player2.name.c_str(), 0.01, 0.7f);
            }
        }


        glFlush();
        // показуємо вікно:
        glutSwapBuffers(); // перемикання буферів
    }

    // Оброблювач події, пов'язаної зі зміною розмірів вікна
    void Scene::on_size(int width, int height)
    {
        this->width = width > 0 ? width : 1;
        if (height == 0)
            height = 1;
        this->height = height;
    }


    //Функція для отримання типу ігрового елемента хрестика чи нулика або їх відсутності
    int Scene::GetType(int x, int y)
    {

        float Xob = allocX(x);
        float Yob = allocZ(y);
        for (std::size_t i = 0; i < shapes.size(); i++) //Пройтись по всім обєктам
        {
            if (shapes[i]->type == 1 || shapes[i]->type == 2) //Вибрати тільки хрестики і нулики
            {

                if (shapes[i]->getXCenter() == Xob && shapes[i]->getZCenter() == Yob) //Якщо знайдено об'єкт в заданих координатах то повертаємо його тип
                {

                    return shapes[i]->type;
                }

            }
        }

        return 0;
    }

    //Перевірка умов на перемогу по типу гравця
    bool Scene::TestVictory(int t)
    {
        //Умова по горизонталі
        for (int i = 0; i < 3; i++)
            if (GetType(0, i) == t && GetType(1, i) == t && GetType(2, i) == t) return true;

        //Умова по вертикалі
        for (int i = 0; i < 3; i++)
            if (GetType(i, 0) == t && GetType(i, 1) == t && GetType(i, 2) == t) return true;

        //Умова навхрест зліва на право
        if (GetType(0, 0) == t && GetType(1, 1) == t && GetType(2, 2) == t) return true;

        //Умов навхрест з права на ліво
        if (GetType(2, 0) == t && GetType(1, 1) == t && GetType(0, 2) == t) return true;

        //Жодна умова перемоги не виконана
        return false;
    }

    //Підрахунок кількості хрестиків і нуликів.
    int Scene::CountElement()
    {
        int count = 0;

        for (std::size_t i = 0; i < shapes.size(); i++) //Пройтись по всі обєктам
        {
            if (shapes[i]->type == 1 || shapes[i]->type == 2) //Вибрати тільки хрестики і нулики
            {

                count++; //Враховуємо його
            }
        }

        return count;
    }


    // Оброблювач подій, пов'язаних з натисканням кнопок миші
    void Scene::on_mouse(int button, int state, int x, int y)
    {

        // Зберігаємо поточні координати миші:
        mouseX = x;
        mouseY = y;
        if (state == GLUT_UP) // кнопка віджата
        {
            this->button = -1;  // ніяка кнопка не натиснута
            return;
        }
        this->button = button;  // зберігаємо інформацію про кнопки
        if (finish || page < 2)
        {
            return;
        }



        int X, Y; //Змінні для отримання результатів пошуку місця натиску

        if (button == 0 && findNearest(mouseX, mouseY, X, Y))
        {

            float CorectHeight = 0.02;

            //Отримати координати місця куди потрібно вкласти новий об'єкт
            float Xob = allocX(X);
            float Yob = allocZ(Y);

            if (!(bool)GetType(X, Y)) //Якщо ми клікнули в місце де немає ще обєктів тоді будемо додавати
            {
                if (Turn)
                {
                    shapes.push_back(new Cross(Xob, 0.00 - CorectHeight, Yob, 0.3, 0.1, 0.3, diffRed, ambiRed, specRed));
                    shapes[shapes.size() - 1]->type = 1; //Задаємо тип елемента
                }
                else
                {
                    shapes.push_back(new Disk(Xob, 0.05 - CorectHeight, Yob, 0.3, 0.05, 0.3, diffGreen, ambiGreen, specGreen, 0.05));
                    shapes[shapes.size() - 1]->type = 2; //Задаємо тип елемента
                }
                Turn = !Turn; //Змінюємо чергу

            }


            if (TestVictory(1)) { player1.victory++;  Turn = true; finish = true; } //Перевірити чи перший гравець виграв
            else
                if (TestVictory(2)) { player2.victory++;  Turn = false;  finish = true; } //Перевірити чи другий гравець виграв
                else
                    if (CountElement() >= 3 * 3) { stalemate = true;  finish = true; } //Якщо усі вільні клітинки заповнено та  ніхто не переміг то нічия

        }
    }

    // Оброблювач подій, пов'язаних з пересуванням миші з натисненою кнопкою
    void Scene::on_motion(int x, int y)
    {
        switch (button)
        {

        case 2: // права кнопка - обертання сцени
            angleX += x - mouseX;
            angleY += y - mouseY;
            mouseX = x;
            mouseY = y;
            break;
        }
    }

    // Оброблювач подій, пов'язаних з натисненням функціональних клавіш і стрілок
    void Scene::on_special(int key, int, int)
    {
        switch (key) {
        case GLUT_KEY_UP:   // наближення
            if (distZ > -1.7)
            {
                break;
            }
            distZ += 0.1;
            break;
        case GLUT_KEY_DOWN: // віддалення
            distZ -= 0.1;
            break;
        case GLUT_KEY_F2:   // нова гра
            initialize();
            break;
        }
    }

    //Введення з клавіатури
    void Scene::on_keyboard(unsigned char key, int, int)
    {

        if (key == 13 && (page == 0 || page == 1))
        {

            if (player2.name.empty() && page == 1)   {
                return;
            }


            if (player1.name.empty()) {
                return;
            }

            page++;
            if (page == 2) startedAt = std::chrono::steady_clock::now();
            return;
        }


        std::string* t = &player1.name;
        if (page == 1) t = &player2.name;


        if (page == 0 || page == 1)
        {

            if (key == 8 || key == 127) //Видаляємо символ якщо натиснуто backspace
            {
                if (t->size() > 0)
                    t->erase(t->begin() + t->size() - 1);
            }
            else if (key >= 32 && key <= 126 && key != ';' && key != '=' && t->size() < 24)
                t->push_back(key);
        }
    }


    void Scene::on_timer(int)
    {
        if (page == 2 && !finish)
            time = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - startedAt).count());
        glutPostRedisplay();
    }

}
