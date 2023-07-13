//
// Created by Junius7 on 7/12/2023.
//

#include "apple.h"

apple::apple() {
    Vector2f startingPosition(400, 300);
    sprite.setSize(Vector2f(20, 20));
    sprite.setFillColor(Color::Red);
    sprite.setPosition(startingPosition);
};

void apple::setPosition(sf::Vector2f newPosition) {
    sprite.setPosition(newPosition);
}

RectangleShape apple::getSprite() {
    return sprite;
}