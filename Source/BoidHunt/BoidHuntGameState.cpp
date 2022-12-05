// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidHuntGameState.h"

#include "BoidHuntHUD.h"

void ABoidHuntGameState::AddBoids(int Number)
{
	NumberOfBoids += Number;
	ABoidHuntHUD* HUD = Cast<ABoidHuntHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (HUD)
	{
		HUD->SetBoids(NumberOfBoids);
        if(NumberOfBoids < 1)
		    HUD->ShowVictoryText();
    }
}

void ABoidHuntGameState::AddFalcons(int Number)
{
	NumberOfFalcons += Number;
	ABoidHuntHUD* HUD = Cast<ABoidHuntHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (HUD)
		HUD->SetFalcons(NumberOfFalcons);
}

void ABoidHuntGameState::SetFuel(double JetpackFuel, double MaxJetpackFuel)
{
	ABoidHuntHUD* HUD = Cast<ABoidHuntHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (HUD)
		HUD->SetFuel(JetpackFuel, MaxJetpackFuel);
}

int ABoidHuntGameState::GetNumberOfBoids()
{
	return NumberOfBoids;
}

int ABoidHuntGameState::GetNumberOfFalcons()
{
	return NumberOfFalcons;
}