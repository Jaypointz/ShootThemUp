// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUPlayerHUDWidget.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return 0.0f;

    const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    const auto HealthComponent = Cast<USTUHealthComponent>(Component);

    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetAmmoData(AmmoData);
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return nullptr;

    const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());

    return Cast<USTUWeaponComponent>(Component);
}
