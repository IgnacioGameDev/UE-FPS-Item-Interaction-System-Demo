// Ignacio Roca UE5 Item Interaction Demo
//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItemBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate);

UCLASS()
class ITEMINTERACTIONDEMO_API AInteractiveItemBase : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FDelegate OnAction;
	
public:	
	AInteractiveItemBase();
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	bool PitchAndRollMouseRotation;

	UPROPERTY(EditAnywhere)
	float RotatePitchMultiplier = 1.5f;

	UPROPERTY(EditAnywhere)
	float RotateYawRollMultiplier = 2.f;

	UPROPERTY(EditAnywhere)
	bool MaintainOriginalRotation;

	UPROPERTY(EditDefaultsOnly)
	bool CheckActionOnEndRotate;

	UPROPERTY(BlueprintReadWrite)
	FName ActionName;

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnTriggerPickUp"))
	void ReceiveOnTriggerPickUp();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPickUp"))
	void ReceiveOnPickUp();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnTriggerPutDown"))
	void ReceiveOnTriggerPutDown();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPutDown"))
	void ReceiveOnPutDown();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStartRotate"))
	void ReceiveOnStartRotate();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnEndRotate"))
	void ReceiveOnEndRotate();
	
	UFUNCTION(BlueprintNativeEvent)
	bool IsActionAvailable();

	void DispatchOnActionEvents();

protected:
	virtual void BeginPlay() override;
};
