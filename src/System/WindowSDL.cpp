// Copyright (C) 2022 Arno Ansems
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
// 
// You should have received a copy of the GNU General Public License 
// along with this program.  If not, see http://www.gnu.org/licenses/ 

#include "WindowSDL.h"
#include "RendererOpenGL.h"
#include "../Engine/Logging.h"
#include "../Engine/EngineCore.h"

GLvoid ReSizeGLScene(IRenderer* renderer, GLsizei width, GLsizei height)        // Resize And Initialize The GL Window
{
    if (height==0)                                      // Prevent A Divide By Zero By
    {
        height=1;                                       // Making Height Equal One
    }

    renderer->SetWindowDimensions(width, height);
}

void CreateGLWindow(int width, int height, int /*bits*/, SDL_Window*& window, SDL_GLContext& context )
{
    Logging::Instance().AddLogMessage("Initializing OpenGL renderer");

    if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1) != 0)
    {
        Logging::Instance().AddLogMessage("WARNING: call to SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,1) failed: " + std::string(SDL_GetError()));
    }

    const std::string windowTitle = "CatacombGL " + EngineCore::GetVersionInfo();
    window = SDL_CreateWindow(
        windowTitle.c_str(),                        // window title
        SDL_WINDOWPOS_UNDEFINED,                    // initial x position
        SDL_WINDOWPOS_UNDEFINED,                    // initial y position
        width,                                      // width, in pixels
        height,                                     // height, in pixels
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE    // flags - see below
    );

    // Check that the window was successfully created
    if (window == nullptr)
    {
        // In the case that the window could not be made...
        Logging::Instance().FatalError("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }

    SDL_ShowWindow(window);

    context = SDL_GL_CreateContext(window);
    if (context == nullptr)
    {
        Logging::Instance().FatalError("SDL_GL_CreateContext failed: " + std::string(SDL_GetError()));
    }
}

GLvoid KillGLWindow(SDL_Window* window, SDL_GLContext& context)
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

[[nodiscard]] bool HandleWindowEvent(const SDL_WindowEvent * event, SDL_Window* window, IRenderer* renderer)
{
    switch (event->event) {
    case SDL_WINDOWEVENT_RESIZED:
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
            int w = 0;
            int h = 0;
            SDL_GetWindowSize(window, &w, &h);
            ReSizeGLScene(renderer, w, h);
            break;
        }
    case SDL_WINDOWEVENT_CLOSE:
        return false;
    }

    return true;
}

void SetScreenMode(const uint8_t screenMode, SDL_Window* window)
{
    if (screenMode == CVarItemIdScreenModeFullscreen)
    {
        // Set the width and height of the fullscreen to the width and height of the desktop area of the
        // display in use.
        const int displayIndex = SDL_GetWindowDisplayIndex(window);
        SDL_Rect r;
        SDL_GetDisplayBounds(displayIndex, &r);
        const SDL_DisplayMode displayMode = { SDL_PIXELFORMAT_RGB24, r.w, r.h, 0, 0 };
        SDL_SetWindowDisplayMode(window, &displayMode);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    else if (screenMode == CVarItemIdScreenModeBorderlessWindowed)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else // CVarItemIdScreenModeWindowed
    {
        SDL_SetWindowFullscreen(window, 0);
    }
}

