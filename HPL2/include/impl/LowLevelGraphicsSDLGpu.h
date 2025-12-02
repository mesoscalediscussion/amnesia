#ifndef HPL_LOWLEVELGRAPHICSSDLGPU_H
#define HPL_LOWLEVELGRAPHICSSDLGPU_H

#include <SDL3/SDL.h>

#include "graphics/LowLevelGraphics.h"
#include "impl/LowLevelGraphicsSDL.h"
#include "math/MathTypes.h"

#include "graphics/Texture.h"
#include "graphics/GPUShader.h"
#include "graphics/VertexBuffer.h"
#include "graphics/FontData.h"
#include "graphics/GPUProgram.h"
#include "graphics/FrameBuffer.h"
#include "graphics/OcclusionQuery.h"

namespace hpl {
	//-------------------------------------------------

	class cTextureSDLGpu : public iTexture {
	public:
		cTextureSDLGpu(const tString& asName, const tWString& asFullPath, eTextureType aType, eTextureUsage aUsage, iLowLevelGraphics* apLowLevelGraphics)
			: iTexture(asName, asFullPath, aType, aUsage, apLowLevelGraphics) {}
		~cTextureSDLGpu() override {}

		bool CreateFromBitmap(cBitmap* pBmp) override { return true; }
		bool CreateAnimFromBitmapVec(std::vector<cBitmap*> *avBitmaps) override { return true; }
		bool CreateCubeFromBitmapVec(std::vector<cBitmap*> *avBitmaps) override { return true; }
		bool CreateFromRawData(const cVector3l &avSize,ePixelFormat aPixelFormat, unsigned char *apData) override { return true; }
		
		void SetRawData(int alLevel, const cVector3l& avOffset, const cVector3l& avSize,
						ePixelFormat aPixelFormat, void *apData) override { }

		void Update(float afTimeStep) override { }
		
		void SetFilter(eTextureFilter aFilter) override { }
		void SetAnisotropyDegree(float afX) override { }
		
		void SetWrapS(eTextureWrap aMode) override { }
		void SetWrapT(eTextureWrap aMode) override { }
		void SetWrapR(eTextureWrap aMode) override { }
		void SetWrapSTR(eTextureWrap aMode) override { }
		
		void SetCompareMode(eTextureCompareMode aMode) override { }
		void SetCompareFunc(eTextureCompareFunc aFunc) override { }
		
		void AutoGenerateMipmaps() override { }
		
		bool HasAnimation() override { return false; }
		void NextFrame() override { }
		void PrevFrame() override { }
		float GetT() override { return 0; }
		float GetTimeCount() override { return 0; }
		void SetTimeCount(float afX) override { }
		int GetCurrentLowlevelHandle() override { return 0; }
	};

	class cGpuShaderSDLGpu : public iGpuShader {
	public:
		cGpuShaderSDLGpu(const tString& asName, const tWString& asFullPath, eGpuShaderType aType, eGpuProgramFormat aProgramFormat)
			: iGpuShader(asName, asFullPath, aType, aProgramFormat) { }
		~cGpuShaderSDLGpu() override = default;

		bool Reload() override { return true; }
		void Unload() override { }
		void Destroy() override { }

		bool SamplerNeedsTextureUnitSetup() override { return false; };
		bool CreateFromFile(const tWString& asFile, const tString& asEntry="main", bool abPrintInfoIfFail=true) override {
			return true;
		}
		bool CreateFromString(const char *apStringData, const tString& asEntry="main", bool abPrintInfoIfFail=true) override {
			return true;
		}
	};

	class cVertexBufferSDLGpu : public iVertexBuffer {
	public:
		cVertexBufferSDLGpu(iLowLevelGraphics* apLowLevelGraphics,
			eVertexBufferType aType,
			eVertexBufferDrawType aDrawType,eVertexBufferUsageType aUsageType,
			int alReserveVtxSize,int alReserveIdxSize)
				: iVertexBuffer(apLowLevelGraphics, aType, aDrawType, aUsageType, alReserveVtxSize, alReserveIdxSize) {}
		~cVertexBufferSDLGpu() override = default;
		
		void CreateElementArray(	eVertexBufferElement aType, eVertexBufferElementFormat aFormat,
									int alElementNum, int alProgramVarIndex=0) override { }
		
		void AddVertexVec3f(eVertexBufferElement aElement,const cVector3f& avVtx) override { }
		void AddVertexVec4f(eVertexBufferElement aElement,const cVector3f& avVtx, float afW) override { }
		void AddVertexColor(eVertexBufferElement aElement,const cColor& aColor) override { }
		void AddIndex(unsigned int alIndex) override { }
		
