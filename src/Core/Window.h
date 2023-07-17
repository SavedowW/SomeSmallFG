#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL.h>
#include <SDL_Image.h>
#include <string>
#include "Vector2.h"

class Window
{
public:
    Window(Vector2<int> resolution_, const std::string &winName_);
    ~Window();

    Window& operator=(const Window &rhs) = delete;
    Window(const Window &rhs) = delete;

    Window& operator=(Window &&rhs);
    Window(Window &&rhs);

    SDL_Window* getWindow();

    Vector2<int> getCurrentResulution() const;

private:
    SDL_Window* m_window = nullptr;
    Vector2<int> m_currentResolution;
    std::string m_winName;

};

#endif