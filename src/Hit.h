#ifndef HIT_H_
#define HIT_H_

#include "Vector2.h"
#include <array>
#include <utility>
#include <vector>

using FrameWindow = std::pair<int, int>;

struct Hit
{
    Hit(float damage_, const std::vector<Hitbox> &hitboxes_);

    std::vector<Hitbox> getHitboxes() const;

    float damage;
    std::vector<Hitbox> hitboxes;
};

//using ActiveFramesVec = std::vector<std::pair<FrameWindow, Hit>>;
using ActiveFramesVec = std::vector<std::pair<FrameWindow, Hit>>;
using HitsVec = std::vector<Hit>;

// TODO: hurtboxes
using HurtboxFramesVec = std::vector<std::pair<FrameWindow, Hitbox>>;
using HurtboxVec = std::vector<Hitbox>;

#endif