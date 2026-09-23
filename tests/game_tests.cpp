#include "Scene.h"
#include "Score.h"
#include "Board.h"
#include "GLPlatform.h"
#include "utils.h"
#include <stdexcept>
#include <iostream>

void check(bool condition) {
    if (!condition) throw std::runtime_error("Regression check failed");
}
namespace CrossGame {
struct SceneTests {
    static void graphics() {
        Scene scene(.4f, .6f);
        scene.on_keyboard('A', 0, 0); scene.on_keyboard(13, 0, 0);
        scene.on_keyboard('B', 0, 0); scene.on_keyboard(13, 0, 0);
        auto click = [&](int x, int z) {
            scene.on_paint();
            double model[16], projection[16], sx, sy, sz;
            int viewport[4];
            glGetDoublev(GL_MODELVIEW_MATRIX, model);
            glGetDoublev(GL_PROJECTION_MATRIX, projection);
            glGetIntegerv(GL_VIEWPORT, viewport);
            gluProject(scene.allocX(x), .1, scene.allocZ(z), model, projection, viewport, &sx, &sy, &sz);
            scene.on_mouse(GLUT_LEFT_BUTTON, GLUT_DOWN, static_cast<int>(sx),
                viewport[3] - static_cast<int>(sy) - 1);
            scene.on_mouse(GLUT_LEFT_BUTTON, GLUT_UP, 0, 0);
        };
        click(0,0); click(0,0);
        check(scene.CountElement() == 1 && !scene.Turn);
        click(0,1); click(1,0); click(1,1); click(2,0);
        check(scene.finish && scene.player1.victory == 1 && !scene.stalemate);
        click(2,2); check(scene.CountElement() == 5);
        scene.on_special(GLUT_KEY_F2, 0, 0);
        const int draw[9][2] = {{0,0},{1,0},{2,0},{1,1},{0,1},{2,1},{1,2},{0,2},{2,2}};
        for (const auto& cell : draw) click(cell[0], cell[1]);
        check(scene.finish && scene.stalemate && scene.player1.victory == 1);
        scene.on_paint();
        check(glGetError() == GL_NO_ERROR);
    }
    static void run() {
        Scene scene(0.4f, 0.6f);
        check(scene.page == 0 && scene.button == -1 && scene.time == 0);
        scene.on_keyboard(13, 0, 0);
        check(scene.page == 0);
        for (int i = 0; i < 200; ++i) scene.on_keyboard('A', 0, 0);
        check(scene.player1.name.size() == 24);
        scene.on_keyboard(127, 0, 0);
        scene.on_keyboard(';', 0, 0);
        scene.on_keyboard('=', 0, 0);
        scene.on_keyboard(1, 0, 0);
        check(scene.player1.name.size() == 23);
        scene.on_mouse(GLUT_LEFT_BUTTON, GLUT_DOWN, 0, 0);
        check(scene.CountElement() == 0);
        scene.on_keyboard(13, 0, 0);
        scene.on_keyboard(13, 0, 0);
        check(scene.page == 1);
        scene.on_keyboard('B', 0, 0);
        scene.on_keyboard(13, 0, 0);
        check(scene.page == 2);
        const int lines[8][6] = {
            {0,0,1,0,2,0}, {0,1,1,1,2,1}, {0,2,1,2,2,2},
            {0,0,0,1,0,2}, {1,0,1,1,1,2}, {2,0,2,1,2,2},
            {0,0,1,1,2,2}, {2,0,1,1,0,2}};
        for (int type : {1, 2}) for (const auto& line : lines) {
            scene.initialize();
            for (int i = 0; i < 3; ++i) {
                auto* piece = new Cross(scene.allocX(line[i*2]), 0, scene.allocZ(line[i*2+1]),
                    .3f, .1f, .3f, GraphUtils::diffRed, GraphUtils::ambiRed, GraphUtils::specRed);
                piece->type = type;
                scene.shapes.push_back(piece);
                check(scene.GetType(line[i*2], line[i*2+1]) == type);
                check(scene.TestVictory(type) == (i == 2));
            }
            check(!scene.TestVictory(3-type));
        }
        scene.player1.victory = 3;
        for (int i = 0; i < 100; ++i) scene.initialize();
        check(scene.shapes.size() == 5 && scene.CountElement() == 0);
        check(scene.player1.victory == 3 && scene.page == 2 && scene.Turn);
        scene.on_size(0, 0);
        check(scene.width == 1 && scene.height == 1);
        Board board(0,0,0,1,1,1,GraphUtils::diffRed,GraphUtils::ambiRed,GraphUtils::specRed);
        check(board.getDiffColor() == GraphUtils::diffRed);
        check(board.getAmbiColor() == GraphUtils::ambiRed);
    }
};
}
int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "--graphics") {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
        glutInitWindowSize(800, 600);
        int window = glutCreateWindow("Tic-tac-toe regression tests");
        CrossGame::SceneTests::graphics();
        glutDestroyWindow(window);
    }
    CrossGame::SceneTests::run();
    std::istringstream input("\nbad;record\n999999999999999999999999999;date;A=1;B=2\n7;23.09.2026;A=1;B=0\n2;23.09.2026;A=0;B=1\n-1;date;A=1;B=2\n");
    std::ostringstream output;
    check(CrossGame::readScores(input, output) == 8);
    check(output.str().find("bad") == std::string::npos);
    std::cout << "All regression checks passed\n";
}
