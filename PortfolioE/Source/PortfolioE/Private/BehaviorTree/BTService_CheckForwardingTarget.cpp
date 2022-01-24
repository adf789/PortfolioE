// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForwardingTarget.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_CheckForwardingTarget::UBTService_CheckForwardingTarget() {
	NodeName = TEXT("Check forwarding target");
}

void UBTService_CheckForwardingTarget::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* MyMonsterActor = OwnerComp.GetAIOwner()->GetPawn();
	if (MyMonsterActor == nullptr) return;

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APOEMonsterAIController::BBKEY_Target));
	if (Target == nullptr) return;

	FCollisionQueryParams Params(NAME_None, false, MyMonsterActor);
	FHitResult HitResult;
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		MyMonsterActor->GetActorLocation(),
		MyMonsterActor->GetActorForwardVector() * 300.0f + MyMonsterActor->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(5.0f),
		Params);


	if (bResult && !HitResult.GetActor()->GetInstigatorController()->IsPlayerController()) {
		int32 WaypointDirection = OwnerComp.GetBlackboardComponent()->GetValueAsInt(APOEMonsterAIController::BBKEY_SetWaypointDirection);
		if (WaypointDirection == 0) {
			FVector TargetDirection = HitResult.GetActor()->GetActorLocation() - MyMonsterActor->GetActorLocation();
			FVector CrossDirect = FVector::CrossProduct(MyMonsterActor->GetActorForwardVector(), TargetDirection);
			WaypointDirection = CrossDirect.Z >= 0? -1 : 1;
		}

		FVector TempWaypointLocation = MyMonsterActor->GetActorForwardVector().RotateAngleAxis(30.0f * WaypointDirection, MyMonsterActor->GetActorUpVector());

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(APOEMonsterAIController::BBKEY_WaypointLocation, MyMonsterActor->GetActorLocation() + TempWaypointLocation * 200.0f);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(APOEMonsterAIController::BBKEY_SetWaypointDirection, WaypointDirection);

#if ENABLE_DRAW_DEBUG
		DrawDebugCylinder(GetWorld(),
			MyMonsterActor->GetActorLocation(),
			MyMonsterActor->GetActorForwardVector() * 300.0f + MyMonsterActor->GetActorLocation(),
			5.0f,
			10,
			bResult ? FColor::Green : FColor::Red,
			false,
			0.1f);

		DrawDebugPoint(GetWorld(),
			MyMonsterActor->GetActorLocation() + TempWaypointLocation * 200.0f,
			10.0f,
			FColor::Green,
			false,
			0.1f);
#endif
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(APOEMonsterAIController::BBKEY_SetWaypointDirection, 0);
	}
}
