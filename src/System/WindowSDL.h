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

#include "../Engine/ConfigurationSettings.h"
#include "../Engine/IRenderer.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <SDL.h>

void CreateGLWindow(int width, int height, int /*bits*/, SDL_Window*& window, SDL_GLContext& context );
GLvoid KillGLWindow(SDL_Window* window, SDL_GLContext& context);

// Resize And Initialize The GL Window
GLvoid ReSizeGLScene(IRenderer* renderer, GLsizei width, GLsizei height);
void SetScreenMode(const uint8_t screenMode, SDL_Window* window);
[[nodiscard]] bool HandleWindowEvent(const SDL_WindowEvent * event, SDL_Window* window, IRenderer* renderer);

