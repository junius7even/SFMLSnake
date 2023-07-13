//
// Created by Junius7 on 7/13/2023.
//
#include "Wall.h"

Wall::Wall(sf::Vector2f position, sf::Vector2f size) {
    wallShape.setSize(size);
    wallShape.setFillColor(Color::White);
    wallShape.setPosition(position);

}

RectangleShape Wall::getShape() {
    return wallShape;
}