		bool Compile(tVertexCompileFlag aFlags) override { return true; }
		void UpdateData(tVertexElementFlag aTypes, bool abIndices) override { }
		
		void CreateShadowDouble(bool abUpdateData) override { }
		
		void Transform(const cMatrixf &mtxTransform) override { }

		void Draw(eVertexBufferDrawType aDrawType = eVertexBufferDrawType_LastEnum) override { }
		void DrawIndices(	unsigned int *apIndices, int alCount,
							eVertexBufferDrawType aDrawType = eVertexBufferDrawType_LastEnum) override { }

		void Bind() override { }
		void UnBind() override { }

        iVertexBuffer* CreateCopy(	eVertexBufferType aType,eVertexBufferUsageType aUsageType,
									tVertexElementFlag alVtxToCopy) override {
			return nullptr;
		}
		
        cBoundingVolume CreateBoundingVolume() override {
			return cBoundingVolume();
		}

        int GetElementNum(eVertexBufferElement aElement) override { return 0; }
		eVertexBufferElementFormat GetElementFormat(eVertexBufferElement aElement) override {
			return eVertexBufferElementFormat_LastEnum;
		}
		int GetElementProgramVarIndex(eVertexBufferElement aElement) override { return 0; }
		float* GetFloatArray(eVertexBufferElement aElement) override { return nullptr; }
		int* GetIntArray(eVertexBufferElement aElement) override { return nullptr; }
		unsigned char* GetByteArray(eVertexBufferElement aElement) override { return nullptr; }
		
		unsigned int* GetIndices() override { return nullptr; }
		
		int GetVertexNum() override { return 0; }
		int GetIndexNum() override { return 0; }
		
		void ResizeArray(eVertexBufferElement aElement, int alSize) override { }
		void ResizeIndices(int alSize) override { }
	};

	class cGpuProgramSDLGpu : public iGpuProgram {
	public:
		cGpuProgramSDLGpu(const tString& asName,eGpuProgramFormat aProgramFormat)
			: iGpuProgram(asName, aProgramFormat) {}
		~cGpuProgramSDLGpu() override = default;

		bool Link() override { return true; }

		void Bind()  override { }
		void UnBind()  override { }

		bool CanAccessAPIMatrix() override { return true; }
		
		bool SetSamplerToUnit(const tString& asSamplerName, int alUnit) override { return true; }

		int GetVariableId(const tString& asName) override { return 0; }
		bool GetVariableAsId(const tString& asName, int alId) override { return true; }
		
		bool SetInt(int alVarId, int alX) override { return true; }
		bool SetFloat(int alVarId, float afX) override { return true; }
		bool SetVec2f(int alVarId, float afX,float afY) override { return true; }
		bool SetVec3f(int alVarId, float afX,float afY,float afZ) override { return true; }
		bool SetVec4f(int alVarId, float afX,float afY,float afZ, float afW) override { return true; }
		bool SetMatrixf(int alVarId, const cMatrixf& mMtx) override { return true; }
		bool SetMatrixf(int alVarId, eGpuShaderMatrix mType, eGpuShaderMatrixOp mOp) override { return true; }
	};

	class cFrameBufferSDLGpu : public iFrameBuffer {
	public:
		cFrameBufferSDLGpu(const tString& asName, iLowLevelGraphics* apLowLevelGraphics)
			: iFrameBuffer(asName, apLowLevelGraphics) {}

		void SetTexture2D(int alColorIdx, iTexture *apTexture, int alMipmapLevel=0) override {}
		void SetTexture3D(int alColorIdx, iTexture *apTexture, int alZ, int alMipmapLevel=0) override {}
		void SetTextureCubeMap(int alColorIdx, iTexture *apTexture, int alFace, int alMipmapLevel=0) override {}

		void SetDepthTexture2D(iTexture *apTexture, int alMipmapLevel=0) override {}
		void SetDepthTextureCubeMap(iTexture *apTexture, int alFace, int alMipmapLevel=0) override {}

		void SetDepthStencilBuffer(iDepthStencilBuffer* apBuffer) override {}

		bool CompileAndValidate() override { return true; }

		void PostBindUpdate() override { }
	};

	class cDepthStencilBufferSDLGpu : public iDepthStencilBuffer {
	public:
		cDepthStencilBufferSDLGpu(const cVector2l& avSize, int alDepthBits, int alStencilBits)
			: iDepthStencilBuffer(avSize, alDepthBits, alStencilBits) {}
	};

	class cOcclusionQuerySDLGpu : public iOcclusionQuery {
	public:
		cOcclusionQuerySDLGpu() = default;
		~cOcclusionQuerySDLGpu() override = default;

		void Begin() override {}
		void End() override {}
		bool FetchResults() override { return true; }
		unsigned int GetSampleCount() override { return 1; }
	};

