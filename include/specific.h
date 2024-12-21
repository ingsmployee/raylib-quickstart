#pragma once

#include "overall.h"
#include <deque>
#include <queue>
#include "raylib.h"
#include "raymath.h"
#include <memory>

// this is where we will put stuff specific to this game that I don't want to transfer between projects.

const int CELL_SIZE = 20;
const Vector2 GRID_DIMENSIONS = {10, 30};
Vector3 offset = {10, 10};

enum directions {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
};

class Centipede: public GameObject {
private:
    Vector2 currentDirection;
    Color colorTail = WHITE;
    Color colorBody = RAYWHITE;
    std::queue<Vector2> queuedDirections;
    int growthToGo = 0;
    

public:
    std::deque<Vector2> body; //positions
    //in public so it's easier to access by reference

    // constructor for a centipede from position and grownLength
    Centipede(Vector2& initPos, Vector2& initDirection, int growthAddAmount) {
        currentDirection = initDirection;
        body.push_front(initPos);
        growthToGo = growthAddAmount;

    }
    // constructor for a centipede from a deque and direction
    Centipede(std::deque<Vector2>& initBody, Vector2& initDirection) {
        currentDirection = initDirection;
        body = initBody;
    }


    void moveAndTurn() {
        if (queuedDirections.size() > 0) {
            currentDirection = queuedDirections.front();
            queuedDirections.pop();
        }

        body.push_front(Vector2Add(body[0], currentDirection));
        //if we have growth to do, then don't delete the back
        if (growthToGo > 0) {
            growthToGo--;
        }
        else {
            body.pop_back();
        }


    }

    void draw() {
        Vector2 bodyPart = body[body.size() - 1];
        DrawCircle(bodyPart.x * CELL_SIZE + offset.x, bodyPart.y * CELL_SIZE + offset.y, CELL_SIZE / 2.0, colorTail);
        bodyPart = body[0];
        DrawCircle(bodyPart.x * CELL_SIZE + offset.x, bodyPart.y * CELL_SIZE + offset.y, CELL_SIZE / 2.0, colorTail);

        for(int i=1; i < body.size() - 1; i++) {
            DrawRectangle(body[i].x * CELL_SIZE + offset.x, body[i].y * CELL_SIZE + offset.x, CELL_SIZE, CELL_SIZE, colorBody);
        }
    }

    void queueTurnaround() { // for when it collides with a mushroom
        queuedDirections.push({0,1});
        queuedDirections.push(Vector2Scale(currentDirection , -1));
    }

    
    Vector2 getDirection() {
        return currentDirection;
    }

};

class Mushroom: public GameObject {

};

class Bullet: public GameObject {

};

class Spider: public GameObject {

};



