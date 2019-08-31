// Copyright (C) 2019 Arno Ansems
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

#include "GameCatacomb3D.h"
#include "EgaGraphCatacomb3D.h"
#include "GameMapsCatacomb3D.h"
#include "IntroViewCatacomb3D.h"
#include "AudioRepositoryCatacomb3D.h"
#include "DecorateAll.h"
#include "..\Engine\IRenderer.h"

static const std::string Catacomb3DName = "Catacomb 3-D v1.22";

GameCatacomb3D::GameCatacomb3D(const std::string gamePath, IRenderer& renderer) :
    m_gameId(5),
    m_gamePath(gamePath),
    m_renderer(renderer),
    m_introView(NULL)
{
    m_gameMaps = NULL;
    m_egaGraph = NULL;
    m_audioRepository = NULL;
    m_audioPlayer = NULL;
}

GameCatacomb3D::~GameCatacomb3D()
{
    if (m_gameMaps != NULL)
    {
        delete m_gameMaps;
    }
    if (m_egaGraph != NULL)
    {
        delete m_egaGraph;
    }
    if (m_audioRepository != NULL)
    {
        delete m_audioRepository;
    }
    if (m_audioPlayer != NULL)
    {
        delete m_audioPlayer;
    }
    if (m_introView != NULL)
    {
        delete m_introView;
    }
}

void GameCatacomb3D::SpawnActors(Level* level, const DifficultyLevel difficultyLevel)
{
    Actor** actors = level->GetBlockingActors();
    Actor* const playerState = new Actor(0, 0, 0, decoratePlayer);
    level->SetPlayerActor(playerState);

    for (uint16_t y = 0; y < level->GetLevelHeight(); y++)
    {
        for (uint16_t x = 0; x < level->GetLevelWidth(); x++)
        {
            const uint16_t tile = level->GetFloorTile(x, y);
            switch (tile)
            {
            case 1:
            case 2:
            case 3:
            case 4:
            {
                playerState->SetX(x + 0.5f);
                playerState->SetY(y + 0.5f);
                playerState->SetAngle((tile - 1) * 90.0f);
                break;
            }

            default:
                break;
            }
        }
    }
}

const DecorateActor& GameCatacomb3D::GetExplosionActor()
{
    return decorateExplosion;
}

const DecorateActor& GameCatacomb3D::GetExplodingWallActor()
{
    return decorateExplodingWall;
}

const DecorateActor& GameCatacomb3D::GetPlayerActor()
{
    return decoratePlayer;
}

GameMaps* GameCatacomb3D::GetGameMaps()
{
    if (m_gameMaps == NULL)
    {
        m_gameMaps = new GameMaps(gameMapsCatacomb3D, m_gamePath);
    }

    return m_gameMaps;
}

EgaGraph* GameCatacomb3D::GetEgaGraph()
{
    if (m_egaGraph == NULL)
    {
        m_egaGraph = new EgaGraph(egaGraphCatacomb3D, m_gamePath, m_renderer);
    }

    return m_egaGraph;
}

AudioRepository* GameCatacomb3D::GetAudioRepository()
{
    if (m_audioRepository == NULL)
    {
        m_audioRepository = new AudioRepository(audioRepositoryCatacomb3D, m_gamePath);
    }

    return m_audioRepository;
}

AudioPlayer* GameCatacomb3D::GetAudioPlayer()
{
    if (m_audioPlayer == NULL)
    {
        m_audioPlayer = new AudioPlayer(GetAudioRepository());
    }

    return m_audioPlayer;
}

IIntroView* GameCatacomb3D::GetIntroView()
{
    if (m_introView == NULL)
    {
        m_introView = new IntroViewCatacomb3D(m_renderer, GetEgaGraph(), m_gamePath);
    }

    return m_introView;
}