	//-------------------------------------------------

	//////////////////////////////////////////
	// Global Functions
	//////////////////////////////////////////

	//-------------------------------------------------

	class cLowLevelGraphicsSDLGpu : public cLowLevelGraphicsSDL
	{
	public:
		cLowLevelGraphicsSDLGpu();
		~cLowLevelGraphicsSDLGpu();

		/////////////////////////////////////////////////////
		/////////////// GENERAL SETUP ///////////////////////
		/////////////////////////////////////////////////////

		bool Init(	int alWidth, int alHeight, int alDisplay, int alBpp, int abFullscreen, int alMultisampling,
					eGpuProgramFormat aGpuProgramFormat,const tString& asWindowCaption,
					const cVector2l &avWindowPos) override;

		int GetCaps(eGraphicCaps aType);

		virtual void SetVsyncActive(bool abX, bool abAdaptive);

		virtual void SetMultisamplingActive(bool abX);

		virtual void SetGammaCorrection(float afX);

		/////////////////////////////////////////////////////
		/////////////// DATA CREATION //////////////////////
		/////////////////////////////////////////////////////

		iFontData* CreateFontData(const tString &asName);

		iTexture* CreateTexture(const tString &asName, eTextureType aType, eTextureUsage aUsage);

		iVertexBuffer* CreateVertexBuffer(	eVertexBufferType aType,
											eVertexBufferDrawType aDrawType,
											eVertexBufferUsageType aUsageType,
											int alReserveVtxSize=0,int alReserveIdxSize=0);

		iGpuProgram* CreateGpuProgram(const tString& asName);
		iGpuShader* CreateGpuShader(const tString& asName, eGpuShaderType aType);

		iFrameBuffer* CreateFrameBuffer(const tString& asName);
		iDepthStencilBuffer* CreateDepthStencilBuffer(const cVector2l& avSize, int alDepthBits, int alStencilBits);

		iOcclusionQuery* CreateOcclusionQuery();

		/////////////////////////////////////////////////////
		/////////// FRAME BUFFER OPERATIONS ///////
		/////////////////////////////////////////////////////

		void ClearFrameBuffer(tClearFrameBufferFlag aFlags);

		void SetClearColor(const cColor& aCol);
		void SetClearDepth(float afDepth);
		void SetClearStencil(int alVal);

		void CopyFrameBufferToTexure(	iTexture* apTex, const cVector2l &avPos,
									const cVector2l &avSize, const cVector2l &avTexOffset=0);
		cBitmap* CopyFrameBufferToBitmap(const cVector2l &avScreenPos=0, const cVector2l &avScreenSize=-1);

		void WaitAndFinishRendering();
		void FlushRendering();
		void SwapBuffers();

		void SetCurrentFrameBuffer(iFrameBuffer* apFrameBuffer, const cVector2l &avPos = 0, const cVector2l& avSize = -1);
		iFrameBuffer* GetCurrentFrameBuffer();

		void SetFrameBufferDrawTargets(int *apTargets, int alNumOfTargets);

		/////////////////////////////////////////////////////
		/////////// RENDER STATE ////////////////////////////
		/////////////////////////////////////////////////////

		void SetColorWriteActive(bool abR,bool abG,bool abB,bool abA);
		void SetDepthWriteActive(bool abX);

		void SetCullActive(bool abX);
		void SetCullMode(eCullMode aMode);

		void SetDepthTestActive(bool abX);
		void SetDepthTestFunc(eDepthTestFunc aFunc);

		void SetAlphaTestActive(bool abX);
		void SetAlphaTestFunc(eAlphaTestFunc aFunc,float afRef);

		void SetStencilActive(bool abX);
		void SetStencilWriteMask(unsigned int alMask);
		void SetStencil(eStencilFunc aFunc,int alRef, unsigned int aMask,
						eStencilOp aFailOp,eStencilOp aZFailOp,eStencilOp aZPassOp);
		void SetStencilTwoSide(	eStencilFunc aFrontFunc,eStencilFunc aBackFunc,
								int alRef, unsigned int aMask,
								eStencilOp aFrontFailOp,eStencilOp aFrontZFailOp,eStencilOp aFrontZPassOp,
								eStencilOp aBackFailOp,eStencilOp aBackZFailOp,eStencilOp aBackZPassOp);

		void SetScissorActive(bool abX);
		void SetScissorRect(const cVector2l& avPos, const cVector2l& avSize);

		void SetClipPlane(int alIdx, const cPlanef& aPlane);
		cPlanef GetClipPlane(int alIdx);
		void SetClipPlaneActive(int alIdx, bool abX);

		void SetColor(const cColor &aColor);

