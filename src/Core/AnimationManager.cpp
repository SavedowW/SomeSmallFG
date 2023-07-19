#include "Application.h"
#include "AnimationManager.h"

AnimationManager::AnimationManager(Renderer* renderer_, const std::string &rootPath_) :
	m_renderer(renderer_),
	m_rootPath(rootPath_ + "/")
{
	//Init preloaded textures arr
	for (int i = 0; i < (int)ANIMATIONS::NONE; ++i)
	{
		m_preloadedArrs[i] = nullptr;
	}

	//Load preloading textures
	//preload(ANIMATIONS::hit1);

	//Init pointers to texture arrays
	for (int i = 0; i < int(ANIMATIONS::NONE); ++i)
	{
		m_textureArrPointers[ANIMATIONS(i)] = std::weak_ptr<TextureArr>();
	}
}

std::shared_ptr<TextureArr> AnimationManager::getTextureArr(ANIMATIONS tex_)
{
	if (m_preloadedArrs[(int)tex_] != nullptr)
	{
		//Logger::print("Texture arr is preloaded\n");
		return m_preloadedArrs[(int)tex_];
	}
	else if (m_textureArrPointers[tex_].expired())
	{
		//Logger::print("Texture arr does not exist, creating new\n");

		//Parse data.txt
		std::ifstream loadData(m_rootPath + m_filePaths[(int)tex_] + "animinfo.txt");
		std::string prefix;
		int ibegin, iend;
		loadData >> prefix >> ibegin >> iend;
		int amount = iend - ibegin + 1;

		//Load all textures
		SDL_Texture** texs = new SDL_Texture * [amount];
		for (int i = ibegin; i <= iend; ++i)
		{
			texs[i - ibegin] = m_renderer->loadTexture((m_rootPath + m_filePaths[(int)tex_] + prefix + std::to_string(i) + ".png").c_str());
		}

		int totalDuration = 0;
		loadData >> totalDuration;
		std::vector<int> framesData;
		for (int i = 0; i < totalDuration; ++i)
		{
			int temp;
			loadData >> temp;
			framesData.push_back(temp);
		}

		//Create TextureArray with loaded textures
		std::shared_ptr<TextureArr> reqElem(new TextureArr(texs, amount, totalDuration, framesData));
		m_textureArrPointers[tex_] = reqElem;
		return reqElem;
	}
	else
	{
		//Logger::print("Texture arr already exist\n");
		return m_textureArrPointers[tex_].lock();
	}
}

void AnimationManager::preload(ANIMATIONS t_animToPreload)
{
	m_preloadedArrs[(int)t_animToPreload] = std::shared_ptr<TextureArr>(getTextureArr(t_animToPreload));
	m_textureArrPointers[t_animToPreload] = m_preloadedArrs[(int)t_animToPreload];
}

//Properly release all textures
TextureArr::~TextureArr()
{
	//Logger::print("Release " + intToString(amount) + " textures\n");
	for (int i = 0; i < amount; ++i)
		SDL_DestroyTexture(tex[i]);
	delete[] tex;
}

Animation::Animation(AnimationManager &animationManager_, ANIMATIONS textures_, LOOPMETHOD isLoop_, int beginFrame_, int beginDirection_) :
	m_isLoop(isLoop_),
	m_currentFrame(beginFrame_),
	m_direction(beginDirection_)
{
	m_textures = animationManager_.getTextureArr(textures_);
}


void Animation::update()
{
	if (isFinished())
		animFinished();
	else
		m_currentFrame += m_direction;;
}

SDL_Texture* Animation::getSprite()
{
	return (*m_textures)[m_currentFrame];
}

bool Animation::isFinished()
{
	if (m_direction > 0)
		return m_currentFrame == m_textures->totalDuration - 1;
	else
		return !m_currentFrame;
}

void Animation::switchDir()
{
	m_direction *= -1;
}

void Animation::setDir(int dir_)
{
	assert(dir_ == 1 || dir_ == -1);

	m_direction = dir_;
}

void Animation::reset(int beginFrame_, int beginDirection_)
{
	m_currentFrame = beginFrame_;
	m_direction = beginDirection_;
}

void Animation::animFinished()
{
	switch (m_isLoop)
	{
	case (LOOPMETHOD::NOLOOP):
			m_direction = 0;
		break;

	case (LOOPMETHOD::JUMP_LOOP):
		if (m_currentFrame >= m_textures->totalDuration - 1)
			m_currentFrame = 0;
		else if (m_currentFrame <= 0)
			m_currentFrame = m_textures->totalDuration - 1;
		break;

	case (LOOPMETHOD::SWITCH_DIR_LOOP):
		m_direction *= -1;
		if (m_direction == -1)
			m_currentFrame = m_textures->totalDuration - 1;
		else
			m_currentFrame = 0;
		break;
	}
}

Vector2<float> Animation::getSize()
{
	return {(float)m_textures->w, (float)m_textures->h};
}

int Animation::getDirection() const
{
	return m_direction;
}