// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem_Etc.h"

UInventoryItem_Etc::UInventoryItem_Etc() {

}

void UInventoryItem_Etc::Use() {
	TEST_LOG_WITH_VAR("Use: %s", *GetDisplayName().ToString());
}
