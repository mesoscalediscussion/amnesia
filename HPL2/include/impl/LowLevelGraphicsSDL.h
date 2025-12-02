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

#ifndef HPL_LOWLEVELGRAPHICS_SDL_H
#define HPL_LOWLEVELGRAPHICS_SDL_H

#include <SDL3/SDL.h>

#include <glad/gl.h>

// Include these AFTER SDL
#ifdef __linux__
#include <GL/glx.h>
#endif

#include "graphics/LowLevelGraphics.h"
#include "math/MathTypes.h"


namespace hpl {

#ifdef __linux__
	typedef int (*GLXSWAPINTERVALPROC)(int interval);
#endif
	
	//-------------------------------------------------	

	class cColorWriteDL
	{
	public:
		bool r;
		bool g;
		bool b;
		bool a;
	};

	//-------------------------------------------------

	class cLowLevelGraphicsSDL : public iLowLevelGraphics
	{
	public:
		cLowLevelGraphicsSDL();
		~cLowLevelGraphicsSDL();

		/////////////////////////////////////////////////////
		/////////////// GENERAL SETUP ///////////////////////
		/////////////////////////////////////////////////////

		virtual bool Init(	int alWidth, int alHeight, int alDisplay, int alBpp, int abFullscreen, int alMultisampling,
					eGpuProgramFormat aGpuProgramFormat,const tString& asWindowCaption,
					const cVector2l &avWindowPos) = 0;

		virtual eGpuProgramFormat GetGpuProgramFormat(){ return mGpuProgramFormat;}

		virtual void ShowCursor(bool abX);

        virtual void SetWindowGrab(bool abX);

        virtual void SetRelativeMouse(bool abX);

        virtual void SetWindowCaption(const tString &asName);

        virtual bool GetWindowMouseFocus();
        
        virtual bool GetWindowInputFocus();
        
        virtual bool GetWindowIsVisible();

		virtual bool GetFullscreenModeActive() { return mbFullscreen; }

		virtual void SetVsyncActive(bool abX, bool abAdaptive);

		virtual void SetMultisamplingActive(bool abX);

		virtual void SetGammaCorrection(float afX);
		virtual float GetGammaCorrection();

		virtual int GetMultisampling(){ return mlMultisampling;}

		virtual cVector2f GetScreenSizeFloat();
		virtual const cVector2l& GetScreenSizeInt();

	protected:
        cVector2l mvScreenSize;
        int mlDisplay;
		int mlMultisampling;
		int mlBpp;
		bool mbFullscreen;
		eGpuProgramFormat mGpuProgramFormat;

		bool mbInitHasBeenRun;
		
		//////////////////////////////////////
		//Gamma
		Uint16 mvStartGammaArray[3][256];
		float mfGammaCorrection;

		//////////////////////////////////////
		//SDL Variables
        SDL_Window *mpScreen;
        SDL_GLContext mGLContext;
        bool mbGrab;
	};
};
#endif // HPL_LOWLEVELGRAPHICS_SDL_H
