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
#include "system/Mutex.h"

#include "impl/LowLevelGraphicsSDLGpu.h"
#include "impl/SDLFontData.h"

#include "graphics/Bitmap.h"
#include "impl/FrameBufferGL.h"

#ifndef WIN32
#define CALLBACK __attribute__ ((__stdcall__))
#endif

namespace hpl {
	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	cLowLevelGraphicsSDLGpu::cLowLevelGraphicsSDLGpu()
	{
		mCurrentFrameBuffer = CreateFrameBuffer(". .");
	}

	//-----------------------------------------------------------------------

	cLowLevelGraphicsSDLGpu::~cLowLevelGraphicsSDLGpu()
	{
		Log("cleaning up SDL_gpu renderer..\n");
		SDL_ReleaseWindowFromGPUDevice(mpDevice, mpScreen);
		SDL_DestroyGPUDevice(mpDevice);
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// GENERAL SETUP
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	bool cLowLevelGraphicsSDLGpu::Init(int alWidth, int alHeight, int alDisplay, int alBpp, int abFullscreen,
		int alMultisampling, eGpuProgramFormat aGpuProgramFormat,const tString& asWindowCaption,
		const cVector2l &avWindowPos)
	{
		mvScreenSize.x = alWidth;
		mvScreenSize.y = alHeight;
        mlDisplay = alDisplay;
		mlBpp = alBpp;
		mbFullscreen = abFullscreen;

		mlMultisampling = alMultisampling;

		mGpuProgramFormat = aGpuProgramFormat;
		if(mGpuProgramFormat == eGpuProgramFormat_LastEnum) mGpuProgramFormat = eGpuProgramFormat_SPIRV;

		SDL_WindowFlags mlFlags = 0;
		if (alWidth == 0 && alHeight == 0) {
			mvScreenSize = cVector2l(800,600);
			mlFlags |= SDL_WINDOW_FULLSCREEN;
		} else if (abFullscreen) {
			mlFlags |= SDL_WINDOW_FULLSCREEN;
		}
		Log(" Setting video mode: %d x %d - %d bpp\n",alWidth, alHeight, alBpp);
		mpScreen = SDL_CreateWindow(asWindowCaption.c_str(), mvScreenSize.x, mvScreenSize.y, mlFlags);

		if (!mpScreen) {
			FatalError("Unable to create SDL window! %s\n", SDL_GetError());
		}

		int w, h;
		SDL_GetWindowSizeInPixels(mpScreen, &w, &h);
		mvScreenSize = cVector2l(w, h);

		if (mbGrab) {
			SetWindowGrab(true);
		}

		// create the gpu device..
		bool bUseDebugMode = false;
#ifdef DEBUG
		bUseDebugMode = true;
#endif
		mpDevice = SDL_CreateGPUDevice(
			SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL,
			bUseDebugMode,
			NULL);

		if (!mpDevice) {
			FatalError("unable to create GPU device! %s\n", SDL_GetError());
		}

		if (!SDL_ClaimWindowForGPUDevice(mpDevice, mpScreen)) {
			FatalError("unable to claim window for GPU device! %s\n", SDL_GetError());
		}

		// turn off cursor by default.
		ShowCursor(false);

		// log some info..
		//SDL_GetGPUDeviceProperties(mpDevice); // wait right this is 3.4.0 and that is not fully out yet :(
		Log("created SDL_gpu renderer! (backend %s)\n", SDL_GetGPUDeviceDriver(mpDevice));

		mbInitHasBeenRun = true;

		return true;
	}

	//-----------------------------------------------------------------------

	int cLowLevelGraphicsSDLGpu::GetCaps(eGraphicCaps aType)
	{
		// kind of just making these numbers up
		switch(aType)
		{
		case eGraphicCaps_TextureTargetRectangle:
				return 1;
		case eGraphicCaps_VertexBufferObject:
				return 1;
		case eGraphicCaps_TwoSideStencil:
				return 1;
		case eGraphicCaps_MaxTextureImageUnits:
				return 32;
		case eGraphicCaps_MaxTextureCoordUnits:
				return 16384;
		case eGraphicCaps_MaxUserClipPlanes:
				return 6;
		case eGraphicCaps_AnisotropicFiltering:
				return 1;
		case eGraphicCaps_MaxAnisotropicFiltering:
				return 16;
		case eGraphicCaps_Multisampling:
				return 1;
		case eGraphicCaps_TextureCompression:
		case eGraphicCaps_TextureCompression_DXTC:
				return 1;
		case eGraphicCaps_AutoGenerateMipMaps:
				return 1;
		case eGraphicCaps_RenderToTexture:
				return 1;
		case eGraphicCaps_MaxDrawBuffers:
				return 255;
		case eGraphicCaps_PackedDepthStencil:
				return 1;
		case eGraphicCaps_TextureFloat:
				return 1;
		case eGraphicCaps_PolygonOffset:
				return 1;
		case eGraphicCaps_ShaderModel_2:
		case eGraphicCaps_ShaderModel_3:
		case eGraphicCaps_ShaderModel_4:
				return 1;
		case eGraphicCaps_OGL_ATIFragmentShader:
				return 0;
		case eGraphicCaps_MaxColorRenderTargets:
				return 255;
		}
		return 0;
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetVsyncActive(bool abX, bool abAdaptive)
		{
		}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetMultisamplingActive(bool abX)
		{
		}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetGammaCorrection(float afX)
		{
			;

			mfGammaCorrection = afX;
		}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// DATA CREATION
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	iFontData* cLowLevelGraphicsSDLGpu::CreateFontData(const tString &asName)
	{
		return hplNew( cSDLFontData, (asName, (iLowLevelGraphics*)this) );
	}

	//-----------------------------------------------------------------------

	iGpuProgram* cLowLevelGraphicsSDLGpu::CreateGpuProgram(const tString& asName)
	{
		return hplNew( cGpuProgramSDLGpu, (asName, eGpuProgramFormat_SPIRV) );
	}

	iGpuShader* cLowLevelGraphicsSDLGpu::CreateGpuShader(const tString& asName, eGpuShaderType aType)
	{
		return hplNew( cGpuShaderSDLGpu, (asName, _W(""), aType, eGpuProgramFormat_SPIRV) );
	}

	//-----------------------------------------------------------------------

	iTexture* cLowLevelGraphicsSDLGpu::CreateTexture(const tString &asName,eTextureType aType,   eTextureUsage aUsage)
	{
		cTextureSDLGpu *pTexture = hplNew( cTextureSDLGpu, (asName, _W(""), aType, aUsage, this) );

		return pTexture;
	}

	//-----------------------------------------------------------------------

	iVertexBuffer* cLowLevelGraphicsSDLGpu::CreateVertexBuffer(eVertexBufferType aType,
															eVertexBufferDrawType aDrawType,
															eVertexBufferUsageType aUsageType,
															int alReserveVtxSize,int alReserveIdxSize)
	{
		return hplNew( cVertexBufferSDLGpu, (this, aType, aDrawType,aUsageType,alReserveVtxSize,alReserveIdxSize) );
	}

	//-----------------------------------------------------------------------

	iFrameBuffer* cLowLevelGraphicsSDLGpu::CreateFrameBuffer(const tString& asName)
	{
		return hplNew(cFrameBufferSDLGpu, (asName, (iLowLevelGraphics*)this));
	}

	//-----------------------------------------------------------------------

	iDepthStencilBuffer* cLowLevelGraphicsSDLGpu::CreateDepthStencilBuffer(const cVector2l& avSize, int alDepthBits, int alStencilBits)
	{
		return hplNew(cDepthStencilBufferSDLGpu,(avSize, alDepthBits, alStencilBits));
	}

	//-----------------------------------------------------------------------

	iOcclusionQuery* cLowLevelGraphicsSDLGpu::CreateOcclusionQuery()
	{
		return hplNew(cOcclusionQuerySDLGpu, ());
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// FRAME BUFFER OPERATIONS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::ClearFrameBuffer(tClearFrameBufferFlag aFlags)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetClearColor(const cColor& aCol){
	}

	void cLowLevelGraphicsSDLGpu::SetClearDepth(float afDepth){
	}

	void cLowLevelGraphicsSDLGpu::SetClearStencil(int alVal){
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::CopyFrameBufferToTexure(iTexture* apTex, const cVector2l &avPos,
		const cVector2l &avSize, const cVector2l &avTexOffset)
	{
	}

	//-----------------------------------------------------------------------

	cBitmap* cLowLevelGraphicsSDLGpu::CopyFrameBufferToBitmap(	const cVector2l &avScreenPos,const cVector2l &avScreenSize)
	{
		return hplNew(cBitmap, ());
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetCurrentFrameBuffer(iFrameBuffer* apFrameBuffer, const cVector2l &avPos, const cVector2l& avSize)
	{
	}

	//-----------------------------------------------------------------------

	iFrameBuffer* cLowLevelGraphicsSDLGpu::GetCurrentFrameBuffer()
	{
		return mCurrentFrameBuffer;
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetFrameBufferDrawTargets(int *apTargets, int alNumOfTargets)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::WaitAndFinishRendering()
	{
	}

	//-----------------------------------------------------------------------


	void cLowLevelGraphicsSDLGpu::FlushRendering()
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SwapBuffers()
	{
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// RENDER STATE
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetColorWriteActive(bool abR,bool abG,bool abB,bool abA)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetDepthWriteActive(bool abX)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetDepthTestActive(bool abX)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetDepthTestFunc(eDepthTestFunc aFunc)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetAlphaTestActive(bool abX)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetAlphaTestFunc(eAlphaTestFunc aFunc,float afRef)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetStencilActive(bool abX)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetStencilWriteMask(unsigned int alMask)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetStencil(eStencilFunc aFunc,int alRef, unsigned int aMask,
		eStencilOp aFailOp,eStencilOp aZFailOp,eStencilOp aZPassOp)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetStencilTwoSide(eStencilFunc aFrontFunc,eStencilFunc aBackFunc,
		int alRef, unsigned int aMask,
		eStencilOp aFrontFailOp,eStencilOp aFrontZFailOp,eStencilOp aFrontZPassOp,
		eStencilOp aBackFailOp,eStencilOp aBackZFailOp,eStencilOp aBackZPassOp)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetCullActive(bool abX)
	{
	}

	void cLowLevelGraphicsSDLGpu::SetCullMode(eCullMode aMode)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetScissorActive(bool abX)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetScissorRect(const cVector2l& avPos, const cVector2l& avSize)
	{
	}

	//-----------------------------------------------------------------------


	void cLowLevelGraphicsSDLGpu::SetClipPlane(int alIdx, const cPlanef& aPlane)
	{
	}

	cPlanef cLowLevelGraphicsSDLGpu::GetClipPlane(int alIdx)
	{
		return cPlanef();
	}

	void cLowLevelGraphicsSDLGpu::SetClipPlaneActive(int alIdx, bool abX)
	{
	}


	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetBlendActive(bool abX)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetBlendFunc(eBlendFunc aSrcFactor, eBlendFunc aDestFactor)
	{
	}

	//-----------------------------------------------------------------------


	void cLowLevelGraphicsSDLGpu::SetBlendFuncSeparate(eBlendFunc aSrcFactorColor, eBlendFunc aDestFactorColor,
		eBlendFunc aSrcFactorAlpha, eBlendFunc aDestFactorAlpha)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetPolygonOffsetActive(bool abX)
	{
	}

	void cLowLevelGraphicsSDLGpu::SetPolygonOffset(float afBias, float afSlopeScaleBias)
	{
	}

	//-----------------------------------------------------------------------


	//////////////////////////////////////////////////////////////////////////
	// MATRIX
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------


	void cLowLevelGraphicsSDLGpu::PushMatrix(eMatrix aMtxType)
	{
	}

	//-----------------------------------------------------------------------


	void cLowLevelGraphicsSDLGpu::PopMatrix(eMatrix aMtxType)
	{
	}
	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetMatrix(eMatrix aMtxType, const cMatrixf& a_mtxA)
	{
	}

	//-----------------------------------------------------------------------


	void cLowLevelGraphicsSDLGpu::SetIdentityMatrix(eMatrix aMtxType)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetOrthoProjection(const cVector2f& avSize, float afMin, float afMax)
	{
	}

	void cLowLevelGraphicsSDLGpu::SetOrthoProjection(const cVector3f& avMin, const cVector3f& avMax)
	{
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// TEXTURE OPERATIONS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------


	void cLowLevelGraphicsSDLGpu::SetTexture(unsigned int alUnit,iTexture* apTex)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetActiveTextureUnit(unsigned int alUnit)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetTextureEnv(eTextureParam aParam, int alVal)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetTextureConstantColor(const cColor &aColor)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetColor(const cColor &aColor)
	{
	}

	//-----------------------------------------------------------------------



	//////////////////////////////////////////////////////////////////////////
	// DRAWING
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawTriangle(tVertexVec& avVtx)
	{
		assert(avVtx.size()==3);
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawQuad(const cVector3f &avPos,const cVector2f &avSize,const cColor& aColor)
	{
	}

	void cLowLevelGraphicsSDLGpu::DrawQuad(const cVector3f &avPos,const cVector2f &avSize,
		const cVector2f &avMinTexCoord,const cVector2f &avMaxTexCoord,
		const cColor& aColor)
	{
	}

	void cLowLevelGraphicsSDLGpu::DrawQuad(const cVector3f &avPos,const cVector2f &avSize,
		const cVector2f &avMinTexCoord0,const cVector2f &avMaxTexCoord0,
		const cVector2f &avMinTexCoord1,const cVector2f &avMaxTexCoord1,
		const cColor& aColor)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawQuad(const tVertexVec &avVtx)
	{
		assert(avVtx.size()==4);
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawQuadMultiTex(const tVertexVec &avVtx,const tVector3fVec &avExtraUvs)
	{
	}


	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawQuad(const tVertexVec &avVtx, const cColor aCol)
	{
		assert(avVtx.size()==4);
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawQuad(const tVertexVec &avVtx,const float afZ)
	{
		assert(avVtx.size()==4);
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawQuad(const tVertexVec &avVtx,const float afZ,const cColor &aCol)
	{
		assert(avVtx.size()==4);
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawLine(const cVector3f& avBegin, const cVector3f& avEnd, cColor aCol)
	{
	}

	void cLowLevelGraphicsSDLGpu::DrawLine(const cVector3f& avBegin, const cColor& aBeginCol, const cVector3f& avEnd, const cColor& aEndCol)
	{
	}

	void cLowLevelGraphicsSDLGpu::DrawBoxMinMax(const cVector3f& avMin, const cVector3f& avMax, cColor aCol)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawSphere(const cVector3f& avPos, float afRadius, cColor aCol)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawSphere(const cVector3f& avPos, float afRadius, cColor aColX, cColor aColY, cColor aColZ)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::DrawLineQuad(const cRect2f& aRect, float afZ, cColor aCol)
	{
	}

	void cLowLevelGraphicsSDLGpu::DrawLineQuad(const cVector3f &avPos,const cVector2f &avSize, cColor aCol)
	{
	}



	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// VERTEX BATCHING
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::AddVertexToBatch(const cVertex *apVtx)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::AddVertexToBatch(const cVertex *apVtx, const cVector3f* avTransform)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::AddVertexToBatch(const cVertex *apVtx, const cMatrixf* aMtx)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::AddVertexToBatch_Size2D(const cVertex *apVtx, const cVector3f* avTransform,
		const cColor* apCol,const float& mfW, const float& mfH)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::AddVertexToBatch_Raw(	const cVector3f& avPos, const cColor &aColor,
		const cVector3f& avTex)
	{
	}


	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::AddIndexToBatch(int alIndex)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::AddTexCoordToBatch(unsigned int alUnit,const cVector3f *apCoord)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::SetBatchTextureUnitActive(unsigned int alUnit,bool abActive)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::FlushTriBatch(tVtxBatchFlag aTypeFlags, bool abAutoClear)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::FlushQuadBatch(tVtxBatchFlag aTypeFlags, bool abAutoClear)
	{
	}

	//-----------------------------------------------------------------------

	void cLowLevelGraphicsSDLGpu::ClearBatch()
	{
	}
}
