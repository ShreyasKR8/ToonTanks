// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h" //For timer functionality.
#include "PawnTank.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(!PlayerPawn)
    {
        return;
    }
	if(ReturnDistanceToPlayer() <= FireRange)
	{
	    RotateTurret(PlayerPawn->GetActorLocation());  //RotateTurret(FVector LookAtTarget) defined in PawnBase.cpp
		return;
	}
}

void APawnTurret::HandleDestruction() 
{
	Super::HandleDestruction();

    Destroy();

	UE_LOG(LogTemp, Warning, TEXT("A turret died"));
}

void APawnTurret::CheckFireCondition() 
{
	if(!PlayerPawn || !(PlayerPawn->GetIsPlayerAlive()))
	{
		return;
	}
	//If Player is in range, then Fire!
	if(ReturnDistanceToPlayer() <= FireRange)
	{
		Fire();
	}
}

float APawnTurret::ReturnDistanceToPlayer() 
{
	if(!PlayerPawn)
	{
		return 0.0f;
	}

	float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
	return Distance;
}

int32 APawnTurret::GetTurretsInRangeCount()
{
    TurretsInRangeCount = 0;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), Turrets);
    for(int32 Index = 0; Index < Turrets.Num(); ++Index)
    {
        TurretActor = Cast<APawnTurret>(Turrets[Index]);
        if(TurretActor->ReturnDistanceToPlayer() <= TurretActor->FireRange)
        {
            TurretsInRangeCount++;
        }
    }
    return TurretsInRangeCount;
}