//
// Created by Junius7 on 7/7/2023.
//

#include "Engine.h"
#include <fstream>
using namespace std;
const sf::Time Engine::TimePerFrame = seconds(1.f/60);

Engine::Engine()
{
    resolution = Vector2f (800, 600);
    window.create(VideoMode(resolution.x, resolution.y), "Junius7", Style::Default);
    window.setFramerateLimit(FPS);
    checkLevelFiles();
    startGame();
}

void Engine::startGame() {
    currentLevel = 1;
    loadLevel(currentLevel);
    speed = 2;
    snakeDirection = Direction::RIGHT;
    timeSinceLastMove = Time::Zero;
    sectionsToAdd = 0;
    directionQueue.clear();
    newSnake();
    moveApple();
    currentGameState = GameState::RUNNING;
    lastGameState = currentGameState;
}

int Engine::randomNumber(int min, int max) {
    static std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void Engine::run() {
    Clock clock; // is outside main loop
    // Main loop - Runs until the window closes
    while (window.isOpen())
    {
        Time dt = clock.restart();

        if (currentGameState == GameState::PAUSED || currentGameState == GameState::GAMEOVER) {
            // If we are paused, then check for input (so we can un-pause) then continue to next loop
            input();

            // Draw GameOver screen
            if (currentGameState == GameState::GAMEOVER) {
                draw();
            }

            sleep (milliseconds(2)); // So we don't bug the cpu
            continue;
        }

        timeSinceLastMove += dt;
        input();
        update();
        draw();
    }
}

void Engine::togglePause() {
    if (currentGameState == GameState::RUNNING) {
        lastGameState = currentGameState;
        currentGameState = GameState::PAUSED;
    }
    else if (currentGameState == GameState::PAUSED) {
        currentGameState = lastGameState;
    }
}

void Engine::checkLevelFiles() {
    // Load levels manifest file
    ifstream levelsManifest ("../assets/levels/levels.txt");
    ifstream testFile;
    for (string manifestLine; getline(levelsManifest, manifestLine);) {
        // Check that we can open the level file
        testFile.open("../assets/levels/" + manifestLine);
        if (testFile.is_open()) {
            // the level file opens, add it to available levels
            levels.emplace_back("../assets/levels/" + manifestLine);
            testFile.close();
            maxLevels++;
        }
    }
}

void Engine::loadLevel(int levelNumber) {
    string levelFile = levels[levelNumber - 1];
    ifstream level (levelFile);
    string line;
    if (level.is_open()) {
        for (int y = 0; y < 30; y++)
        {
            getline(level, line);
            for (int x = 0; x < 40; x++) {
                if (line[x] == 'x')
                    wallSections.emplace_back(Wall(Vector2f(x * 20, y * 20), Vector2f(20, 20)));
            }
        }
    }
    level.close();
}

void Engine::update() {
    // update snakePosition
    if (timeSinceLastMove.asSeconds() >= seconds(1.f/float(speed)).asSeconds()) {
        Vector2f thisSectionPosition = snake[0].getPosition();
        Vector2f lastSectionPosition = thisSectionPosition;
        // If directionQueue isn't empty
        if (!directionQueue.empty()) {
            // Make sure we don't reverse direction
            switch (snakeDirection) {
                case Direction::UP:
                    if (directionQueue.front() != Direction::DOWN)
                        snakeDirection = directionQueue.front();
                    break;
                case Direction::DOWN:
                    if (directionQueue.front() != Direction::UP)
                        snakeDirection = directionQueue.front();
                    break;
                case Direction::LEFT:
                    if (directionQueue.front() != Direction::RIGHT)
                        snakeDirection = directionQueue.front();
                    break;
                case Direction::RIGHT:
                    if (directionQueue.front() != Direction::LEFT)
                        snakeDirection = directionQueue.front();
                    break;
            }
            directionQueue.pop_front();
        }
        // Do we need to grow the snake
        // Grows the snake one move at a time
        if (sectionsToAdd) {
            addSnakeSection();
            sectionsToAdd--;
        }

        // Update snake head and tail positions
        switch (snakeDirection) {
            case Direction::UP:
                snake[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y - 20));
                break;
            case Direction::DOWN:
                snake[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y + 20));
                break;
            case Direction::LEFT:
                snake[0].setPosition(Vector2f(thisSectionPosition.x -20, thisSectionPosition.y));
                break;
            case Direction::RIGHT:
                snake[0].setPosition(Vector2f(thisSectionPosition.x + 20, thisSectionPosition.y));
                break;
        }

        for (unsigned int s = 1; s < snake.size(); s++) {
            thisSectionPosition = snake[s].getPosition();
            snake[s].setPosition(lastSectionPosition); // Sets current position as the position of the former block once was
            lastSectionPosition = thisSectionPosition; // Updates the "last block position" to the current block being changed, then moves onto the next block
        }
        // Run snake section update functions (updates the actual rectangle being drawn's position)
        for (auto &s: snake) {
            s.update();
        }

        // Collision detection -- apple
        if (snake[0].getShape().getGlobalBounds().intersects(apple.getSprite().getGlobalBounds())) {
            // We hit the apple. Increase snake length, increase speed, and move the apple
            // TODO: increment score, apples eaten, and check if its time for the next level
            sectionsToAdd += 4;
            speed++;
            moveApple();
        }

        // Collision detection -- wall
        for (int w = 0; w < wallSections.size(); w++) {
            if (wallSections[w].getShape().getGlobalBounds().intersects(snake[0].getShape().getGlobalBounds()))
                currentGameState = GameState::GAMEOVER;
        }

        // Collision detection - snake body
        for (int s = 1; s < snake.size(); s++) {
            if (snake[0].getShape().getGlobalBounds().intersects(snake[s].getShape().getGlobalBounds()))
                currentGameState = GameState::GAMEOVER;
        }

        timeSinceLastMove = Time::Zero;
    } // End update snake positions
}

