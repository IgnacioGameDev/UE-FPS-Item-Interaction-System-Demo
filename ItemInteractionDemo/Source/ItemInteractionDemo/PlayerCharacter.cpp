// Ignacio Roca UE5 Item Interaction Demo

#include "PlayerCharacter.h"

#include "InspectItemMenu.h"
#include "InteractiveItemBase.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	InspectItemMenu = CreateWidget<UInspectItemMenu>(GetWorld()->GetFirstPlayerController(), InspectItemMenuClass);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsMovingItem)
		MoveItem(DeltaTime);
	else if (IsRotatingItem)
		RotateItem();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookVertical", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveX", this, &APlayerCharacter::MoveXAxis);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerCharacter::MoveYAxis);
	PlayerInputComponent->BindAction("Interact", IE_Pressed ,this, &APlayerCharacter::OnInteract);
}

void APlayerCharacter::MoveXAxis(float InputValue)
{
	AddMovementInput(GetActorForwardVector() * InputValue);
}

void APlayerCharacter::MoveYAxis(float InputValue)
{
	AddMovementInput(GetActorRightVector() * InputValue);
}

void APlayerCharacter::OnInteract()
{
	if (GetVelocity().Size() > 0) return;
	FHitResult hit;
	const FVector playerCameraWorldLocation = PlayerCamera->GetComponentLocation();
	if (GetWorld()->LineTraceSingleByChannel(hit,playerCameraWorldLocation , PlayerCamera->GetForwardVector() * InteractRange +  playerCameraWorldLocation, ECC_Visibility))
	{
		if (hit.GetActor()->ActorHasTag("Interactive"))
		{
			HeldItem = Cast<AInteractiveItemBase>(hit.GetActor());
			TriggerPickUpItem();
		}
		else
			ReceiveOnHitDrawer(hit.GetActor());
	}
}

void APlayerCharacter::MoveItem(const float DeltaTime)
{
	const float currentDistance = FVector::Distance(HeldItem->GetActorLocation(), ToLocation);
	HeldItem->SetActorRelativeLocation(FMath::VInterpTo(HeldItem->GetActorLocation(), ToLocation, DeltaTime, GrabMoveSpeed));
	if (HeldItem->MaintainOriginalRotation)
		HeldItem->SetActorRelativeRotation(FMath::Lerp(HeldItem->GetActorRotation(), StartingItemRotation, GrabRotateSpeed * (1 - currentDistance / MoveDistance)));
	if (currentDistance <= 0.1f)
	{
		IsMovingItem = false;
		if (IsHoldingItem)
			OnPickUpItem();
		else
			OnPutDownItem();
	}
}

void APlayerCharacter::RotateItem()
{
	const FQuat deltaQuat = HeldItem->PitchAndRollMouseRotation ?
		FRotator(InputComponent->GetAxisValue("LookVertical") * HeldItem->RotatePitchMultiplier, 0, InputComponent->GetAxisValue("LookHorizontal") * -1.f * HeldItem->RotateYawRollMultiplier).Quaternion() :
		FRotator(InputComponent->GetAxisValue("LookVertical") * HeldItem->RotatePitchMultiplier, InputComponent->GetAxisValue("LookHorizontal") * -1.f * HeldItem->RotateYawRollMultiplier, 0).Quaternion();
	
	FQuat newRotation = PlayerCamera->GetComponentTransform().InverseTransformRotation(HeldItem->GetActorQuat());
	newRotation = deltaQuat * newRotation;
	newRotation = PlayerCamera->GetComponentTransform().TransformRotation(newRotation);
	HeldItem->SetActorRotation(newRotation);
}

void APlayerCharacter::TriggerPickUpItem()
{
	InputComponent->ClearActionBindings();
	InputComponent->AxisBindings.Empty();
	
	if (HeldItem->GetAttachParentActor() != nullptr)
	{
		HeldItemParent = HeldItem->GetAttachParentActor();
		HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	
	StartingItemLocation = HeldItem->GetActorLocation();
	StartingItemRotation = HeldItem->GetActorRotation();
	ToLocation = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * HoldDistance;
	MoveDistance = FVector::Distance(StartingItemLocation, ToLocation);
	IsMovingItem = true;
	IsHoldingItem = true;
	
	HeldItem->SetActorEnableCollision(false);
	HeldItem->ReceiveOnTriggerPickUp();
}

void APlayerCharacter::TriggerPutDownItem()
{
	InspectItemMenu->RemoveFromViewport();
	ToLocation =  StartingItemLocation;
	IsMovingItem = true;
	IsHoldingItem = false;
	
	HeldItem->ReceiveOnTriggerPutDown();
}

void APlayerCharacter::OnPickUpItem()
{
	InputComponent->Deactivate();
	InputComponent->BindAction("ExitInteraction", IE_Pressed, this, &APlayerCharacter::TriggerPutDownItem);
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::OnStartRotateItem);
	InputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::OnEndRotateItem);
	InputComponent->BindAxis("LookHorizontal");
	InputComponent->BindAxis("LookVertical");
	InputComponent->BindAction("ItemAction", IE_Pressed, HeldItem, &AInteractiveItemBase::DispatchOnActionEvents);
	InputComponent->Activate();
	
	InspectItemMenu->AddToViewport();

	HeldItem->ReceiveOnPickUp();
	UpdateItemMenu(HeldItem->IsActionAvailable());

	GetLocalViewingPlayerController()->SetShowMouseCursor(true);
}

void APlayerCharacter::OnPutDownItem()
{
	if (HeldItemParent != nullptr)
		HeldItem->AttachToActor(HeldItemParent, FAttachmentTransformRules::KeepWorldTransform);

	InputComponent->ClearActionBindings();
	InputComponent->AxisBindings.Empty();
	SetupPlayerInputComponent(InputComponent);

	HeldItem->SetActorEnableCollision(true);
	HeldItem->ReceiveOnPutDown();

	GetLocalViewingPlayerController()->SetShowMouseCursor(false);
}

void APlayerCharacter::OnStartRotateItem()
{
	if (ForceStopRotate) return;
	IsRotatingItem = true;
	HeldItem->ReceiveOnStartRotate();
	InputComponent->RemoveActionBinding("ExitInteraction", IE_Pressed);
}

void APlayerCharacter::OnEndRotateItem()
{
	if (ForceStopRotate) return;
	IsRotatingItem = false;
	HeldItem->ReceiveOnEndRotate();
	if (HeldItem->CheckActionOnEndRotate)
		UpdateItemMenu(HeldItem->IsActionAvailable());
	InputComponent->BindAction("ExitInteraction", IE_Pressed, this, &APlayerCharacter::TriggerPutDownItem);
}

void APlayerCharacter::UpdateItemMenu(bool ShowActionPrompt)
{
	const ESlateVisibility menuVisibility = ShowActionPrompt ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	InspectItemMenu->ItemActionHorizontalBox->SetVisibility(menuVisibility);
	InspectItemMenu->ItemActionText->SetText(FText::FromName(HeldItem->ActionName));
}

