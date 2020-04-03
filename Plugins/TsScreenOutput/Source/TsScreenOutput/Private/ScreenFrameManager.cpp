// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenFrameManager.h"
#include <Runtime\Engine\Public\ImageUtils.h>
#include <Runtime\Core\Public\Misc\FileHelper.h>
#include <Runtime\Engine\Classes\Camera\PlayerCameraManager.h>
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/SceneCapture2D.h"
#include <string>


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

// Sets default values
AScreenFrameManager::AScreenFrameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

UTextureRenderTarget2D* AScreenFrameManager::CreateRenderTarget2D()
{
	UTextureRenderTarget2D* renderTarget = NewObject<UTextureRenderTarget2D>();


	if (isHDR)
		renderTarget->InitAutoFormat(Width, Height);
	else
		renderTarget->InitCustomFormat(Width, Height, PF_B8G8R8A8, false);

	return renderTarget;
}

void AScreenFrameManager::InitializeCapture(int32 width, int32 height, int32 fieldOfView, bool makeHDR)
{
	Width = width;
	Height = height;
	isHDR = makeHDR;
	FOV = fieldOfView;
}

bool AScreenFrameManager::SaveRenderTarget(FString path, FString fileName)
{
	if (RenderTarget == nullptr)
		RenderTarget = CreateRenderTarget2D();

	// attach uscenecapturecompnent2D to our camera manager
	if (SceneCapture == nullptr)
	{
		ASceneCapture2D* sceneCaptureActor = (ASceneCapture2D*)GetWorld()->SpawnActor<ASceneCapture2D>(ASceneCapture2D::StaticClass());
		SceneCapture = sceneCaptureActor->GetCaptureComponent2D();

		SceneCapture->ProjectionType = ECameraProjectionMode::Type::Perspective;
		SceneCapture->FOVAngle = 90.0f;
		SceneCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
		SceneCapture->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
		SceneCapture->bCaptureOnMovement = true;
		SceneCapture->bCaptureEveryFrame = true;
		SceneCapture->MaxViewDistanceOverride = -1.0f;
		SceneCapture->bAutoActivate = true;
		SceneCapture->DetailMode = EDetailMode::DM_High;
	}


	FTransform cameraTransform = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetRootComponent()->GetComponentTransform();
	SceneCapture->SetWorldTransform(cameraTransform);

	//FString isSceneCaptureEmpty = (SceneCapture != nullptr) ? " Not Empty " : "Empty";
	//print(isSceneCaptureEmpty);

	//RenderTarget->UpdateResourceImmediate();

	// capture scene
	SceneCapture->TextureTarget = RenderTarget;
	SceneCapture->CaptureScene();

	// set uscene's capture compoennt's FTextureRenderTargetResources.

	FTextureRenderTargetResource* resource = RenderTarget->GameThread_GetRenderTargetResource();

	FReadSurfaceDataFlags readPixelFlags(RCM_UNorm);

	TArray<FColor> outBMP;

	outBMP.AddUninitialized(RenderTarget->GetSurfaceWidth() *RenderTarget->GetSurfaceHeight());

	resource->ReadPixels(outBMP, readPixelFlags);

	//print(FString::FromInt(outBMP.Num() * 4));

	for (FColor& color : outBMP)
	{
		color.A = 255;
	}

	FIntPoint destSize(RenderTarget->GetSurfaceWidth(), RenderTarget->GetSurfaceHeight());

	TArray<uint8> compressedBitmap;

	FImageUtils::CompressImageArray(destSize.X, destSize.Y, outBMP, compressedBitmap);

	FString fullPath = FPaths::Combine(path, fileName);

	//print(fullPath);

	bool imageSavedOK = FFileHelper::SaveArrayToFile(compressedBitmap, *fullPath);

	return imageSavedOK;
}