void GameCatacomb3D::DrawHelpPage()
{
    m_renderer.Render2DBar(0, 0, 640, 200, EgaLightGray);
    const Font* font = GetEgaGraph()->GetFont(3);
    m_renderer.RenderTextLeftAligned("                    SUMMARY OF GAME CONTROLS", font, EgaBlue, 128, 19);
    m_renderer.RenderTextLeftAligned("         ACTION", font, EgaRed, 128, 37);
    m_renderer.RenderTextLeftAligned("Arrow keys, joystick, or mouse", font, EgaRed, 128, 55);
    m_renderer.RenderTextLeftAligned("TAB or V while turning", font, EgaRed, 128, 64);
    m_renderer.RenderTextLeftAligned("ALT or Button 2 while turning", font, EgaRed, 128, 73);
    m_renderer.RenderTextLeftAligned("CTRL or Button 1", font, EgaRed, 128, 82);
    m_renderer.RenderTextLeftAligned("Z", font, EgaRed, 128, 91);
    m_renderer.RenderTextLeftAligned("X or Enter", font, EgaRed, 128, 100);
    m_renderer.RenderTextLeftAligned("F1", font, EgaRed, 128, 109);
    m_renderer.RenderTextLeftAligned("F2", font, EgaRed, 128, 118);
    m_renderer.RenderTextLeftAligned("F3", font, EgaRed, 128, 127);
    m_renderer.RenderTextLeftAligned("F4", font, EgaRed, 128, 136);
    m_renderer.RenderTextLeftAligned("F5", font, EgaRed, 128, 145);
    m_renderer.RenderTextLeftAligned("ESC", font, EgaRed, 128, 154);

    m_renderer.RenderTextLeftAligned("   REACTION", font, EgaDarkGray, 400, 37);
    m_renderer.RenderTextLeftAligned("Move and turn", font, EgaDarkGray, 400, 55);
    m_renderer.RenderTextLeftAligned("Turn quickly (Quick Turn)", font, EgaDarkGray, 400, 64);
    m_renderer.RenderTextLeftAligned("Move sideways", font, EgaDarkGray, 400, 73);
    m_renderer.RenderTextLeftAligned("Shoot a Missile", font, EgaDarkGray, 400, 82);
    m_renderer.RenderTextLeftAligned("Shoot a Zapper", font, EgaDarkGray, 400, 91);
    m_renderer.RenderTextLeftAligned("Shoot an Xterminator", font, EgaDarkGray, 400, 100);
    m_renderer.RenderTextLeftAligned("Help (this screen)", font, EgaDarkGray, 400, 109);
    m_renderer.RenderTextLeftAligned("Sound control", font, EgaDarkGray, 400, 118);
    m_renderer.RenderTextLeftAligned("Save game position", font, EgaDarkGray, 400, 127);
    m_renderer.RenderTextLeftAligned("Restore a saved game", font, EgaDarkGray, 400, 136);
    m_renderer.RenderTextLeftAligned("Joystick control", font, EgaDarkGray, 400, 145);
    m_renderer.RenderTextLeftAligned("System options", font, EgaDarkGray, 400, 154);
}

bool GameCatacomb3D::ProcessInputOnHelpPage(PlayerInput& playerInput)
{
    return (playerInput.GetFirstKeyPressed() != SDLK_UNKNOWN);
}

const std::map<uint16_t, const DecorateActor>& GameCatacomb3D::GetDecorateActors() const
{
    return decorateCatacomb3DAll;
}

const std::string& GameCatacomb3D::GetName() const
{
    return Catacomb3DName;
}

const uint8_t GameCatacomb3D::GetId() const
{
    return m_gameId;
}

void GameCatacomb3D::DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory)
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::STATUSPIC), 0, 120);

    m_renderer.RenderTextCentered(locationMessage.c_str(), GetEgaGraph()->GetFont(3), EgaBrightYellow, 160, 121);
}

void GameCatacomb3D::DrawScroll(const uint8_t scrollIndex)
{
    // TODO
}

void GameCatacomb3D::DrawFinal()
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(FINALEPIC), 0, 0);
}

const uint16_t GameCatacomb3D::GetMenuCursorPic() const
{
    return PSHOT1PIC;
}

