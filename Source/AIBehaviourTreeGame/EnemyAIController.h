// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ball.h"
#include "EnemyAIController.generated.h"


struct FAivState : public TSharedFromThis<FAivState>
{
private:
	TFunction<void(AAIController*)> Enter;
	TFunction<void(AAIController*)> Exit;
	TFunction<TSharedPtr<FAivState>(AAIController*, const float)> Tick;

public:

	

	FAivState()
	{
		Enter = nullptr;
		Exit = nullptr;
		Tick = nullptr;

	}

	FAivState(TFunction<void(AAIController*)> InEnter = nullptr, TFunction<void(AAIController*)> InExit = nullptr, TFunction<TSharedPtr<FAivState>(AAIController*, const float)> InTick = nullptr)
	{
		Enter = InEnter;
		Exit = InExit;
		Tick = InTick;

	}
	

	FAivState(const FAivState& Other) = delete;
	FAivState operator = (const FAivState& Other) = delete;
	FAivState(const FAivState&& Other) = delete;
	FAivState operator = (const FAivState&& Other) = delete;

	void CallEnter(AAIController* AIController)
	{
		if (Enter) 
		{
			Enter(AIController);
		}
	}

	void CallExit(AAIController* AIController)
	{
		if (Exit)
		{
			Exit(AIController);
		}
	}

	TSharedPtr<FAivState> CallTick(AAIController* AIController, const float DeltaTime)
	{
		if (Tick)
		{
			TSharedPtr<FAivState> NewState = Tick(AIController, DeltaTime);

			if (NewState != nullptr && NewState != AsShared())
			{
				CallExit(AIController);
				NewState->CallEnter(AIController);
				return NewState;
			}
		}

		return AsShared();
	}
};
/**
 * 
 */
UCLASS()

class AIBEHAVIOURTREEGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	bool bIsRunningAway;
	

protected:
	TSharedPtr<FAivState> CurrentState;
	TSharedPtr<FAivState> GoToPlayer;
	TSharedPtr<FAivState> GoToBall;
	TSharedPtr<FAivState> RunAway;
	TSharedPtr<FAivState> GrabBall;
	TSharedPtr<FAivState> SearchForBall;

	FTimerHandle RestartLevelTimer;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBlackboardData* BlackboardData;

	UPROPERTY(Transient)
	UBlackboardComponent* BlackboardComponent;

	ABall* BestBall;
	

	void SetBestBall(ABall* Ball);
	void ResetBestBall();
	void ShootPlayerInAir();
	void RestartLevel();
	void StartRestartLevelTimer();
	void IncrementDeliveredBallsToPlayer();
	void DetachSpecialBall(AAIController* AIController);
	void ShootEnemyInAir(AAIController* AIController);

};