		void SetBlendActive(bool abX);
		void SetBlendFunc(eBlendFunc aSrcFactor, eBlendFunc aDestFactor);
		void SetBlendFuncSeparate(	eBlendFunc aSrcFactorColor, eBlendFunc aDestFactorColor,
									eBlendFunc aSrcFactorAlpha, eBlendFunc aDestFactorAlpha);

		void SetPolygonOffsetActive(bool abX);
		void SetPolygonOffset(float afBias,float afSlopeScaleBias);

		/////////////////////////////////////////////////////
		/////////// MATRIX //////////////////////////////////
		/////////////////////////////////////////////////////

		void PushMatrix(eMatrix aMtxType);
		void PopMatrix(eMatrix aMtxType);
		void SetIdentityMatrix(eMatrix aMtxType);

		void SetMatrix(eMatrix aMtxType, const cMatrixf& a_mtxA);

		void SetOrthoProjection(const cVector2f& avSize, float afMin, float afMax);
		void SetOrthoProjection(const cVector3f& avMin, const cVector3f& avMax);

		/////////////////////////////////////////////////////
		/////////// TEXTURE OPERATIONS ///////////////////////
		/////////////////////////////////////////////////////

		void SetTexture(unsigned int alUnit,iTexture* apTex);
		void SetActiveTextureUnit(unsigned int alUnit);
		void SetTextureEnv(eTextureParam aParam, int alVal);
		void SetTextureConstantColor(const cColor &aColor);


		/////////////////////////////////////////////////////
		/////////// DRAWING ///////////////////////////////
		/////////////////////////////////////////////////////

		void DrawTriangle(tVertexVec& avVtx);

		void DrawQuad(	const cVector3f &avPos,const cVector2f &avSize, const cColor& aColor=cColor(1,1));
		void DrawQuad(	const cVector3f &avPos,const cVector2f &avSize,
						const cVector2f &avMinTexCoord,const cVector2f &avMaxTexCoord,
						const cColor& aColor=cColor(1,1));
		void DrawQuad(	const cVector3f &avPos,const cVector2f &avSize,
						const cVector2f &avMinTexCoord0,const cVector2f &avMaxTexCoord0,
						const cVector2f &avMinTexCoord1,const cVector2f &avMaxTexCoord1,
						const cColor& aColor=cColor(1,1));

		void DrawQuad(const tVertexVec &avVtx);
		void DrawQuad(const tVertexVec &avVtx, const cColor aCol);
		void DrawQuad(const tVertexVec &avVtx,const float afZ);
		void DrawQuad(const tVertexVec &avVtx,const float afZ,const cColor &aCol);
		void DrawQuadMultiTex(const tVertexVec &avVtx,const tVector3fVec &avExtraUvs);

		void DrawLine(const cVector3f& avBegin, const cVector3f& avEnd, cColor aCol);
		void DrawLine(const cVector3f& avBegin, const cColor& aBeginCol, const cVector3f& avEnd, const cColor& aEndCol);

		void DrawBoxMinMax(const cVector3f& avMin, const cVector3f& avMax, cColor aCol);
		void DrawSphere(const cVector3f& avPos, float afRadius, cColor aCol);
		void DrawSphere(const cVector3f& avPos, float afRadius, cColor aColX, cColor aColY, cColor aColZ);

		void DrawLineQuad(const cRect2f& aRect, float afZ, cColor aCol);
		void DrawLineQuad(const cVector3f &avPos,const cVector2f &avSize, cColor aCol);

		/////////////////////////////////////////////////////
		/////////// VERTEX BATCHING /////////////////////////
		/////////////////////////////////////////////////////

		void AddVertexToBatch(const cVertex *apVtx);
		void AddVertexToBatch(const cVertex *apVtx, const cVector3f* avTransform);
		void AddVertexToBatch(const cVertex *apVtx, const cMatrixf* aMtx);

		void AddVertexToBatch_Size2D(const cVertex *apVtx, const cVector3f* avTransform,
										const cColor* apCol,const float& mfW, const float& mfH);

		void AddVertexToBatch_Raw(	const cVector3f& avPos, const cColor &aColor,
									const cVector3f& avTex);


		void AddTexCoordToBatch(unsigned int alUnit,const cVector3f *apCoord);
		void SetBatchTextureUnitActive(unsigned int alUnit,bool abActive);

		void AddIndexToBatch(int alIndex);

		void FlushTriBatch(tVtxBatchFlag aTypeFlags, bool abAutoClear=true);
		void FlushQuadBatch(tVtxBatchFlag aTypeFlags, bool abAutoClear=true);
		void ClearBatch();
	private:
		// . .

		iFrameBuffer* mCurrentFrameBuffer;
	};
};

#endif // HPL_LOWLEVELGRAPHICSSDLGPU_H
