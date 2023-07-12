//
// Created by Junius7 on 7/11/2023.
//

#include "SnakeSection.h"

SnakeSection::SnakeSection(Vector2f startPosition) {
    section.setSize(Vector2f(20, 20));
    section.setFillColor(Color::Green);
    section.setPosition(startPosition);
}

void SnakeSection::setPosition(Vector2f newPosition) {
    position = newPosition;
}

Vector2f SnakeSection::getPosition() {
    return position;
}

RectangleShape SnakeSection::getShape() {
    return section;
}

void SnakeSection::update() {
    section.setPosition(position);
}