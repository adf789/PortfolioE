// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem_Equipment.h"

UInventoryItem_Equipment::UInventoryItem_Equipment() {

}

void UInventoryItem_Equipment::Use() {
	TEST_LOG_WITH_VAR("Use: %s", *GetDisplayName().ToString());
}


