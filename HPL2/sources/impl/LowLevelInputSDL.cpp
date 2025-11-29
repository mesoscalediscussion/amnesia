/*
 * Copyright © 2011-2020 Frictional Games
 * 
 * This file is part of Amnesia: A Machine For Pigs.
 * 
 * Amnesia: A Machine For Pigs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. 

 * Amnesia: A Machine For Pigs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Amnesia: A Machine For Pigs.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "impl/LowLevelInputSDL.h"

#include "impl/MouseSDL.h"
#include "impl/KeyboardSDL.h"
#include "impl/GamepadSDL3.h"

#include "system/LowLevelSystem.h"
#include "graphics/LowLevelGraphics.h"

#include "engine/Engine.h"

#include <SDL3/SDL.h>

namespace hpl {

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	cLowLevelInputSDL::cLowLevelInputSDL(iLowLevelGraphics *apLowLevelGraphics)
        : mpLowLevelGraphics(apLowLevelGraphics), mbQuitMessagePosted(false)
	{
		LockInput(true);
		RelativeMouse(false);
		SDL_InitSubSystem(SDL_INIT_GAMEPAD);
	}

	//-----------------------------------------------------------------------

	cLowLevelInputSDL::~cLowLevelInputSDL()
	{
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC METHOD
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	
	void cLowLevelInputSDL::LockInput(bool abX)
	{
		mpLowLevelGraphics->SetWindowGrab(abX);
	}

	void cLowLevelInputSDL::RelativeMouse(bool abX)
	{
		mpLowLevelGraphics->SetRelativeMouse(abX);
	}
	
	//-----------------------------------------------------------------------

	void cLowLevelInputSDL::BeginInputUpdate()
	{
		SDL_Event sdlEvent;

		mlstEvents.clear();
		while(SDL_PollEvent(&sdlEvent)!=0)
		{
			// built-in SDL3 gamepad hotplug code
			// this whole contract should be rewritten to allow clean adding/removing
			// of controllers, instead of brute force rescanning
			if (sdlEvent.type == SDL_EVENT_GAMEPAD_ADDED)
			{
				// sdlEvent.cdevice.which is the device #
				cEngine::SetDeviceWasPlugged();
			}
			else if (sdlEvent.type == SDL_EVENT_GAMEPAD_REMOVED)
			{
				// sdlEvent.cdevice.which is the instance # (not device #).
				// instance # increases as devices are plugged and unplugged.
				cEngine::SetDeviceWasRemoved();
			}
#if defined (__APPLE__)
			if (sdlEvent.type == SDL_EVENT_KEY_DOWN)
			{
				if (sdlEvent.key.key == SDLK_Q && sdlEvent.key.mod & SDL_KMOD_GUI) {
					mbQuitMessagePosted = true;
				}
				else {
					mlstEvents.push_back(sdlEvent);
				}
			}
			else
#endif
				if (sdlEvent.type == SDL_EVENT_QUIT)
				{
					mbQuitMessagePosted = true;
				}
				else
					mlstEvents.push_back(sdlEvent);
		}
	}
	
	//-----------------------------------------------------------------------

	void cLowLevelInputSDL::EndInputUpdate()
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelInputSDL::InitGamepadSupport()
	{
	}

	void cLowLevelInputSDL::DropGamepadSupport()
	{
	}

	int cLowLevelInputSDL::GetPluggedGamepadNum()
	{
		int num_gamepads;
		SDL_GetGamepads(&num_gamepads);
		return num_gamepads;
	}

	//-----------------------------------------------------------------------
	
	iMouse* cLowLevelInputSDL::CreateMouse()
	{
		return hplNew( cMouseSDL,(this));
	}
	
	//-----------------------------------------------------------------------
	
	iKeyboard* cLowLevelInputSDL::CreateKeyboard()
	{
		return hplNew( cKeyboardSDL,(this) );
	}

	//-----------------------------------------------------------------------

	iGamepad* cLowLevelInputSDL::CreateGamepad(int alIndex)
	{
		return hplNew( cGamepadSDL3, (this, alIndex) );
	}
	
	//-----------------------------------------------------------------------

    bool cLowLevelInputSDL::isQuitMessagePosted()
    {
        return mbQuitMessagePosted;
    }
    
    void cLowLevelInputSDL::resetQuitMessagePosted()
    {
        mbQuitMessagePosted = false;
    }

	//-----------------------------------------------------------------------
    
}
