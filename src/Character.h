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
#include "HealthWidget.h"
#include "NotifyWidget.h"
#include "HealthHandler.h"
#include "ComboPhysicsHandler.h"

/*
 * TODO: LIST
 * crouching and jumping attacks [DONE]
 * crouching pushbox, probably other pushboxes [DONE]
 * air attacks that alter inertia [DONE]
 * lows, overheads, unblockables [DONE]
 * hard and soft knockdown [DONE]
 * different types of landing recovery [DONE]
 * * Hard when blocks attack in the air (can only block during recovery)
 * * Vulnerable when uses air attacks (cannot block or do any attack for a period of time)
 * airdash, backdash, air backdash [DONE]
 * 
 * HUD and HUD widgets [DONE]
 * counter hit [DONE]
 * * Counter hits are defined by different launch trajectory, damage, hard knockdown, wallsplat / wallbounce / groundbounce,
 * * extended hitstop for the one who takes the hit and potentially different animation on CH (for example, if move goes into
 * * throw-like animation on CH), but the last one is not necessary
 * health [DONE]
 * damage scaling [DONE]
 * proratio [DONE]
 * 
 * gravity, impulse scaling [DONE]
 * Ground bounce [DONE, but no animation]
 * Wallbounce, wallsplat
 * 
 * step - cancel and step - cancel specific moves
 * * Kind of like dash cancel, except with completely fixed range and unique follow ups
 * throws
 * Projectiles
 * Clash, forced clash property
 * parries, guard points
 * wallbang (?)
 * * Wallbang (?): when character blocks an attack with wallbang (?) property, he gets into a wallbang blockstun.
 * * When character in wallbang blockstun touches the wall, he takes an unblockable hit with very low damage and hitstun,
 * * but 100% proratio. He can be comboed from this hit, but it should depend on m_spacing 
 * overheat gauge
 * from above / from below property (probably rename)
 * aerial hitstun scaling
 * Aerial breakaway and fatal counter
 * * This game has no airtech, but aerial breakaway is a kind of combo breaker. If the character tries to guess if the next attack
 * * will be "from above" or "from below" and guesses correctly, he enters fully invulnerable state, escapes the combo and potentially punishes it.
 * * If he guesses wrong, he takes a "fatal counter" that has different properties and resets gravity scaling
 * Blockstun scaling
 * * When character begins to block, his internal 20-frame timer should begin to go. At frame 0 he takes 2x blockstun and
 * * from frame 19 he takes 1x blockstun. There should be a widget in the HUD that displays this scaling. The scaling only works
 * * when the character in the blockstun or holds block and is able to block. For example, if you press back, block and attack,
 * * release block and press it back before blockstun (with auto-block) ends, the timer does not stop, but if you start blocking
 * * during knockdown recovery, the timer begins to move only once you are fully recovered
 * instant block, IB reversals, failed block, block counter
 * * Once you press and hold any block, you enter instant block state. IB window is 7 frames. After it, there is
 * * 3 frames of failed block state. These states are completely independent from character's actual state. For example,
 * * if you press back 2 frames before recovery from knd, you have only 5 IB frames and 3 FB frames. IB forces you to take
 * * only 1x blockstun no matter how would blockstun scaling work normally and can be cancelled into step, backdash, jumps
 * * and character-specific IB reversals
 * * FB forces increased blockstun. There is no auto-block during FB, and the first hit you take in this state causes block counter
 * * Block counter is similar to regular counter hits, but is better
 * FEAR gauge
 * * Guard gauge, scales chip damage
 * 
 * 
 * Particles
 * sounds
 * Buffs system
 * Transition animations
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
    Character(Application &application_, Vector2<float> pos_, float maxHealth_, float baseGravity_, Camera *cam_);

    void setOnStage(Application &application_, int playerId_, Character *otherCharacter_);

    virtual CharacterUpdateRes update();
    virtual void draw(Renderer &renderer_, Camera &camera_);

    Vector2<float> getPos() const;
    void setPos(Vector2<float> pos_);
    virtual Collider getPushbox() const;

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

    virtual HIT_RESULT applyHit(HitEvent &hitEvent_) = 0;
    virtual void applyHitstop(int hitstopLength);
    HitData getCurrentTakenHit();
    void takeCornerPushback(HitData fromHit_, float rangeToCorner_, const Vector2<int> dirFromCorner_);

    virtual bool canApplyDrag() const;
    virtual bool canApplyGravity() const;
    virtual bool canBeDraggedByInertia() const;
    void turnVelocityToInertia();

    virtual void generateWidgets(Application &application_, HUD &hud_);

protected:
    virtual void enterHitstunAnimation(const PostHitProperties &props_) = 0;

    Vector2<float> m_pos;
    Vector2<float> m_velocity;
    Vector2<float> m_inertia;
    float m_inertiaDrag = 1;
    bool m_airborne = false;

    bool m_inHitstop = false;
    FrameTimer m_hitstopTimer;

    Collider m_pushbox;
    Collider m_airbornePushbox;
    Collider m_crouchingPushbox;
    Character *m_otherCharacter;
    int m_playerId;

    BlockHandler m_blockHandler;

    std::map<ANIMATIONS, std::unique_ptr<Animation>> m_animations;
    Animation *m_currentAnimation;
    ORIENTATION m_dirToEnemy;
    ORIENTATION m_ownOrientation;

    HitData m_currentTakenHit;
    PostHitProperties m_hitProps;

    HealthWidget *m_healthWidget = nullptr;
    NotifyWidget *m_notifyWidget = nullptr;

    HealthHandler m_healthHandler;
    ComboPhysicsHandler m_comboPhysHandler;

    Camera *m_cam;

    HITSTUN_ANIMATION m_hitstunAnimation;

};

#endif