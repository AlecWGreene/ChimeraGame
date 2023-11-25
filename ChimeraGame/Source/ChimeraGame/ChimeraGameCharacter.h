// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ChimeraAbilitySystemComponent.h"
#include "ChimeraGameCharacter.generated.h"

struct FInputActionValue;

UCLASS(config=Game)
class AChimeraGameCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	//----- Components -----//
protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChimeraAbilitySystemComponent> AbilitySystemComponent;

public:

	class USpringArmComponent* GetCameraBoom() const { return CameraBoom.Get(); }
	class UCameraComponent* GetFollowCamera() const { return FollowCamera.Get(); }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return Cast<UAbilitySystemComponent>(AbilitySystemComponent.Get()); }

	//----- Constructor and Engine Events -----//
public:
	AChimeraGameCharacter();

	//----- Interface Overrides -----//
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//----- Player Input -----//
protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UChimeraInputConfig> DefaultPlayerInputConfig;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UInputMappingContext> DefaultPlayerInputMappingContext;

	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleLookInput(const FInputActionValue& InputActionValue);
};
