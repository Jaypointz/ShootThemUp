// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sections/MovieSceneParticleSection.h"
#include "STUUtils.h"
#include "Components/AudioComponent.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CollisionComponent->bReturnMaterialOnMove = true;
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;

    WeaponFX = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(MovementComponent);
    check(CollisionComponent);
    check(WeaponFX);
    check(ProjectileFX);

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);

    TraceFXComponent = SpawnNiagaraSystem(TraceFX);
    TraceFXComponent->OnSystemFinished.AddDynamic(this, &ASTUProjectile::OnTraceFXFinished);

    ProjectileFXComponent = SpawnNiagaraSystem(ProjectileFX);

    SetLifeSpan(LifeSeconds);
}

void ASTUProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld())
        return;

    MovementComponent->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(GetWorld(), //
        DamageAmount,                               //
        GetActorLocation(),                         //
        DamageRadius,                               //
        UDamageType::StaticClass(),                 //
        {GetOwner()},                               //
        this,                                       //
        GetController(),                            //
        DoFullDamage);

    // DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

    WeaponFX->PlayImpactFX(Hit);

    UAudioComponent* AudioComponent = STUUtils::GetSTUPlayerComponent<UAudioComponent>(this);

    STUUtils::DeactivateComponent(AudioComponent);
    STUUtils::DeactivateComponent(ProjectileFXComponent);
    STUUtils::DeactivateComponent(TraceFXComponent);

    UStaticMeshComponent* StaticMesh = STUUtils::GetSTUPlayerComponent<UStaticMeshComponent>(this);

    if (StaticMesh)
    {
        StaticMesh->SetVisibility(false);
    }
}

void ASTUProjectile::OnTraceFXFinished(UNiagaraComponent* NiagaraComponent)
{
    if (NiagaraComponent)
    {
        Destroy();
    }
}

AController* ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

UNiagaraComponent* ASTUProjectile::SpawnNiagaraSystem(UNiagaraSystem* NiagaraSystem) const
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, //
        GetRootComponent(),                                            //
        FName(),                                                       //
        FVector::ZeroVector,                                           //
        FRotator::ZeroRotator,                                         //
        EAttachLocation::SnapToTarget,                                 //
        true);
}

