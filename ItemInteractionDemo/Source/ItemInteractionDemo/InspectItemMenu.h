// Ignacio Roca UE5 Item Interaction Demo
//
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InspectItemMenu.generated.h"

UCLASS()
class ITEMINTERACTIONDEMO_API UInspectItemMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* ItemActionHorizontalBox;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ItemActionText;
};
