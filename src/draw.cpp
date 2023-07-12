//
// Created by Junius7 on 7/10/2023.
//
#include "Engine.h"
void Engine::draw()
{
    // Clears the window to a certain color
    window.clear(Color::Black);
    // Draw snake sections
    for (auto & s: snake) {
        window.draw(s.getShape());
    }
    window.display();
}