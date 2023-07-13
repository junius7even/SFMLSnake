//
// Created by Junius7 on 7/12/2023.
//

#ifndef JUNIUS7_APPLE_H
#define JUNIUS7_APPLE_H

#include <SFML/Graphics.hpp>
using namespace sf;

class apple {
private:
    RectangleShape sprite;

public:
    apple();
    void setPosition(Vector2f newPosition);
    RectangleShape getSprite();
};


#endif //JUNIUS7_APPLE_H
