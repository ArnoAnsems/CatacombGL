// Copyright (C) 2021 Arno Ansems
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
#pragma once

#include "GuiElementBase.h"
#include "RenderableText.h"

class GuiElementEditText : public GuiElementBase
{
public:
    GuiElementEditText(
        const PlayerInput& playerInput,
        std::string& outputText,
        const std::string& initialText,
        const uint16_t maxTextLength,
        RenderableText& renderableText,
        GuiEvent& textCompleteAction);
    ~GuiElementEditText() override;

    virtual const GuiEvent& ProcessInput() override;
    virtual void Draw(IRenderer& renderer) const override;

private:
    static bool KeyIsSuitableForText(const SDL_Keycode keyCode);

protected:
    std::string& m_outputText;
    const std::string m_initialText;
    RenderableText& m_renderableText;
    const GuiEvent m_textCompleteAction;
    const GuiEvent m_busyAction = { GuiActionBusy, 0 };
    bool m_enteringText;
    const uint16_t m_maxTextLength;
};




