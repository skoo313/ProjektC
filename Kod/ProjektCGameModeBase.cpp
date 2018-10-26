// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjektCGameModeBase.h"

void AProjektCGameModeBase::StartPlay()
{
    Super::StartPlay();

    if (GEngine)
    {
        //The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MAP_01"));
    }
}


