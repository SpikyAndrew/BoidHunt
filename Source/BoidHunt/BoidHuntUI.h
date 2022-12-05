// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoidHuntUI.generated.h"

/**
 * 
 */
UCLASS()
class BOIDHUNT_API UBoidHuntUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetBoids(int Number);
	void SetFalcons(int Number);
	void ShowVictoryText();
	
private:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* BoidCounter;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* FalconCounter;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* VictoryText;
};
