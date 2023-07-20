#ifndef CHARACTER_H_
#define CHARACTER_H_
#define DEBUG_

#include <queue>
#include "Level.h"
#include "Application.h"
#include "AnimationManager.h"
#include "ActionResolver.h"
#include "FrameTimer.h"

struct CharacterUpdateRes
{
    Vector2<float> moveOffset;
    Vector2<float> newPos;
    Collider pushbox;
};

template <typename CharState_t>
struct CharData
{
    Vector2<float> pos;
    Vector2<float> velocity;
    Vector2<float> inertia;
    bool airborne = false;
    ORIENTATION dirToEnemy;
    ORIENTATION ownDirection;
    Vector2<float> dirToEnemyVec;
    Vector2<float> ownDirectionVec;
    CharState_t state;
};

// TODO: messy interface, should move something to protected or private
class Character
{
public:
    Character(Application &application_, Vector2<float> pos_);

    void setOnStage(Application &application_, int playerId_, Character *otherCharacter_);

    virtual CharacterUpdateRes update();
    virtual void draw(Renderer &renderer_, Camera &camera_);

    Vector2<float> getPos() const;
    void setPos(Vector2<float> pos_);
    Collider getPushbox() const;

    Vector2<float> getVelocity() const;
    Vector2<float> getInertia() const;
    Vector2<float> getFullVelocity() const;

    Vector2<float> getHorDirToEnemy() const;
    Vector2<float> getOwnHorDir() const;

    virtual ~Character() {};

    virtual void loadAnimations(Application &application_) = 0;
    virtual void proceedCurrentState() = 0;
    virtual void updateState() = 0;
    virtual void initiate() = 0;
    virtual void land() = 0;
    virtual HitsVec getHits(bool allHits_ = false) = 0;
    virtual HurtboxVec getHurtboxes() = 0;

    virtual void updateOwnOrientation();
    virtual void updatePosition();

    virtual void applyHit(const HitEvent &hitEvent) = 0;
    virtual void applyHitstop(int hitstopLength);

    virtual bool canApplyDrag() const;
    virtual bool canApplyGravity() const;
    virtual bool canBeDraggedByInertia() const;
    void turnVelocityToInertia();

protected:
    Vector2<float> m_pos;
    Vector2<float> m_velocity;
    Vector2<float> m_inertia;
    float m_inertiaDrag = 1;
    float m_gravity = 1.5;
    bool m_airborne = false;

    bool m_inHitstop = false;
    FrameTimer m_hitstopTimer;

    Collider m_pushbox;
    Character *m_otherCharacter;
    int m_playerId;

    std::map<ANIMATIONS, std::unique_ptr<Animation>> m_animations;
    Animation *m_currentAnimation;
    ORIENTATION m_dirToEnemy;
    ORIENTATION m_ownOrientation;

};

#endif