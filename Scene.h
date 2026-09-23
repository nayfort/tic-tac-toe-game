// Scene.h
#ifndef Scene_h
#define Scene_h

#include "Shape.h"
#include "Disk.h"
#include "Cross.h"
#include <vector>
#include <chrono>
#include<string>

namespace CrossGame
{

    const int M = 3, N = 3; // кількість рядків та колонок поля

    // Основний клас гри, який представляє геометрію сцени,
    // контролює правила гри, розташування елементів,
    // а також реалізує обробку подій GLUT
    class Scene
    {
        friend struct SceneTests;
        std::vector<Shape*> shapes; // "гнучкий" масив указівників на елементи гри
        int button = -1;           // кнопка миші (-1 - не натиснута, 0 - ліва, 2 - права)
        float angleX, angleY; // поточний кут повороту сцени
        float mouseX = 0, mouseY = 0; // поточні координати
        float width = 800, height = 600;  // Розміри вікна
        float distZ;          // відстань по осі Z до сцени
        bool finish;          // ознака того, що гру завершено
        float xStep, zStep;   // відстань між ячейками
        std::chrono::steady_clock::time_point startedAt;
        int time;             // поточний час у секуднах

    public:
        bool Turn;             //Черга хто ходить
        int page = 0;              //На якій ми сторінці змінна для розподілу етапу гри
        bool stalemate;        //нічия

        struct Player
        {
            std::string name;
            int victory = 0;
        }player1, player2;      //Створюємо двох гравців

    public:
        Scene(float xStep, float zStep);
        ~Scene();
        void on_paint();
        void on_size(int width, int height);
        void on_mouse(int button, int state, int x, int y);
        void on_motion(int x, int y);
        void on_special(int key, int x, int y);
        void on_keyboard(unsigned char key, int x, int y);
        void on_timer(int value);
    private:
        void initialize();
        bool findNearest(int x, int z, int& x1, int& z1);
        int CountElement();
        float allocX(int i);
        float allocZ(int i);
        int GetType(int x, int y);
        bool TestVictory(int t);
    };

}

#endif
