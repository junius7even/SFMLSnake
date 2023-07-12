//
// Created by Junius7 on 7/11/2023.
//

#ifndef JUNIUS7_SNAKESECTION_H
#define JUNIUS7_SNAKESECTION_H

#include "SFML/Graphics.hpp"

using namespace sf;

class SnakeSection {
private:
    Vector2f position;
    RectangleShape section;

public:
    explicit SnakeSection(Vector2f startPosition);
    Vector2f getPosition();
    void setPosition(Vector2f newPosition);
    void update();
    RectangleShape getShape();
};


#endif //JUNIUS7_SNAKESECTION_H
