// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All)

constexpr ECollisionChannel ECC_Weapon = ECC_GameTraceChannel1;
constexpr ECollisionChannel ECC_LockOnTarget = ECC_GameTraceChannel2;