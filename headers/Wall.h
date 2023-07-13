//
// Created by Junius7 on 7/13/2023.
//

#ifndef JUNIUS7_WALL_H
#define JUNIUS7_WALL_H

#include <SFML/Graphics.hpp>
using namespace sf;

class Wall {
private:
    RectangleShape wallShape;

public:
    Wall(Vector2f position, Vector2f size);
    RectangleShape getShape();
};

#endif //JUNIUS7_WALL_H
