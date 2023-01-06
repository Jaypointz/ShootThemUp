// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "STUHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    AActor* ClosestEnemyAtSight = FindBestPawn(UAISense_Sight::StaticClass());

    return ClosestEnemyAtSight ? ClosestEnemyAtSight : FindBestPawn(UAISense_Damage::StaticClass());
}

AActor* USTUAIPerceptionComponent::FindBestPawn(TSubclassOf<UAISense> Sense) const
{
    TArray<AActor*> PerceivedActors;

    GetCurrentlyPerceivedActors(Sense, PerceivedActors);
    if (PerceivedActors.Num() == 0)
        return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller)
        return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
        return nullptr;

    float BestDistance = MAX_FLT;

    AActor* BestPawn = nullptr;
    for (const auto PerceivedActor : PerceivedActors)
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PerceivedActor);

        const auto PerceivedPawn = Cast<APawn>(PerceivedActor);
        const auto AreEnemies = PerceivedPawn && STUUtils::AreEnemies(Controller, PerceivedPawn->GetController());

        if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (PerceivedActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PerceivedActor;
            }
        }
    }

    return BestPawn;
}
