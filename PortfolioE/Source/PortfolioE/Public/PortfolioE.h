// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(POE, Log, All);

#define CHECKRETURN(expr, ...) {if(expr) {UE_LOG(POE, Error, TEXT("'"#expr"'")); return __VA_ARGS__;}}