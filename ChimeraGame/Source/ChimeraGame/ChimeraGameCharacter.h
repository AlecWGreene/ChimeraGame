// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChimeraGameCharacter.generated.h"

struct FInputActionValue;

// @agreene #Todo - 2023/06/24 - Move macro to a separate file
#define DECLARE_ACTOR_COMPONENT(ComponentClass, ComponentName) \
	TObjectPtr<class ComponentClass> ComponentName; \
public: \
	FORCEINLINE class ComponentClass* Get##ComponentName() const { return ComponentName; } \
protected:

UCLASS(config=Game)
class AChimeraGameCharacter : public ACharacter
{
	GENERATED_BODY()

	//----- Components -----//
protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	DECLARE_ACTOR_COMPONENT(USprintArmComponent, CameraBoom);

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	DECLARE_ACTOR_COMPONENT(UCameraComponent, FollowCamera);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag InputMoveTag;
	UPROPERTY(Config) FName InputMoveTagName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag InputLookTag;
	UPROPERTY(Config) FName InputLookTagName;

	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleLookInput(const FInputActionValue& InputActionValue);
};

