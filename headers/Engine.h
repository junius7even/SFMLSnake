//
// Created by Junius7 on 7/7/2023.
//

#ifndef GRAPHICSPROJECTS_ENGINE_H
#define GRAPHICSPROJECTS_ENGINE_H
#include <SFML/Graphics.hpp>
#include <random>
#include "SnakeSection.h"
#include <vector>
#include <deque>
#include "apple.h"
#include "Wall.h"

using namespace std;
using namespace sf;

class Engine {
private:
    // Window
    Vector2f resolution;
    RenderWindow window;
    const unsigned int FPS = 60;
    static const Time TimePerFrame;
    vector<SnakeSection> snake;

    int snakeDirection;
    deque<int> directionQueue;
    int speed;
    int sectionsToAdd;

    apple apple;
    void moveApple();
    void checkLevelFiles();
    int randomNumber(int min, int max);
    Time timeSinceLastMove;

    int currentGameState;
    int lastGameState;

    vector <Wall> wallSections;
    int currentLevel;
    int maxLevels;
    vector<string> levels;

public:
    enum Direction {UP, RIGHT, DOWN, LEFT};
    enum GameState {RUNNING, PAUSED, GAMEOVER};
    Engine();
    void addDirection(int newDirection);
    void update();
    void input();
    void draw();
    void newSnake();
    void addSnakeSection();
    // Main loop will be in the run function
    void run();
    void togglePause();
    void loadLevel(int levelNumber);
    void startGame();
};


#endif //GRAPHICSPROJECTS_ENGINE_H
