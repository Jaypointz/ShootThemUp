// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUHealthComponent.h"

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
}
