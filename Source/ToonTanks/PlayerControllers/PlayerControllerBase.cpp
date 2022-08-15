// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "GameFramework/PlayerController.h"


void APlayerControllerBase::SetPlayerEnabledState(bool SetPlayerEnabled) 
{
    if(SetPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
        // GetWorldTimerManager().SetTimer(FTimer, this, &APlayerController::RestartLevel, 3.f);
    }

    bShowMouseCursor = SetPlayerEnabled;
}

void APlayerControllerBase::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    FTimerHandle RestartTimer;
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, 3.f);
    
}
