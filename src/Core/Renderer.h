#ifndef RENDERER_H_
#define RENDERER_H_

#include "Window.h"
#include "Camera.h"

class Renderer
{
public:
    Renderer(Window &window_);
    ~Renderer();

    Renderer& operator=(const Renderer &rhs) = delete;
    Renderer(const Renderer &rhs) = delete;

    Renderer& operator=(Renderer &&rhs);
    Renderer(Renderer &&rhs);

    //Texture creation
	SDL_Texture* createTexture(int w_, int h_);
	SDL_Texture* createTexture(const Vector2<int> &size_);
	SDL_Texture* loadTexture(const char* file_);
	SDL_Texture* createTextureFromSurface(SDL_Surface* sur_);

	//Texture render
	void renderTexture(SDL_Texture* tex_, float x_, float y_, float w_, float h_);
    void renderTexture(SDL_Texture* tex_, float x_, float y_, const Camera &cam_, SDL_RendererFlip flip_ = SDL_FLIP_NONE);
	void renderTexture(SDL_Texture* tex_, float x_, float y_);
	void renderTexture(SDL_Texture* tex_, float x_, float y_, float w_, float h_, float angle_, SDL_FPoint* center_, SDL_RendererFlip flip_);
	void renderTexture(SDL_Texture* tex_, float x_, float y_, float w_, float h_, const Camera &cam_, float angle_, SDL_RendererFlip flip_);
	void renderTexture(SDL_Texture* tex_, const SDL_FRect *src, const SDL_FRect *dst, float angle_, SDL_FPoint* center_, SDL_RendererFlip flip_);
	void renderTexture(SDL_Texture* tex_, float x_, float y_, float angle_, SDL_FPoint* center_, SDL_RendererFlip flip_);

	//Primitives
	void drawRectangle(const Vector2<float> &pos_, const Vector2<float> &size_, const SDL_Color& col_, const Camera &cam_);
	void drawRectangle(const Vector2<float> &pos_, const Vector2<float> &size_, const SDL_Color& col_);
	void fillRectangle(const Vector2<float> &pos_, const Vector2<float> &size_, const SDL_Color& col_, const Camera &cam_);
	void fillRectangle(const Vector2<float> &pos_, const Vector2<float> &size_, const SDL_Color& col_);

	//Global render
	void fillRenderer(const SDL_Color &col_);
	void updateScreen();

	//Render target settings
	void setRenderTarget(SDL_Texture* tex_ = NULL);

	SDL_Renderer *getRenderer();

//private:
    SDL_Renderer* m_renderer = nullptr;


};

#endif