// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    void BeginPlay() override;

    virtual void StartFire() override;
    virtual void StopFire() override;

    virtual void Zoom(bool Enabled) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float FOVZoomAngle = 50.0f;

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFX;

    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
    FTimerHandle ShotTimerHandle;

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    UPROPERTY()
    UAudioComponent* FireAudioComponent;

    void MakeDamage(const FHitResult& HitResult);

    void InitFX();
    void SetFXActive(bool IsActive);

    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

    AController* GetController() const;

    float DefaultCameraFOV = 90.0f;
};