void Engine::newSnake() {
    snake.clear();
    snake.emplace_back(Vector2f(100, 100));
    snake.emplace_back(Vector2f(80, 100));
    snake.emplace_back(Vector2f(60, 100));
}

void Engine::addSnakeSection() {
    // Add section at the same position as the last section of the snake.
    Vector2f newSectionPosition = snake[snake.size() - 1].getPosition();
    snake.emplace_back(newSectionPosition);
}

void Engine::moveApple() {
    // Find a location to place the apple
    // Must not be a location the snake is in or a wall

    // Divide field into sections the size of the apple, remove 2 to exclude exterior wall --> how many apples there can be on an axis
    Vector2f applePerScreen = Vector2f (resolution.x/20 - 2, resolution.y / 20 -2);
    Vector2f newAppleLocation;
    bool badLocation = false;
    srand(time(nullptr));
    // Loop until we find a valid location
    do {
        badLocation = false;
        // Generate random location

        // Random number % (screnresolution.x / 20) [how many blocks of 20 there are horizontally on screen] = generate a number from 0 to that block count randomly. Then multiply by 20
//        newAppleLocation.x = (float) (1 + rand()/((RAND_MAX + 1u)/(int)appleResolution.x)) * 20;

        newAppleLocation.x = (float) (1 + randomNumber(0, INT32_MAX) % (int)(applePerScreen.x)) * 20;
        newAppleLocation.y = (float) (1 + rand()/((RAND_MAX + 1u)/(int)applePerScreen.y)) * 20;
        // Check if it is in the snake
        for (auto &s: snake) {
            if (s.getShape().getGlobalBounds().intersects(Rect<float>(newAppleLocation.x, newAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }

        // Check if it is in the walls
        for (auto &w: wallSections) {
            if (w.getShape().getGlobalBounds().intersects(Rect<float>(newAppleLocation.x, newAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }
    }
    while (badLocation);

    apple.setPosition(newAppleLocation);
}