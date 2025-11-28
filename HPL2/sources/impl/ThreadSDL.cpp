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

#include "impl/ThreadSDL.h"

#if USE_SDL2
#include "SDL2/SDL.h"
#elif USE_SDL3
#include <SDL3/SDL.h>
#endif
#include <typeinfo>

namespace hpl {

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	
	cThreadSDL::cThreadSDL()
	{
		mpThreadHandle = NULL;
	}

	//-----------------------------------------------------------------------

	cThreadSDL::~cThreadSDL()
	{

	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	
	void cThreadSDL::Start()
	{
		mbThreadActive = true;
#if USE_SDL2
        mpThreadHandle = SDL_CreateThread(iThread::MainThreadFunc, typeid(this).name(), this);
#elif USE_SDL3
		mpThreadHandle = SDL_CreateThread(iThread::MainThreadFunc, typeid(this).name(), this);
#endif
	}

	//-----------------------------------------------------------------------
	
	void cThreadSDL::Stop()
	{
		mbThreadActive = false;
		SDL_WaitThread(mpThreadHandle,0);
		mpThreadHandle = NULL;
	}

	//-----------------------------------------------------------------------

	void cThreadSDL::Sleep(unsigned int alSleepTime)
	{
		SDL_Delay(alSleepTime);
	}

	//-----------------------------------------------------------------------

	void cThreadSDL::SetPriority(eThreadPrio aPrio)
	{
        int prio = TranslateEnginePrio(aPrio);
#if USE_SDL2
        SDL_SetThreadPriority((SDL_ThreadPriority)prio);
#elif USE_SDL3
		SDL_SetCurrentThreadPriority((SDL_ThreadPriority)prio);
#endif
		iThread::SetPriority(aPrio);
	}

	//-----------------------------------------------------------------------

	int cThreadSDL::TranslateEnginePrio(eThreadPrio aPrio)
	{
        switch (aPrio) {
            case eThreadPrio_Low:
                return SDL_THREAD_PRIORITY_LOW;
            case eThreadPrio_High:
                return SDL_THREAD_PRIORITY_HIGH;
            case eThreadPrio_Normal:
            default:
                return SDL_THREAD_PRIORITY_NORMAL;
		}
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PRIVATE METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------

}

