#ifndef TEXT_MANAGER_H_
#define TEXT_MANAGER_H_

#include "Renderer.h"
#include <vector>
#include <functional>
#include <array>
#include <SDL_ttf.h>
#include "Vector2.h"

namespace fonts
{

    struct Symbol
    {
        SDL_Texture *tex = nullptr;
        int minx, maxx, miny, maxy, advance;
        Symbol() = default;
        Symbol& operator=(Symbol &rhs) = delete;
        Symbol(Symbol &rhs) = delete;
        Symbol& operator=(Symbol &&rhs);
        Symbol(Symbol &&rhs);
        ~Symbol();
    };

    class Font
    {
    public:
        template<typename Func, typename... Args>
        Font(Func generateSymbols, Args&&... args);
        const Symbol &operator[](char ch);

    private:
        std::array<Symbol, 256> m_symbols;
    };
}

class TextManager
{
public:
    TextManager(Renderer *renderer_, const std::string &basePath_);

    template<int fontid>
    void renderText(const std::string &text, Vector2<float> pos);

private:
    static void generateOutlinedTexturedSymbols(std::array<fonts::Symbol, 256> &symbols_, Renderer &renderer_, const std::string &basePath_, const std::string &font_, const std::string &texture_, int size_, int outlineWidth_, const SDL_Color &outlineColor_);
    static void generateSimpleSymbols(std::array<fonts::Symbol, 256> &symbols_, Renderer &renderer_, const std::string &basePath_, const std::string &font_, int size_, const SDL_Color &color_);
    Renderer *m_renderer;
    std::array<fonts::Font, 3> m_fonts;

};

#endif