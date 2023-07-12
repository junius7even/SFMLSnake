//
// Created by Junius7 on 7/7/2023.
//

#include "Engine.h"
const sf::Time Engine::TimePerFrame = seconds(1.f/60);

Engine::Engine()
{
    resolution = Vector2f (800, 600);
    window.create(VideoMode(resolution.x, resolution.y), "Junius7", Style::Default);
    window.setFramerateLimit(FPS);

    speed = 2;
    snakeDirection = Direction::RIGHT;

    timeSinceLastMove = Time::Zero;

    newSnake();
}

void Engine::run() {
    Clock clock; // is outside main loop
    // Main loop - Runs until the window closes
    while (window.isOpen())
    {
        Time dt = clock.restart();
        timeSinceLastMove += dt;
        input();
        update();
        draw();
    }
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