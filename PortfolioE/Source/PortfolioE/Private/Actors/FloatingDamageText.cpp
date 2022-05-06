// Description
// 데미지 애니메이션 클래스

#include "FloatingDamageText.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "POEGameInstance.h"
#include "ActorObjectPool.h"

void AFloatingDamageText::PostInitializeComponents() {
	Super::PostInitializeComponents();
	UWidgetComponent* FloatingDamage = Cast<UWidgetComponent>(GetDefaultSubobjectByName(TEXT("DamageText")));
	if (FloatingDamage != nullptr) {
		UserWidget = FloatingDamage->GetUserWidgetObject();
		if (UserWidget != nullptr) {
			FloatingDamageText = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TEXT("FloatingDamageText")));
		}

		UWidgetBlueprintGeneratedClass* WidgetClass = Cast< UWidgetBlueprintGeneratedClass>(UserWidget->GetClass());

		for (UWidgetAnimation* animation : WidgetClass->Animations) {
			if (animation->GetName().Equals(TEXT("Fadeout_INST"))) {
				FloatingDamageAnim = animation;
				break;
			}
		}

		if (FloatingDamageAnim != nullptr) {
			FloatingDamageAnim->OnAnimationFinished.AddDynamic(this, &AFloatingDamageText::InActive);
		}
	}
}

void AFloatingDamageText::InActive()
{
	Super::InActive();

	UPOEGameInstance* POEGameInstance = Cast<UPOEGameInstance>(GetGameInstance());
	CHECKRETURN(POEGameInstance == nullptr);
	POEGameInstance->DamageTextPooling->AddObject(this);
}

void AFloatingDamageText::ShowDamage(float Damage) {
	if (!::IsValid(FloatingDamageAnim) || FloatingDamageAnim == nullptr) {
		TEST_LOG("FloatingDamageAnim not valid");
		return;
	}
	else if (!::IsValid(FloatingDamageText) || FloatingDamageText == nullptr) {
		TEST_LOG("FloatingDamageText not valid");
		return;
	}
	if (!::IsValid(UserWidget) || UserWidget == nullptr) {
		TEST_LOG("UserWidget not valid");
		return;
	}

	FloatingDamageText->SetText(FText::AsNumber(FMath::RoundToInt(Damage)));
	UserWidget->PlayAnimation(FloatingDamageAnim);
}

