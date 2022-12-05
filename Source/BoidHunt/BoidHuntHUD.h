// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidHuntCharacter.h"
#include "GameFramework/HUD.h"
#include "BoidHuntUI.h"
#include "BoidHuntHUD.generated.h"

// Used to hold the UMG UI widgets.
UCLASS()
class BOIDHUNT_API ABoidHuntHUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetBoids(int Number);
	void SetFalcons(int Number);
	void ShowVictoryText();
	void SetFuel(double JetpackFuel, double MaxJetpackFuel);
	void SetAmmo(int Ammo);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBoidHuntUI> UIClass;
	UPROPERTY()
	UBoidHuntUI* UI;
};
