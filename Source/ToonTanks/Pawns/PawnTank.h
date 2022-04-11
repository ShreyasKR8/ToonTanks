// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class APawnTurret;

UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))	
	UCameraComponent* Camera;

	FVector MoveDirection;

	FQuat RotationDirection;

    FTimerHandle RepairTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float HealingRate = 2.0f;

	UPROPERTY(VisibleAnywhere)
    APawnTurret* TurretActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 100.0f;

    UPROPERTY(EditAnywhere)
    int32 TurretsInRangeCount = 0;

    UPROPERTY(VisibleAnywhere)
    float TankHealth;

	UPROPERTY(VisibleAnywhere)	
	APlayerController* PlayerControllerRef;

	bool bIsPlayerAlive = true;

	void CalculateMoveInput(float Value);

    void RotateView(float Input);

	void CalculateRotateInput(float Value);

	void Move();

	void Rotate();

    void RepairTank();

public: 
	APawnTank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void HandleDestruction() override;

	bool GetIsPlayerAlive();

    float GetTankHealth();

    void SetTankHealth(float NewHealth); 
};
