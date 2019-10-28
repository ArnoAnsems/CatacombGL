// Copyright (C) 2018 Arno Ansems
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

#include "AudioPlayer.h"
#include <stdlib.h>
#include "..\..\ThirdParty\RefKeen\id_sd.h"

AudioPlayer::AudioPlayer(AudioRepository* audioRepository) :
    m_audioRepository(audioRepository)
{

}

AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::Play(const uint16_t index)
{
    if (SD_GetSoundMode() == sdm_AdLib)
    {
        AdlibSound* sound = m_audioRepository->GetAdlibSound(index);
        if (sound != NULL)
        {
            SDL_ALPlaySound(sound);
        }
    }
    else if (SD_GetSoundMode() == sdm_PC)
    {
        PCSound* sound = m_audioRepository->GetPCSound(index);
        if (sound != NULL)
        {
            SDL_PCPlaySound(sound);
        }
    }
}

bool AudioPlayer::IsPlaying()
{
    return SD_SoundPlaying();
}