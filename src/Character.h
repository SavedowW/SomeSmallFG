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
#include "ResourceCounter.h"
#include "InteractableStateMachine.h"
#include "ProjectileFactory.h"

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
 * Wallbounce, wallsplat [1/2 DONE]
 * * Wallsplat should be possible only at the real wall, wallbounce - on both real wall and character's max range boundary
 * * In case of real wall, wall splat should have priority
 * 
 * step - cancel and step - cancel specific moves [DONE]
 * * Kind of like dash cancel, except with completely fixed range and unique follow ups
 * throws [DONE]
 * Priority [DONE]
 * * Character who lands a hit or a throw gets a priority.
 * * Priority defines who's drawn on top of another, who's updates are calculated first and who techs in same frame situation
 * Projectiles
 * Clash, forced clash property [DONE]
 * parries, guard points
 * wallbang (?)
 * * Wallbang (?): when character blocks an attack with wallbang (?) property, he gets into a wallbang blockstun.
 * * When character in wallbang blockstun touches the wall, he takes an unblockable hit with very low damage and hitstun,
 * * but 100% proratio. He can be comboed from this hit, but it should depend on spacing 
 * overheat gauge, burnout state
 * from above / from below property (probably rename)
 * aerial hitstun scaling
 * Aerial breakaway and fatal counter
 * * This game has no airtech, but aerial breakaway is a kind of combo breaker. If the character tries to guess if the next attack
 * * will be "from above" or "from below" and guesses correctly, he enters fully invulnerable state, escapes the combo and potentially punishes it.
 * * If he guesses wrong, he takes a "fatal counter" that has different properties and resets gravity scaling
 * Blockstun scaling [DONE]
 * * When character begins to block, his internal 20-frame timer should begin to go. At frame 0 he takes 2x blockstun and
 * * from frame 19 he takes 1x blockstun. There should be a widget in the HUD that displays this scaling. The scaling only works
 * * when the character in the blockstun or holds block and is able to block. For example, if you press back, block and attack,
 * * release block and press it back before blockstun (with auto-block) ends, the timer does not stop, but if you start blocking
 * * during knockdown recovery, the timer begins to move only once you are fully recovered
 * instant block, IB reversals, failed block, block counter [DONE IB, IB cancels except IB reversals]
 * * Once you press and hold any block, you enter instant block state. IB window is 7 frames. After it, there is
 * * 3 frames of failed block state. These states are completely independent from character's actual state. For example,
 * * if you press back 2 frames before recovery from knd, you have only 5 IB frames and 3 FB frames. IB forces you to take
 * * only 1x blockstun no matter how would blockstun scaling work normally and can be cancelled into step, jumps, airdash
 * * and character-specific IB reversals
 * * FB forces increased blockstun. There is no auto-block during FB, and the first hit you take in this state causes block counter
 * * Block counter is similar to regular counter hits, but is better
 * FEAR gauge
 * * Guard gauge, scales chip damage
 * 
 * 
 * Particles [DONE]
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


/*
 *
 * Animations for throws:
 * 
 * for attacker:
 * startup (actual startup frames and a number of frames while holding an opponent for a throw tech opportunity)
 * Throw animation
 * Throw whiff animation
 * 
 * for defender:
 * startup (a number of frames while being held for a throw tech opportunity)
 * Throw animation
 * 
 * 
 * States for throw:
 * 
 * for attacker:
 * throw startup
 * throw hold
 * throw whiff
 * throw tech
 * actual throw
 * 
 * for defender:
 * throw hold
 * throw tech
 * throw animation
 * state after release
 * 
*/

struct CharacterUpdateRes
{
    Vector2<float> moveOffset;
    Vector2<float> newPos;
    Collider pushbox;
};

struct GenericCharacterData
{
    GenericCharacterData(int statecnt_);
    int m_idle;
    int m_float;
    StateMarker m_dontConvertVelocityToInertia;
    StateMarker m_prejums;
    StateMarker m_noDrag;
    StateMarker m_noInertia;

    std::map<int, int> m_hitstunAnimToStates;
    int m_crouchHitstunAnim = (int)HITSTUN_ANIMATION::CROUCH;
    int m_groundBounceHitstun;

    int m_blockstunStanding;
    int m_blockstunCrouching;
    int m_blockstunAir;

    int m_hardKD;
    int m_softKD;
    int m_vulnerableLandingRecovery;
    int m_softLandingRecovery;
    int m_step;
    int m_airdash;

    std::map<int, int> m_thrownStates;
    std::map<int, int> m_throwTechedStates;

    Collider m_standingPushbox;
    Collider m_crouchingPushbox;
    Collider m_airPushbox;
    Collider m_airHitstunPushbox;

    std::set<int> m_clashCancelOptions;

    StateMarker m_useDirToEnemyForInputs;
};

