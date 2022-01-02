// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "POEGameVariables.h"

DECLARE_LOG_CATEGORY_EXTERN(POE, Log, All);

#define CHECKRETURN(expr, ...) {if(expr) {UE_LOG(POE, Error, TEXT("%s : %s"), *CALL_INFO, TEXT("'"#expr"'")); return __VA_ARGS__;}}
#define CALL_INFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define TEST_LOG(expr) UE_LOG(POE, Warning, TEXT(#expr))
#define TEST_LOG_WITH_VAR(expr, ...) UE_LOG(POE, Warning, TEXT(#expr), ##__VA_ARGS__)