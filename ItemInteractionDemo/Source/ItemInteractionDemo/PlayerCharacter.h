// Ignacio Roca UE5 Item Interaction Demo
//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ITEMINTERACTIONDEMO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(EditDefaultsOnly)
	float InteractRange = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float GrabMoveSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float GrabRotateSpeed = 50.f;

	UPROPERTY(EditDefaultsOnly)
	float HoldDistance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInspectItemMenu> InspectItemMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* PlayerCamera;

protected:
	UPROPERTY(BlueprintReadWrite)
	bool ForceStopRotate = false;

	UPROPERTY()
	class AInteractiveItemBase* HeldItem;

	UPROPERTY()
	AActor* HeldItemParent;

	UPROPERTY()
	class UInspectItemMenu* InspectItemMenu;
	
	bool IsMovingItem = false;
	bool IsHoldingItem = false;
	bool IsRotatingItem = false;

	float MoveDistance = 0.f;
	FVector StartingItemLocation, ToLocation;
	FRotator StartingItemRotation;

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnHitDrawer"))
	void ReceiveOnHitDrawer(AActor* Drawer);
	
	void MoveXAxis(float InputValue);
	void MoveYAxis(float InputValue);
	
	void OnInteract();
	void MoveItem(const float DeltaTime);
	void RotateItem();
	
	void TriggerPickUpItem();
	void TriggerPutDownItem();
	void OnPickUpItem();
	void OnPutDownItem();
	
	void OnStartRotateItem();
	void OnEndRotateItem();

	UFUNCTION(BlueprintCallable)
	void UpdateItemMenu(bool ShowActionPrompt);
};
