#include "Hit.h"

Hit::Hit(float damage_, const std::vector<Hitbox> &hitboxes_) :
    damage(damage_),
    hitboxes(hitboxes_)
{
        
}

std::vector<Hitbox> Hit::getHitboxes() const
{
    return hitboxes;
}