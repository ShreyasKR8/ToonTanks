// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

class APawnTank;
class APawnTurret;

UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f;

	FTimerHandle FireRateTimerHandle;

    UPROPERTY(VisibleAnywhere)
	APawnTank * PlayerPawn;

    UPROPERTY(VisibleAnywhere)
    APawnTurret* TurretActor;

    TArray<AActor*> Turrets;

    int32 TurretsInRangeCount = 0;

	void CheckFireCondition();

    void RepairTank();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRange = 500.0f;
    
	float ReturnDistanceToPlayer();

    int32 GetTurretsInRangeCount();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