const uint16_t GameCatacomb3D::GetNorthIconSprite() const
{
    return 0;
}

const std::string GameCatacomb3D::GetSavedGamesPath() const
{
    return "\\Cat3D";
}

const std::vector<std::vector<uint16_t>> wallSkeletonAnimations = { { 0 } };
const std::vector<std::vector<uint16_t>>& GameCatacomb3D::GetWallSkeletonAnimations() const
{
    return wallSkeletonAnimations;
}

const SDL_Keycode GameCatacomb3D::GetCheatsKeyCode() const
{
    return SDLK_F10;
}

void GameCatacomb3D::PlaySoundBodyExplode()
{
    
}

void GameCatacomb3D::PlaySoundHitGate()
{
    
}

void GameCatacomb3D::PlaySoundPlayerHurt(const int16_t playerHealth)
{
    if (playerHealth > 33)
    {
        m_audioPlayer->Play(TAKEDAMAGESND);
    }
    else if (playerHealth > 0)
    {
        m_audioPlayer->Play(TAKEDMGHURTSND);
    }
    else
    {
        m_audioPlayer->Play(GAMEOVERSND);
    }
}

void GameCatacomb3D::PlaySoundGetPotion()
{
    m_audioPlayer->Play(GETPOTIONSND);
}

void GameCatacomb3D::PlaySoundUsePotion()
{
    m_audioPlayer->Play(USEPOTIONSND);
}

void GameCatacomb3D::PlaySoundNoItem()
{
    m_audioPlayer->Play(NOITEMSND);
}

void GameCatacomb3D::PlaySoundGetBolt()
{
    m_audioPlayer->Play(GETBOLTSND);
}

void GameCatacomb3D::PlaySoundGetNuke()
{
    m_audioPlayer->Play(GETNUKESND);
}

void GameCatacomb3D::PlaySoundGetScroll()
{
    m_audioPlayer->Play(GETSCROLLSND);
}

void GameCatacomb3D::PlaySoundGetGem()
{
    // No gems in Catacomb 3-D
}

void GameCatacomb3D::PlaySoundGetKey()
{
    m_audioPlayer->Play(GETKEYSND);
}

void GameCatacomb3D::PlaySoundGetPoints()
{
    m_audioPlayer->Play(GETPOINTSSND);
}

void GameCatacomb3D::PlaySoundUseKey()
{
    m_audioPlayer->Play(USEKEYSND);
}

void GameCatacomb3D::PlaySoundUseBolt()
{
    m_audioPlayer->Play(USEBOLTSND);
}

void GameCatacomb3D::PlaySoundUseNuke()
{
    m_audioPlayer->Play(USENUKESND);
}

void GameCatacomb3D::PlaySoundFreezeTime()
{
    // No freezetime in Catacomb 3-D
}

void GameCatacomb3D::PlaySoundFreezeTimeTick(const int32_t seconds)
{
    // No freezetime in Catacomb 3-D
}

void GameCatacomb3D::PlaySoundWalk(const bool walk1)
{
    m_audioPlayer->Play(walk1 ? WALK1SND : WALK2SND);
}

void GameCatacomb3D::PlaySoundBossDeath()
{
    // TODO
}

void GameCatacomb3D::PlaySoundShoot()
{
    m_audioPlayer->Play(SHOOTSND);
}

void GameCatacomb3D::PlaySoundShootWall()
{
    m_audioPlayer->Play(SHOOTWALLSND);
}

void GameCatacomb3D::PlaySoundBoom()
{
    
}

void GameCatacomb3D::PlaySoundSmallMonster()
{
}

void GameCatacomb3D::PlaySoundLargeMonster()
{
}

void GameCatacomb3D::PlaySoundWarpUpOrDown(const bool up)
{
    m_audioPlayer->Play(up ? WARPUPSND : WARPDOWNSND);
}

void GameCatacomb3D::PlaySoundWarp()
{
    // TODO
}

void GameCatacomb3D::PlaySoundPortal()
{

}