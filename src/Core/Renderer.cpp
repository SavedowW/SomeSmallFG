#include "Renderer.h"
#include <stdexcept>

Renderer::Renderer(Window &window_)
{
    auto win = window_.getWindow();
    if (!win)
        throw std::runtime_error("Trying to create renderer without window");

    m_renderer = SDL_CreateRenderer(window_.getWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (m_renderer == nullptr) {
		std::cout << "Renderer creation error: " << SDL_GetError() << std::endl;
		throw std::runtime_error("Cannot create renderer");
	}

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_renderer);
}

Renderer& Renderer::operator=(Renderer &&rhs)
{
    m_renderer = rhs.m_renderer;

    rhs.m_renderer = nullptr;

    return *this;
}

Renderer::Renderer(Renderer &&rhs)
{
    m_renderer = rhs.m_renderer;

    rhs.m_renderer = nullptr;
}

SDL_Texture* Renderer::createTexture(int w_, int h_)
{
	return SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w_, h_);
}

SDL_Texture* Renderer::createTexture(const Vector2<int>& size_)
{
	return SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size_.x, size_.y);
}

SDL_Texture* Renderer::loadTexture(const char* file_)
{
	SDL_Texture* texture = IMG_LoadTexture(m_renderer, file_);
	if (!texture) {
		std::cout << "Texture loading problem: " << file_ << "  |  " << IMG_GetError() << std::endl;
	}
	return texture;
}

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* sur_)
{
	return SDL_CreateTextureFromSurface(m_renderer, sur_);
}

void Renderer::renderTexture(SDL_Texture* tex_, float x_, float y_, float w_, float h_, float angle_, SDL_FPoint* center_, SDL_RendererFlip flip_)
{
	SDL_FRect dst;

	dst.x = x_;
	dst.y = y_;
	dst.w = w_;
	dst.h = h_;

	SDL_RenderCopyExF(m_renderer, tex_, NULL, &dst, angle_, center_, flip_);
}

void Renderer::renderTexture(SDL_Texture* tex_, float x_, float y_, const Camera &cam_, SDL_RendererFlip flip_)
{
    //throw std::runtime_error("You shouldn't use this renderTexture yet");
	int tw, th;
	SDL_QueryTexture(tex_, NULL, NULL, &tw, &th);

	auto camSize = cam_.getSize();

	auto texTL = ((Vector2{x_, y_} - cam_.getTopLeft()) / cam_.getScale());
	auto texBR = ((Vector2{x_ + tw, y_ + th} - cam_.getTopLeft()) / cam_.getScale());
	auto size = texBR - texTL;
	
	renderTexture(tex_, texTL.x, texTL.y, size.x, size.y, 0, nullptr, flip_);
}

void Renderer::renderTexture(SDL_Texture* tex_, float x_, float y_, float angle_, SDL_FPoint* center_, SDL_RendererFlip flip_)
{
	int w, h;
	SDL_QueryTexture(tex_, NULL, NULL, &w, &h);

	renderTexture(tex_, x_, y_, w, h, angle_, center_, flip_);
}

void Renderer::renderTexture(SDL_Texture* tex_, float x_, float y_, float w_, float h_)
{
	SDL_FRect dst;

	dst.x = x_;
	dst.y = y_;
	dst.w = w_;
	dst.h = h_;

	SDL_RenderCopyF(m_renderer, tex_, NULL, &dst);
}

void Renderer::renderTexture(SDL_Texture* tex_, float x_, float y_)
{
	int w, h;
	SDL_QueryTexture(tex_, NULL, NULL, &w, &h);
	renderTexture(tex_, x_, y_, w, h);
}

void Renderer::drawRectangle(const Vector2<float> &pos_, const Vector2<float> &size_, const SDL_Color& col_, const Camera &cam_)
{
	auto rectTL = ((pos_ - cam_.getTopLeft()) / cam_.getScale());
	auto rectSize = size_ / cam_.getScale();
	SDL_SetRenderDrawColor(m_renderer, col_.r, col_.g, col_.b, col_.a);
	SDL_FRect rect = { rectTL.x, rectTL.y, rectSize.x, rectSize.y };
	SDL_RenderDrawRectF(m_renderer, &rect);
}

void Renderer::fillRectangle(const Vector2<float> &pos_, const Vector2<float> &size_, const SDL_Color& col_, const Camera &cam_)
{
	auto rectTL = ((pos_ - cam_.getTopLeft()) / cam_.getScale());
	auto rectSize = size_ / cam_.getScale();
	SDL_SetRenderDrawColor(m_renderer, col_.r, col_.g, col_.b, col_.a);
	SDL_FRect rect = { rectTL.x, rectTL.y, rectSize.x, rectSize.y };
	SDL_RenderFillRectF(m_renderer, &rect);
}

void Renderer::fillRenderer(const SDL_Color& col_)
{
	SDL_SetRenderDrawColor(m_renderer, col_.r, col_.g, col_.b, col_.a);
	SDL_RenderClear(m_renderer);
}

void Renderer::updateScreen()
{
	SDL_RenderPresent(m_renderer);
}

void Renderer::setRenderTarget(SDL_Texture* tex_)
{
	int i = SDL_SetRenderTarget(m_renderer, tex_);
	if (i != 0)
		std::cout << i << std::endl;
}