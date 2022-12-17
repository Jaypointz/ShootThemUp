// Shoot Them Up Game, All Rights Reserved.

#include "AI/STUAICharacter.h"
#include "BrainComponent.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUAIWeaponComponent.h"
#include "STUGameModeBase.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();

     if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());

        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUAICharacter::OnMatchStateChanged);
        }
    }
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();

    CleanupAI();
}

void ASTUAICharacter::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver)
    {
        CleanupAI();
    }
}

void ASTUAICharacter::CleanupAI()
{
    const auto STUController = Cast<AAIController>(Controller);

    if (STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}
