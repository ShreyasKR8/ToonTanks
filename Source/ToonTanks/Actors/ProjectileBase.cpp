// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

	float InitialLifespan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay() 
{
	Super::BeginPlay();
	
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{

	AActor* MyOwner = GetOwner();
	if(!MyOwner)
	{
		return;
	}

	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
        if(HitParticle)
        {
		    UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
        }
        if(HitSound)
        {
		    UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
        }
        if(HitCameraShakeClass)
        {
            //UE 4.25 - ClientPlayCameraShake; UE 4.26+ - ClientStartCameraShake;
		    GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HitCameraShakeClass);
        }

		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Damage applied"));
	}
}
