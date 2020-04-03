// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "UniformBuffer.h"
#include "RHICommandList.h"

/**
 * 
 */
class TSSCREENOUTPUT_API FrameScreenCapture
{
public:

	FrameScreenCapture(ERHIFeatureLevel::Type ShaderFeatureLevel);

	~FrameScreenCapture();

	//This method will execute the specific pixel shader implementation of a given class 
	virtual void RenderMyTest(FRHICommandList& RHICmdList, ERHIFeatureLevel::Type FeatureLevel, const FLinearColor& Color)
	{


	}
};
