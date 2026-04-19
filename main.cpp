#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdexcept>
using namespace std;

// ================= CONSTANTS =================
const int HEIGHT = 15;
const int WIDTH = 40;

// ================= BASE CLASS =================
class Character {
protected:
    int x, y;
    char symbol;

public:
    Character(int x, int y, char s) : x(x), y(y), symbol(s) {}
    virtual void move(char) = 0;

    int getX() { return x; }
    int getY() { return y; }
    char getSymbol() { return symbol; }
};

// ================= PLAYER =================
class Player : public Character {
public:
    Player() : Character(1, 1, '*') {}

    void move(char key) override {
        int newX = x, newY = y;

        if (key == 'w') newY--;
        else if (key == 's') newY++;
        else if (key == 'a') newX--;
        else if (key == 'd') newX++;

        if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT)
            return;

        x = newX;
        y = newY;
    }

    void reset() {
        x = 1;
        y = 1;
    }
};

// ================= ENEMY =================
class Enemy : public Character {
private:
    int path[12][2] = {
        {10,3},{11,3},{12,3},{13,3},{14,3},{15,3},
        {16,3},{17,3},{18,3},{19,3},{20,3},{21,3}
    };
    int index;

public:
    Enemy() : Character(10, 3, 'G'), index(0) {}

    void move(char) override {
        x = path[index][0];
        y = path[index][1];
        index = (index + 1) % 12;
    }

    void reset() {
        index = 0;
        x = path[0][0];
        y = path[0][1];
    }
};

// ================= GAME =================
class Game {
private:
    char map[HEIGHT][WIDTH + 1] = {
        "########################################",
        "#   #####          #####        #####  #",
        "#   #   #          #   #              #",
        "#   #   #==========#   #==============#",
        "#   #####                          ####",
        "#                                  ####",
        "#   ////                             ##",
        "#   ////        #####                ##",
        "#   ////        #   #   Room3        ##",
        "#   ////        #####                ##",
        "#                                  ###X",
        "#                                      ",
        "#                                      ",
        "#                                      ",
        "########################################"
    };

    Player player;
    Enemy granny;

public:
    void showControls() {
        system("cls");
        cout << "===== HAUNTED ESCAPE =====\n\n";
        cout << "Controls:\n";
        cout << "W - Up\nA - Left\nS - Down\nD - Right\n\n";
        cout << "P - Play\nQ - Quit\n";

        while (true) {
            char ch = _getch();
            if (ch == 'p' || ch == 'P') break;
            if (ch == 'q' || ch == 'Q') exit(0);
        }
    }

    void draw() {
        system("cls");
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == player.getY() && j == player.getX())
                    cout << player.getSymbol();
                else if (i == granny.getY() && j == granny.getX())
                    cout << granny.getSymbol();
                else
                    cout << map[i][j];
            }
            cout << endl;
        }
    }

    void input() {
        if (_kbhit()) {
            char key = _getch();
            player.move(key);
        }
    }

    void logic() {
        granny.move('x');

        if (player.getX() == granny.getX() &&
            player.getY() == granny.getY()) {
            throw runtime_error("GRANNY");
        }

        if (map[player.getY()][player.getX()] == 'X') {
            throw runtime_error("WIN");
        }
    }

    bool endScreen(string msg) {
        system("cls");
        cout << "\n\n" << msg << "\n\n";
        cout << "P - Play Again\nQ - Quit\n";

        while (true) {
            char ch = _getch();
            if (ch == 'p' || ch == 'P') return true;
            if (ch == 'q' || ch == 'Q') return false;
        }
    }

    void resetGame() {
        player.reset();
        granny.reset();
    }

    void run() {
        while (true) {
            showControls();
            resetGame();

            try {
                while (true) {
                    draw();
                    input();
                    logic();
                    Sleep(120);
                }
            }
            catch (runtime_error e) {
                string msg =
                    (string(e.what()) == "GRANNY")
                    ? "GRANNY CAUGHT YOU!\nGAME OVER"
                    : "YOU ESCAPED!\nYOU WIN";

                if (!endScreen(msg)) break;
            }
        }
    }
};

// ================= MAIN =================
int main() {
    Game game;
    game.run();
    return 0;
}  
