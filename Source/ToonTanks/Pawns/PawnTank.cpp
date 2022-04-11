// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTurret.h"
#include "ToonTanks/Components/HealthComponent.h"


APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // SpringArm->SetupAttachment(TurretMesh);
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());

    AActor* TurretClass = UGameplayStatics::GetActorOfClass(GetWorld(), APawnTurret::StaticClass());
    TurretActor = Cast<APawnTurret>(TurretClass);

    GetWorldTimerManager().SetTimer(RepairTimerHandle, this, &APawnTank::RepairTank, HealingRate, true);
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    Rotate();
    Move();

    // UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComponent->Health)
    if(PlayerControllerRef)
    {
        FHitResult TraceHitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurret(HitLocation);
    }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
    // PlayerInputComponent->BindAxis("RotateTurret", this, &APawnTank::RotateView);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value) 
{
    MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0); //FVector(float InX, float InY, float InZ);
}

void APawnTank::CalculateRotateInput(float Value) 
{
    float YawRotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds; //Yaw - movement in horizontal plane
    FRotator Rotation = FRotator(0, YawRotateAmount, 0); //FRotator( float InPitch, float InYaw, float InRoll );
    // CounterRotation = FRotator(0, -YawRotateAmount, 0);
    RotationDirection = FQuat(Rotation); 
}

// void APawnTank::RotateView(float Input)
// {
//     FRotator Rotation = TurretMesh->GetRelativeRotation();

//     Rotation.Yaw += Input * RotateSpeed * GetWorld()->DeltaTimeSeconds;
//     TurretMesh->SetRelativeRotation(Rotation);
// }

void APawnTank::Move() 
{
    AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate() 
{
    AddActorLocalRotation(RotationDirection, true);
    // TurretMesh->AddLocalRotation(CounterRotation, false);
}

void APawnTank::HandleDestruction() 
{
    Super::HandleDestruction();
    UE_LOG(LogTemp, Warning, TEXT("A tank died"));
    //Hide Player, Todo- Create new function to handle this.
    bIsPlayerAlive = false;
    
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    // Destroy();
}

void APawnTank::RepairTank()
{
    TankHealth = HealthComponent->Health;
    if(TurretActor && GetTankHealth() <= 180.f)
    {
        TurretsInRangeCount = TurretActor->GetTurretsInRangeCount();
        UE_LOG(LogTemp, Warning, TEXT("countinrange: %d"), TurretsInRangeCount);
        if(TurretsInRangeCount == 0)
        {
            TankHealth += 20.f;
            SetTankHealth(TankHealth);
        }
    }
}

bool APawnTank::GetIsPlayerAlive() 
{
    return bIsPlayerAlive;
}

float APawnTank::GetTankHealth()
{
    return HealthComponent->Health;
}

void APawnTank::SetTankHealth(float NewHealth)
{
    HealthComponent->Health = NewHealth;
}