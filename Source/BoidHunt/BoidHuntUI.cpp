// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidHuntUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"



void UBoidHuntUI::SetBoids(int Number)
{
    if (BoidCounter)
	    BoidCounter->SetText(FText::AsNumber(Number));
}

void UBoidHuntUI::SetFalcons(int Number)
{
    if (FalconCounter)
	    FalconCounter->SetText(FText::AsNumber(Number));
}

void UBoidHuntUI::ShowVictoryText()
{
	VictoryText->SetVisibility(ESlateVisibility::Visible);
}

void UBoidHuntUI::SetFuel(double JetpackFuel, double MaxJetpackFuel)
{
	FuelBar->SetPercent(JetpackFuel / MaxJetpackFuel);
}

void UBoidHuntUI::SetAmmo(int Ammo)
{
	AmmoCounter->SetText(FText::AsNumber(Ammo));
}