// TODO: messy interface, should move something to protected or private
class Character : public InteractableStateMachine
{
public:
    Character(Application &application_, Vector2<float> pos_, float maxHealth_, float baseGravity_, Camera *cam_, ParticleManager *particleManager_, int maxAirdashes_, int maxDJumps_, int framesBeforeAirdash_, int framesBeforeAirjump_,
    StateMarker autoRealignAfter_, int stateCnt_);

    void setOnStage(Application &application_, int playerId_, Character *otherCharacter_, PriorityHandler *priorityHandler_);

    virtual CharacterUpdateRes update() override;
    virtual void drawGroundProjection(Renderer &renderer_, Camera &camera_, float angle_) override;
    virtual void draw(Renderer &renderer_, Camera &camera_) override;

    Collider getPushbox() const;

    virtual ~Character() {};

    virtual void proceedCurrentState() override;
    virtual void initiate() override;
    virtual void land();
    virtual HitsVec getHits() override;
    virtual void updateBlockState();
    virtual bool isInHitstun() const;
    virtual bool isInBlockstun() const;
    virtual bool isInInstantBlockstun() const;
    virtual bool isKnockedDown() const;
    bool passableThrough() const;

    virtual bool canBeThrown(THROW_LIST throw_) const;
    void setThrowInvul();
    virtual void attemptThrow();

    virtual float touchedWall(int sideDir_) override;

    virtual std::string CharStateData() const = 0;

    virtual void updatePosition() override;

    virtual HIT_RESULT applyHit(HitEvent &hitEvent_) override;

    void takePushback(const Vector2<float> pushback_);
    virtual void applyClash(const Hit &clashedHit_, int opponentsHitId_);

    virtual bool canApplyDrag() const override;
    virtual bool canApplyGravity() const;
    virtual bool canBeDraggedByInertia() const override;
    virtual void turnVelocityToInertia(float horMultiplier_ = 0.9f) override;

    virtual void generateWidgets(Application &application_, HUD &hud_);

    virtual ORIENTATION getInputDir() const override;

    void queueProjectile(std::unique_ptr<Projectile> &&pt_);
    std::vector<std::unique_ptr<Projectile>> &getQueuedProjectiles();

protected:
    void enterHitstunAnimation(const PostHitProperties &props_);
    void startShine(const SDL_Color &col_, int lockedDuration_, int alphaDuration_);

    virtual void jumpUsingAction();

    void lockInAnimation();
    void releaseFromAnimation();
    void tieAnimWithOpponent();
    void untieAnimWithOpponent();
    virtual Collider getUntiedPushbox() const;
    virtual void enterThrown(THROW_LIST throw_);
    virtual void throwTeched(THROW_TECHS_LIST tech_);

    Vector2<float> m_pushback;
    float m_pushbackMaxCarry = 12.0f;
    float m_pushbackCurrentCarry = 12.0f;
    float m_pushbackDeterioration = 2.0f;
    float m_pushbackTreshold = 35.0f;

    BlockHandler m_blockHandler;
    BLOCK_FRAME m_blockstunType = BLOCK_FRAME::NONE;

    HitData m_currentTakenHit;
    PostHitProperties m_hitProps;
    FrameTimer m_throwInvulTimer;

    HealthWidget *m_healthWidget = nullptr;
    NotifyWidget *m_notifyWidget = nullptr;

    HealthHandler m_healthHandler;
    ComboPhysicsHandler m_comboPhysHandler;

    Camera *m_cam = nullptr;

    HITSTUN_ANIMATION m_hitstunAnimation;

    SDL_Color m_colorShine;
    FrameTimer m_shineLockedTimer;
    FrameTimer m_shineAlphaTimer;

    bool m_lockedInAnimation = false;
    bool m_tiedAnimWithOpponent = false;

    bool m_throwPossible = false;

    ResourceCounter<int> m_jumpsAvailable;
    ResourceCounter<int> m_airdashesAvailable;
    FrameTimer m_airdashTimer;
    FrameTimer m_airjumpTimer;
    const int m_framesBeforeAirdash;
    const int m_framesBeforeAirjump;

    bool m_usedAirAttack = false;

    GenericCharacterData m_genericCharacterData;

    StateMarker m_autoRealignAfter;

    std::vector<std::unique_ptr<Projectile>> m_queuedProjectiles;

    ProjectileFactory m_ptFactory;

    friend Action_throw_startup;
    friend Action_throw_tech;
    friend Action_throw_hold;
    friend Action_thrown_hold;
    friend Action_throw_whiff;
    friend Action_locked_animation;
    friend ActionCharacter;
    friend Action_throw_startup;
    friend Action_throw_hold;
    friend Action_thrown_hold;
    friend Action_throw_whiff;
    friend Action_throw_tech;
    friend Action_locked_animation;
    friend Action_jump;
    friend Action_prolonged;
    friend Action_attack;
    friend Action_float;
    friend Action_airjump;
    friend Action_char1_move_projectile;
};

#endif