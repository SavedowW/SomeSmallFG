#ifndef CHARACTER_H_
#define CHARACTER_H_
#define DEBUG_

#include <queue>
#include "Level.h"
#include "Application.h"
#include "AnimationManager.h"
#include "ActionResolver.h"
#include "FrameTimer.h"
#include "BlockHandler.h"

/*
 * TODO: LIST
 * crouching and jumping attacks [DONE]
 * crouching pushbox, probably other pushboxes
 * air attacks that alter inertia
 * lows, overheads, unblockables [DONE]
 * hard and soft knockdown [1/2 DONE, soft knd needs its own animation]
 * different types of landing recovery
 * * Hard when blocks attack in the air (can only block during recovery)
 * * Vulnerable when uses air attacks (cannot block or do any attack for a period of time)
 * airdash, backdash, air backdash [DONE]
 * 
 * HUD and HUD widgets
 * counter hit (with CH hitstun, hitstop and impulse)
 * health
 * blockstun scaling
 * ground recovery
 * gravity scaling
 * damage scaling
 * proratio
 * 
 * throws
 * Projectiles
 * Clash, forced clash property
 * parries, guard points
 * Ground bounce
 * Wallbounce, wallsplat, wallbang (?)
 * * Wallbang (?): when character blocks an attack with wallbang (?) property, he gets into a wallbang blockstun.
 * * When character in wallbang blockstun touches the wall, he takes an unblockable hit with very low damage and hitstun,
 * * but 100% proratio. He can be comboed from this hit, but it should depend on spacing 
 * from above / from below property (probably rename)
 * overheat gauge
 * aerial hitstun scaling
 * Aerial breakaway and fatal counter
 * * This game has no airtech, but aerial breakaway is a kind of combo breaker. If the character tries to guess if the next attack
 * * will be "from above" or "from below" and guesses correctly, he enters fully invulnerable state, escapes the combo and potentially punishes it.
 * * If he guesses wrong, he takes a "fatal counter" that has different properties and resets gravity scaling
 * 
 * Particles
 * sounds
 * Buffs system
 * 
 * Other characters and stages
 * Character / stage select screen
 * Input settings
 * Resolution settings
 * more oomph overall
*/

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
    virtual void updateBlockState() = 0;
    virtual bool isInHitstun() const = 0;

    virtual std::string CharStateData() const = 0;

    virtual void updateOwnOrientation();
    virtual void updatePosition();

    virtual HIT_RESULT applyHit(const HitEvent &hitEvent_, HIT_RESULT hitRes_ = HIT_RESULT::NONE) = 0;
    virtual void applyHitstop(int hitstopLength);
    HitData getCurrentTakenHit();
    void takeCornerPushback(HitData fromHit_, float rangeToCorner_, const Vector2<int> dirFromCorner_);

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
    Collider m_airbornePushbox;
    Character *m_otherCharacter;
    int m_playerId;

    BlockHandler m_blockHandler;

    std::map<ANIMATIONS, std::unique_ptr<Animation>> m_animations;
    Animation *m_currentAnimation;
    ORIENTATION m_dirToEnemy;
    ORIENTATION m_ownOrientation;

    HitData m_currentTakenHit;

};

#endif