#ifndef ANIMATION_MANAGER_H_
#define ANIMATION_MANAGER_H_
#include "TextureManager.h"
#include <fstream>

enum class ANIMATIONS
{
	PARTICLE_HIT1,
	PARTICLE_HIT2,
	PARTICLE_HIT2_SLOWED,
	PARTICLE_BLOCK,
	CHAR1_IDLE,
	CHAR1_CROUCH_IDLE,
	CHAR1_WALK_FWD,
	CHAR1_WALK_BWD,
	CHAR1_PREJUMP,
	CHAR1_JUMP,
	CHAR1_LANDING_RECOVERY,
	CHAR1_GROUND_DASH,
	CHAR1_GROUND_DASH_RECOVERY,
	CHAR1_STEP,
	CHAR1_STEP_RECOVERY,
	CHAR1_BACKDASH,
	CHAR1_AIRDASH,
	CHAR1_AIR_BACKDASH,
	CHAR1_MOVE_A,
	CHAR1_MOVE_B,
	CHAR1_MOVE_C,
	CHAR1_MOVE_2B,
	CHAR1_MOVE_JA,
	CHAR1_MOVE_JC,
	CHAR1_MOVE_214C,
	CHAR1_HITSTUN_LOW,
	CHAR1_HITSTUN_MID,
	CHAR1_HITSTUN_HIGH,
	CHAR1_HITSTUN_AIR,
	CHAR1_HITSTUN_CROUCH,
	CHAR1_JC_LANDING_RECOVERY,
	CHAR1_BLOCKSTUN_STANDING,
	CHAR1_BLOCKSTUN_CROUCHING,
	CHAR1_KNOCKDOWN,
	CHAR1_SOFT_KNOCKDOWN,
	CHAR1_KNOCKDOWN_RECOVERY,
	NONE
};

//Texture array structure
struct TextureArr
{
	//Makes sure textures actually exist
	TextureArr(SDL_Texture** tex_, int amount_, int totalDuration_, const std::vector<int> &framesData_) :
		tex(tex_), amount(amount_), totalDuration(totalDuration_), framesData(framesData_)
	{
		//assert(t_tex);

		//Sizes are based on first sprite
		SDL_QueryTexture(tex[0], NULL, NULL, &w, &h);
	}

	SDL_Texture* operator[](const int rhs)
	{
		return tex[framesData[rhs]];
	}

	//Texture array and required info
	SDL_Texture** tex;
	int amount;
	int w, h;
	int totalDuration;
	std::vector<int> framesData;

	//Properly removes texture
	virtual ~TextureArr();
};

/**************************
*
*	Animation manager holds all texture arrays
*	and makes sure there is only one version of each
*	texture array exist and only if required
*
**************************/
class AnimationManager
{
public:
	AnimationManager(Renderer* renderer_, const std::string &rootPath_);
	std::shared_ptr<TextureArr> getTextureArr(ANIMATIONS tex_);

private:
	std::string m_rootPath;
	Renderer* m_renderer;
	const std::string m_filePaths[(int)ANIMATIONS::NONE] = {
		"Resources/Sprites/Particles/Hit1/",
		"Resources/Sprites/Particles/Hit2/",
		"Resources/Sprites/Particles/Hit2_Slowed/",
		"Resources/Sprites/Particles/Block/",
		"Resources/Sprites/Char1/Idle/",
		"Resources/Sprites/Char1/CrouchIdle/",
		"Resources/Sprites/Char1/WalkForward/",
		"Resources/Sprites/Char1/WalkForward/",
		"Resources/Sprites/Char1/Prejump/",
		"Resources/Sprites/Char1/Jump/",
		"Resources/Sprites/Char1/LandingRecovery/",
		"Resources/Sprites/Char1/GroundDash/",
		"Resources/Sprites/Char1/GroundDashRecovery/",
		"Resources/Sprites/Char1/Step/",
		"Resources/Sprites/Char1/StepRecovery/",
		"Resources/Sprites/Char1/Backdash/",
		"Resources/Sprites/Char1/Airdash/",
		"Resources/Sprites/Char1/AirBackdash/",
		"Resources/Sprites/Char1/Jab/",
		"Resources/Sprites/Char1/MoveB/",
		"Resources/Sprites/Char1/MoveC/",
		"Resources/Sprites/Char1/Move2B/",
		"Resources/Sprites/Char1/MoveJA/",
		"Resources/Sprites/Char1/MoveJC/",
		"Resources/Sprites/Char1/Move214C/",
		"Resources/Sprites/Char1/LowHitstun/",
		"Resources/Sprites/Char1/MidHitstun/",
		"Resources/Sprites/Char1/HighHitstun/",
		"Resources/Sprites/Char1/AirHitstun/",
		"Resources/Sprites/Char1/CrouchHitstun/",
		"Resources/Sprites/Char1/JCLandingRecovery/",
		"Resources/Sprites/Char1/StandingBlock/",
		"Resources/Sprites/Char1/CrouchBlock/",
		"Resources/Sprites/Char1/Knockdown/",
		"Resources/Sprites/Char1/SoftKnockdown/",
		"Resources/Sprites/Char1/KnockdownRecovery/"
	};
	std::map<ANIMATIONS, std::weak_ptr<TextureArr>> m_textureArrPointers;

	void preload(ANIMATIONS t_animToPreload);
	std::shared_ptr<TextureArr> m_preloadedArrs[(int)ANIMATIONS::NONE];
};

enum class LOOPMETHOD
{
	NOLOOP,
	JUMP_LOOP,
	SWITCH_DIR_LOOP
};

/**************************
*
*	Animation class
*	Call update each frame
*	Direction = 1 - direct order
*	Direction = -1 - reverse order
*
**************************/
class Animation
{
public:
	Animation(AnimationManager &animationManager_, ANIMATIONS textures_, LOOPMETHOD isLoop_ = LOOPMETHOD::JUMP_LOOP, int beginFrame_ = -1, int beginDirection_ = 1);
	void update();
	SDL_Texture* getSprite();
	bool isFinished();
	void switchDir();
	void setDir(int dir_);
	Vector2<float> getSize();
	void reset(int beginFrame_ = -1, int beginDirection_ = 1);
	int getDirection() const;

private:
	std::shared_ptr<TextureArr> m_textures;
	int m_currentFrame;
	int m_direction;
	LOOPMETHOD m_isLoop;
	void animFinished();

};

using Animation_t = std::shared_ptr<Animation>;

#endif