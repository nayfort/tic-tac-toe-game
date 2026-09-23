// main.cpp
#define _CRT_SECURE_NO_WARNINGS

#include "GLPlatform.h"
#include <iostream>
#include "Scene.h"
#include <string>
#include <fstream>
#include <sstream>
#include "Score.h"
#include <ctime>
#include <cstdlib>
#include <memory>
#include <stdio.h>

using namespace std;
using CrossGame::Scene;

std::unique_ptr<Scene> scene; // вказівник на клас Scene


std::uint64_t num = 1;
void setScore();

void on_paint()
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_paint();
}

void on_size(int width, int height)
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_size(width, height);
}

void on_mouse(int button, int state, int x, int y)
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_mouse(button, state, x, y);
}

void on_motion(int x, int y)
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_motion(x, y);
}

void on_special(int key, int x, int y)
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_special(key, x, y);
}


void on_keyboard(unsigned char key, int x, int y)
{
    // обробка подій від клавіатури:
    if (key == 27) {
        exit(0);
    }

    scene->on_keyboard(key, x, y);
}

void on_timer(int value)
{
    // обробка подій від таймера
    scene->on_timer(value);
    glutTimerFunc(25, on_timer, 0); // зa 25 мс викличеться ця функція
}


void getScore() {

    ifstream file("scoring.txt");
    num = CrossGame::readScores(file, cout);

}


string getDateStr()
{
    time_t curtime;
    struct tm* loctime;
    char buffer[12];

    time(&curtime);
    loctime = localtime(&curtime);

    if (!loctime) return "unknown";
    strftime(buffer, 12, "%d.%m.%Y", loctime);
    return buffer;
}


void setScore()
{
    if (!scene || scene->page < 2) return;
    string dateStr = getDateStr();

    ofstream of("scoring.txt", ios::app);// запис рядку у кінець файлу

    if (of.is_open())
    {
        of << num << ";" << dateStr << ";" << scene->player1.name << "=" << scene->player1.victory << ";"
                        << scene->player2.name << "=" << scene->player2.victory << endl;
    }

    of.flush();
    if (!of) cerr << "Could not save scores to scoring.txt\n";
}


int main(int argc, char* argv[])
{

    glutInit(&argc, argv);         // ініціалізуємо GLUT
    scene = std::make_unique<Scene>(0.4f, 0.4f);
    std::atexit(setScore);   // створюємо об'єкт "сцена"
    glutInitWindowSize(800, 600);  // встановлюємо розміри вікна
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);// ініціалізуємо режими відображення
    glutCreateWindow("Tic-tac-toe");     // створюємо вікно
    glutDisplayFunc(on_paint);     // реєструємо функцію відображення
    glutReshapeFunc(on_size);      // реєструємо функцію обробки зміни розмірів вікна
    glutMotionFunc(on_motion);     // реєструємо функцію, яка відповідає за переміщення миші з натиснутою кнопкою
    glutMouseFunc(on_mouse);       // реєструємо функцію, яка відповідає за натискання на кнопку миші
    glutKeyboardFunc(on_keyboard); // реєструємо функцію, яка відповідає за натискання клавіш
    glutSpecialFunc(on_special);   // реєструємо функцію, яка відповідає за натискання спеціальних клавіш
    glutTimerFunc(25, on_timer, 0);// кожні 25 мс викликається ця функція
    getScore();
    glutMainLoop();                // стартуємо основний цикл обробки подій
    setScore();
    scene.reset();                  // видаляємо об'єкт "сцена"
    return(0);
}
