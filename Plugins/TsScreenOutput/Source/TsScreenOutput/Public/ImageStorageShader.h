#pragma once
#include "GlobalShader.h"
#include <Runtime\RenderCore\Public\ShaderParameterUtils.h>

class FImageStorageShaderVS : public FGlobalShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FImageStorageShaderVS, Global, /*MyMODULE_API*/);

	FImageStorageShaderVS()
	{

	}

	FImageStorageShaderVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer)
	{

	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

};

// This needs to go on a cpp file
IMPLEMENT_SHADER_TYPE(, FImageStorageShaderVS, TEXT("ImageStorage"), TEXT("MainVS"), SF_Vertex);

class FImageStorageShaderPS : public FGlobalShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FImageStorageShaderPS, Global, /*MYMODULE_API*/);

	FShaderParameter MyColorParameter;

	FImageStorageShaderPS() {}

	FImageStorageShaderPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer)
	{
		MyColorParameter.Bind(Initializer.ParameterMap, TEXT("MyColor"), SPF_Mandatory);
	}

	static void ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Platform, OutEnvironment);
		// Add your own defines for the shader code
		OutEnvironment.SetDefine(TEXT("MY_DEFINE"), 1);
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		// Could skip compiling for Platform == SP_METAL for example
		return true;
	}

	// FShader interface.
	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << MyColorParameter;
		return bShaderHasOutdatedParameters;
	}

	void SetColor(FRHICommandList& RHICmdList, const FLinearColor& Color)
	{
		SetShaderValue(RHICmdList, GetPixelShader(), MyColorParameter, Color);
	}


};

// Same source file as before, different entry point
IMPLEMENT_SHADER_TYPE(, FMyTestPS, TEXT("ImageStorage"), TEXT("MainPS"), SF_Pixel);