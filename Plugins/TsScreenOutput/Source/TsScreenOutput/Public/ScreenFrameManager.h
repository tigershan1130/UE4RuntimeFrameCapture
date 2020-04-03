// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime\Engine\Classes\Engine\TextureRenderTarget2D.h>
#include <Runtime\Engine\Classes\Components\SceneCaptureComponent2D.h>
#include "ScreenFrameManager.generated.h"


UCLASS()
class TSSCREENOUTPUT_API AScreenFrameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScreenFrameManager();


protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	int32 Width;
	
	int32 Height;

	bool isHDR = false;

	int32 FOV;

	UTextureRenderTarget2D* RenderTarget;

	USceneCaptureComponent2D* SceneCapture;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void InitializeCapture(int32 width, int32 height, int32 fieldOfView, bool makeHDR);

	UFUNCTION(BlueprintCallable)
	bool SaveRenderTarget(FString path, FString fileName);

	UFUNCTION(BlueprintCallable)	
	UTextureRenderTarget2D* CreateRenderTarget2D();
};
