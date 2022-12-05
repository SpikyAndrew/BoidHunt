// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidHuntHUD.h"

#include "BoidHuntGameState.h"
#include "Blueprint/UserWidget.h"



void ABoidHuntHUD::BeginPlay()
{
	Super::BeginPlay();
	if (UIClass)
	{
		UI = CreateWidget<UBoidHuntUI>(GetWorld(), UIClass);
		UI->AddToPlayerScreen();
		if (UWorld* World = GetWorld())
		{
			if (ABoidHuntGameState* GameState = Cast<ABoidHuntGameState>(World->GetGameState()))
			{
				UI->SetBoids(GameState->GetNumberOfBoids());
				UI->SetFalcons(GameState->GetNumberOfFalcons());
			}

		}
	}
}

void ABoidHuntHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (UI)
	{
		UI->RemoveFromParent();
		UI = nullptr;
	}
}

void ABoidHuntHUD::ShowVictoryText()
{
	UI->ShowVictoryText();
}


void ABoidHuntHUD::SetBoids(int Number)
{
	if (UI)
		UI->SetBoids(Number);
}

void ABoidHuntHUD::SetFalcons(int Number)
{
	if (UI)
		UI->SetFalcons(Number);
}