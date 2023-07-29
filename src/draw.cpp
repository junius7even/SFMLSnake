//
// Created by Junius7 on 7/10/2023.
//
#include "Engine.h"
void Engine::draw()
{
    // Clears the window to a certain color
    window.clear(Color::Black);

    // Draw apple
    window.draw(apple.getSprite());

    // Draw walls
    for (auto & w : wallSections) {
        window.draw(w.getShape());
    }

    // Draw Text
    window.draw(titleText);
    window.draw(currentLevelText);
    window.draw(applesEatenText);
    window.draw(scoreText);

    // Gameover text
    if (currentGameState == GameState::GAMEOVER) {
        window.draw(gameOverText);
        window.draw(pressEnterText);
    }

    // Draw snake sections
    for (auto & s: snake) {
        window.draw(s.getShape());
    }
    window.display();
}