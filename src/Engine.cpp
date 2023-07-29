//
// Created by Junius7 on 7/7/2023.
//

#include "Engine.h"
#include <fstream>
#include <iostream>
using namespace std;
const sf::Time Engine::TimePerFrame = seconds(1.f/60);

Engine::Engine()
{
    resolution = Vector2f (800, 600);
    window.create(VideoMode(resolution.x, resolution.y), "Junius7", Style::Default);
    window.setFramerateLimit(FPS);
    checkLevelFiles();
    startGame();
    mainFont.loadFromFile("../assets/fonts/slant_regular.ttf");
    setupText(&titleText, mainFont, "junius7", 28, Color::Blue);
    FloatRect titleTextBounds =  titleText.getLocalBounds();
    titleText.setPosition(Vector2f(resolution.x/2 - titleTextBounds.width/2, -9));

    setupText(&currentLevelText, mainFont, "level 1", 28, Color::Blue);
    FloatRect currentlevelTextBounds = currentLevelText.getGlobalBounds();
    setupText(&applesEatenText, mainFont, "apples 0", 28, Color::Blue);
    //std::cout<<"bound left: " + to_string(currentlevelTextBounds.left)<<endl;
    //std::cout<<"bound width: " + to_string(currentlevelTextBounds.width)<<endl;

    applesEatenText.setPosition(
            Vector2f(currentlevelTextBounds.left + currentlevelTextBounds.width + 20, -9)
    );
    setupText(&scoreText, mainFont, to_string(score), 28, Color::Blue);
    FloatRect scoreTextBounds = scoreText.getLocalBounds();
    scoreText.setPosition(Vector2f(resolution.x - scoreTextBounds.width - 15, -9));
    setupText(&gameOverText, mainFont, "GAME OVER", 72, Color::Yellow);
    FloatRect gameOverTextBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(Vector2f(resolution.x/2 - gameOverTextBounds.width/2, 100));
    gameOverText.setOutlineColor(Color::Black);
    gameOverText.setOutlineThickness(2);

    setupText(&pressEnterText, mainFont, "Press ENTER to try again", 38, Color::Green);
    FloatRect pressEnterTextBounds = pressEnterText.getLocalBounds();
    pressEnterText.setPosition(Vector2f(resolution.x/2 - pressEnterTextBounds.width/2, 200));
    pressEnterText.setOutlineColor(Color::Black);
    pressEnterText.setOutlineThickness(2);
}

void Engine::startGame() {
    score = 0;
    applesEatenThisLevel = 0;
    applesEatenTotal = 0;
    currentLevel = 0;
    loadLevel(currentLevel);
    speed = 5;
    snakeDirection = Direction::RIGHT;
    timeSinceLastMove = Time::Zero;
    sectionsToAdd = 0;
    beginNextLevel();
    currentLevelText.setPosition(Vector2f(15, -9));
    currentGameState = GameState::RUNNING;
    lastGameState = currentGameState;
    currentLevelText.setString("level " + to_string(currentLevel));
    applesEatenText.setString("apples " + to_string(applesEatenTotal));
    scoreText.setString("score " + to_string(score));
    FloatRect scoreTextBounds = scoreText.getLocalBounds();
    scoreText.setPosition(Vector2f(resolution.x - scoreTextBounds.width - 15, -9));
    //std::cout<<"bound left: " + to_string(currentlevelTextBounds.left)<<endl;
    //std::cout<<"bound width: " + to_string(currentlevelTextBounds.width)<<endl;

    FloatRect currentlevelTextBounds = currentLevelText.getGlobalBounds();
    applesEatenText.setPosition(
            Vector2f(currentlevelTextBounds.left + currentlevelTextBounds.width + 20, -9)
            );
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

void Engine::setupText(sf::Text *textItem, const sf::Font &font, const std::string &value, int size, sf::Color colour) {
    textItem->setFont(font);
    textItem->setString(value);
    textItem->setCharacterSize(size);
    textItem->setFillColor(colour);
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
        // Update score
        score += snake.size() + (applesEatenTotal + 1);
        scoreText.setString("score " + to_string(score));
        FloatRect scoreTextBounds = scoreText.getLocalBounds();
        scoreText.setPosition(Vector2f(resolution.x - 250, -9));


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
            applesEatenThisLevel ++;
            applesEatenTotal++;
            applesEatenText.setString("apples " + to_string(applesEatenTotal));
            FloatRect currentlevelTextBounds = currentLevelText.getGlobalBounds();
            applesEatenText.setPosition(
                    Vector2f(currentlevelTextBounds.left + currentlevelTextBounds.width + 20, -9)
            );

            bool beginningNewLevel = false;
            if (applesEatenThisLevel >= 10) {
                // Begin next level if there's more
                // otherwise stay on last level and keep getting harder
                beginningNewLevel = true;
                beginNextLevel();
            }
            if (!beginningNewLevel) {
                if (currentLevel < maxLevels) {
                    sectionsToAdd += 4;
                    speed++;
                    moveApple();
                }
            }

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

// Increment the level number, load the next level and reset the snake
void Engine::beginNextLevel() {
    currentLevel ++;
    wallSections.clear();
    directionQueue.clear();
    speed = 5 + currentLevel;
    sectionsToAdd = 0;
    applesEatenThisLevel = 0;

    loadLevel(currentLevel);
    newSnake();
    moveApple();
    currentLevelText.setString("level " + to_string(currentLevel));
    FloatRect currentlevelTextBounds = currentLevelText.getGlobalBounds();
    applesEatenText.setPosition(
            Vector2f(currentlevelTextBounds.left + currentlevelTextBounds.width + 20, -9)
    );
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
        newAppleLocation.y = (float) (1 + randomNumber(0, INT32_MAX) % (int)(applePerScreen.y)) * 20;
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