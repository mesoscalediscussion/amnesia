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

#include <assert.h>
#include <stdlib.h>

#include "system/LowLevelSystem.h"
#include "system/Platform.h"

#include "impl/LowLevelGraphicsSDL.h"

#ifndef WIN32
#define CALLBACK __attribute__ ((__stdcall__))
#endif

namespace hpl {

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	cLowLevelGraphicsSDL::cLowLevelGraphicsSDL()
	{
		mlMultisampling =0;
        mpScreen = 0;
        mbGrab = false;

		mbInitHasBeenRun = false;

		//TTF_Init();
	}

	//-----------------------------------------------------------------------

	cLowLevelGraphicsSDL::~cLowLevelGraphicsSDL()
	{
        SDL_DestroyWindow(mpScreen);
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// GENERAL SETUP
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDL::ShowCursor(bool abX)
	{
		;
		if (abX)
			SDL_ShowCursor();
		else
			SDL_HideCursor();
	}
    
	//-----------------------------------------------------------------------

    void cLowLevelGraphicsSDL::SetWindowGrab(bool abX)
    {
		mbGrab = abX;
		if (mpScreen) {
			SDL_SetWindowMouseGrab(mpScreen, abX);
		}
    }

	void cLowLevelGraphicsSDL::SetRelativeMouse(bool abX)
	{
		SDL_SetWindowRelativeMouseMode(mpScreen, abX);
	}

    void cLowLevelGraphicsSDL::SetWindowCaption(const tString &asName)
    {
        SDL_SetWindowTitle(mpScreen, asName.c_str());
    }

    bool cLowLevelGraphicsSDL::GetWindowMouseFocus()
    {
        return (SDL_GetWindowFlags(mpScreen) & SDL_WINDOW_MOUSE_FOCUS) != 0;
    }

    bool cLowLevelGraphicsSDL::GetWindowInputFocus()
    {
        return (SDL_GetWindowFlags(mpScreen) & SDL_WINDOW_INPUT_FOCUS) != 0;
    }

    bool cLowLevelGraphicsSDL::GetWindowIsVisible()
    {
		return (SDL_GetWindowFlags(mpScreen) & SDL_WINDOW_HIDDEN) == 0;
    }

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDL::SetVsyncActive(bool abX, bool abAdaptive)
	{
        ;
        SDL_GL_SetSwapInterval(abX ? (abAdaptive ? -1 : 1) : 0);
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDL::SetMultisamplingActive(bool abX)
	{	
		;

		if(!GLAD_GL_ARB_multisample || mlMultisampling<=0) return;

		if(abX)
			glEnable(GL_MULTISAMPLE_ARB);
		else
			glDisable(GL_MULTISAMPLE_ARB);
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDL::SetGammaCorrection(float afX)
	{
		;

		mfGammaCorrection = afX;
	}

	float cLowLevelGraphicsSDL::GetGammaCorrection()
	{
		;

		return mfGammaCorrection;
	}

	//-----------------------------------------------------------------------

	cVector2f cLowLevelGraphicsSDL::GetScreenSizeFloat()
	{
		;

		return cVector2f((float)mvScreenSize.x, (float)mvScreenSize.y);
	}

	const cVector2l& cLowLevelGraphicsSDL::GetScreenSizeInt()
	{
		;

		return mvScreenSize;
	}

	//-----------------------------------------------------------------------

}
