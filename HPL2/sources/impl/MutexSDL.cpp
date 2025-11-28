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

#include "impl/MutexSDL.h"

#if USE_SDL2
#include "SDL2/SDL.h"
#elif USE_SDL3
#include <SDL3/SDL.h>
#endif

namespace hpl {

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	
	cMutexSDL::cMutexSDL()
	{
		mpMutexHandle = NULL;
		mpMutexHandle = SDL_CreateMutex();
	}

	//-----------------------------------------------------------------------

	cMutexSDL::~cMutexSDL()
	{
		if(mpMutexHandle)
			SDL_DestroyMutex(mpMutexHandle);
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

#if USE_SDL2
	bool cMutexSDL::Lock()
	{
		return SDL_LockMutex(mpMutexHandle)!=-1;
	}
	
	bool cMutexSDL::Unlock()
	{
		return SDL_UnlockMutex(mpMutexHandle)!=-1;
	}
#elif USE_SDL3
	bool cMutexSDL::Lock()
	{
		SDL_LockMutex(mpMutexHandle);
		return true;
	}

	bool cMutexSDL::Unlock()
	{
		SDL_UnlockMutex(mpMutexHandle);
		return true;
	}
#endif

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PRIVATE METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------

}

