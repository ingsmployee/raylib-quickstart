#pragma once

#include "objects_and_level.h"
#include <deque>


class Level1 : public Level {
private:

    int Vec2FindInDeque(std::deque<Vector2> d, Vector2 v) {
        for(int i=1; i<d.size(); i++) {
            if (Vector2Equals(d.at(i), v)) {
                return i;
            }
        }
        clog(D_MINIMAL, "Searching for non-existing deque element!");
        return -1;
    }

public:

    std::shared_ptr<Centipede> centipedeSplit(Centipede* og, Vector2 bodyPartPos) {
        std::deque<Vector2>* body_a = &(og->body);
        int index = Vec2FindInDeque(*body_a, bodyPartPos);

        std::shared_ptr<Centipede> clone = addObject<Centipede> (*body_a, og->getDirection());
        clone->body.erase(clone->body.begin() + index, clone->body.end());

        body_a->erase(body_a->begin(), body_a->begin() + index); // probably shit im ngl

        return clone;
    }
};