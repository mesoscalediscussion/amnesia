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

#ifndef HPL_GAMEPAD_SDL3_H
#define HPL_GAMEPAD_SDL3_H

#include <vector>
#include <list>
#include "system/SystemTypes.h"
#include "input/Gamepad.h"
//#include "input/InputTypes.h"

#include "SDL3/SDL.h"

namespace hpl {

	class cLowLevelInputSDL;
	
	class cGamepadSDL3 : public iGamepad
	{
	public:
		cGamepadSDL3(cLowLevelInputSDL *apLowLevelInputSDL, int alIndex);

		tString GetGamepadName() { return msGamepadName; }

		int GetNumButtons();
		int GetNumAxes();
		int GetNumHats();
		int GetNumBalls();

		void Update();

		//Gamepad specific
		bool HasInputUpdates();
		cGamepadInputData GetInputUpdate();
		
		bool ButtonIsDown(eGamepadButton aButton);
		cGamepadInputData GetButton();
		bool ButtonIsPressed();
		bool ButtonIsReleased();
		cGamepadInputData GetReleasedButton();

		float GetAxisValue(eGamepadAxis aAxis);
		float GetAxisDeadZoneRadiusValue();
		void SetAxisDeadZoneRadiusValue(float afValue);
		bool AxesUpdated();
		cGamepadInputData GetUpdatedAxis();

		eGamepadHatState GetHatCurrentState(eGamepadHat aHat);
		bool HatIsInState(eGamepadHat aHat, eGamepadHatState aState);
		bool HatsChanged();
		cGamepadInputData GetHatState();

		cVector2l GetBallAbsPos(eGamepadBall aBall);
		cVector2l GetBallRelPos(eGamepadBall aBall);

	private:
		eGamepadButton		SDLToButton(uint8_t alButton);
		eGamepadAxis		SDLToAxis(uint8_t alAxis);
		float				SDLToAxisValue(int16_t alAxisValue);
		eGamepadHat			SDLToHat(uint8_t alHat);
		eGamepadHatState	SDLToHatState(uint8_t alHat);
		eGamepadBall		SDLToBall(uint8_t alBall);
        //void ClearKeyList();
		//eKey AsciiToKey(int alChar);

		//void AddKeyToList(int alSDLMod, eKey aKey, int alUnicode, std::list<cKeyPress>& alstKeys);

		tString msGamepadName;
		
		std::vector<float>				mvAxisArray;
		std::vector<eGamepadAxisRange>	mvAxisRange;
		std::vector<bool>				mvButtonArray;

#ifdef WIN32
		std::vector<float>				mvRemappedAxisArray;
		std::vector<eGamepadHatState>	mvHatArray;
		std::vector<bool>				mvRemappedButtonArray;
#endif

		std::list<cGamepadInputData> mlstInputUpdates;

		std::list<cGamepadInputData> mlstButtonsPressed;
		std::list<cGamepadInputData> mlstButtonsReleased;

		std::list<cGamepadInputData> mlstAxisChanges;

		SDL_Gamepad*		mpHandle;
		SDL_JoystickID      mlInstance;
		cLowLevelInputSDL	*mpLowLevelInputSDL;

		static float mfInvAxisMax;
		static float mfDeadZoneRadius;
	};

};

#endif // HPL_GAMEPAD_SDL3_H
