#include "Application.h"
#include "AnimationManager.h"
#include <cassert>

AnimationManager::AnimationManager(Renderer* renderer_, const std::string &rootPath_) :
	m_renderer(renderer_),
	m_rootPath(rootPath_ + "/")
{
	auto sprPath = rootPath_ + "/Resources/Sprites/";
	std::filesystem::path basePath(sprPath);
	/*for (const auto &entry : std::filesystem::recursive_directory_iterator(sprPath))
	{
		std::filesystem::path dirpath = entry.path();
		std::filesystem::path filepath(entry.path().string() + "/animinfo.txt");
		if (entry.is_directory() && std::filesystem::exists(filepath))
		{
			ContainedAnimationData cad;
			cad.m_path = dirpath.string();
			m_textureArrs.push_back(cad);
			auto idstring = utils::getRelativePath(sprPath, dirpath.string());
			m_ids[idstring] = m_textureArrs.size() - 1;
		}
	}*/

	std::cout << "=== LISTING FOUND ANIMATIONS ===\n";
	for (const auto &entry : std::filesystem::recursive_directory_iterator(sprPath))
	{
		std::filesystem::path dirpath = entry.path();
		if (entry.is_regular_file() && dirpath.extension() == ".panm")
		{
			auto path = utils::getRelativePath(sprPath, dirpath.string());
			std::cout << utils::removeExtention(path) << std::endl;

			ContainedAnimationData cad;
			cad.m_path = dirpath.string();

			m_textureArrs.push_back(cad);
			m_ids[utils::removeExtention(path)] = m_textureArrs.size() - 1;
		}
	}
	std::cout << "=== LISTING ENDS HERE ===\n";

	//Load preloading textures
	// TODO: preload particles and effects depending on characters and stage
	preload("Particles/Block");
	preload("Particles/Hit1");
	preload("Particles/Hit2");
	preload("Particles/Hit2_Slowed");
	preload("Particles/Clash");
}

std::shared_ptr<TextureArr> AnimationManager::getTextureArr(int id_)
{
	if (m_textureArrs[id_].m_preloaded)
	{
		//Logger::print("Texture arr is preloaded\n");
		return m_textureArrs[id_].m_preloaded;
	}
	else if (m_textureArrs[id_].m_texArr.expired())
	{
		//Logger::print("Texture arr does not exist, creating new\n");

		EngineAnimation anim;
		anim.loadAnimation(m_textureArrs[id_].m_path, *m_renderer);
		SDL_Texture **texs = new SDL_Texture*[anim.m_frameCount];
		SDL_Texture **whiteTexs = new SDL_Texture*[anim.m_frameCount];
		for (int i = 0; i < anim.m_frameCount; ++i)
		{
			texs[i] = m_renderer->createTextureFromSurface(anim.m_surfaces[i]);
			whiteTexs[i] = m_renderer->createTextureFromSurface(anim.m_whiteSurfaces[i]);
		}

		std::vector<int> framesData;
		for (int i = 0; i < anim.m_duration; ++i)
		{
			framesData.push_back(anim.m_framesData[i]);
		}

		//Create TextureArray with loaded textures
		std::shared_ptr<TextureArr> reqElem(new TextureArr(texs, whiteTexs, anim.m_frameCount, anim.m_duration, framesData, anim.m_width, anim.m_height, anim.m_origin));
		m_textureArrs[id_].m_texArr = reqElem;
		return reqElem;
	}
	else
	{
		//Logger::print("Texture arr already exist\n");
		return m_textureArrs[id_].m_texArr.lock();
	}
}

void AnimationManager::preload(int toPreload_)
{
	if (m_textureArrs[toPreload_].m_preloaded == nullptr)
	{
		m_textureArrs[toPreload_].m_preloaded = std::shared_ptr<TextureArr>(getTextureArr(toPreload_));
	}
}

void AnimationManager::preload(const std::string &toPreload_)
{
	int id = getAnimID(toPreload_);

	preload(id);
}

int AnimationManager::getAnimID(const std::string &animName_) const
{
	try
	{
    	return m_ids.at(animName_);
	}
	catch (std::out_of_range exc_)
	{
		throw std::string("Failed to find animation ") + animName_ + " : " + exc_.what();
	}
}

//Properly release all textures
TextureArr::~TextureArr()
{
	//Logger::print("Release " + intToString(amount) + " textures\n");
	for (int i = 0; i < amount; ++i)
		SDL_DestroyTexture(tex[i]);
	delete[] tex;
}

Animation::Animation(AnimationManager &animationManager_, int id_, LOOPMETHOD isLoop_, int beginFrame_, int beginDirection_) :
	m_isLoop(isLoop_),
	m_currentFrame(beginFrame_),
	m_direction(beginDirection_)
{
	m_textures = animationManager_.getTextureArr(id_);
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
	if (m_currentFrame == -1)
		m_currentFrame = 0;
		
	return (*m_textures)[m_currentFrame];
}

SDL_Texture* Animation::getWhiteSprite()
{
	if (m_currentFrame == -1)
		m_currentFrame = 0;
		
	return m_textures->getWhite(m_currentFrame);
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

Vector2<float> Animation::getOrigin()
{
    return m_textures->m_origin;
}

int Animation::getDirection() const
{
	return m_direction;
}

Animation::Animation(Animation &&anim_)
{
	m_textures = std::move(anim_.m_textures);
	m_currentFrame = anim_.m_currentFrame;
	m_direction = anim_.m_direction;
	m_isLoop = anim_.m_isLoop;
}

Animation &Animation::operator=(Animation &&anim_)
{
	m_textures = std::move(anim_.m_textures);
	m_currentFrame = anim_.m_currentFrame;
	m_direction = anim_.m_direction;
	m_isLoop = anim_.m_isLoop;

	return *this;
}