// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ChimeraAbilitySystemComponent.h"
#include "ChimeraCharacter.generated.h"

struct FInputActionValue;

UCLASS(Blueprintable, BlueprintType, config = Game)
class CHIMERACORE_API AChimeraCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	//----- Overrides -----//
public:
	AChimeraCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	//----- Player Input -----//
protected:

	void HandleInputSettingsLoaded(class UInputComponent* PlayerInputComponent);

	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleLookInput(const FInputActionValue& InputActionValue);

	//----- Animation -----//
protected:

	UFUNCTION(BlueprintPure)
	virtual const class UChimeraAnimSet* GetAnimSetForMesh( 
		UPARAM(meta = (Categories = "Anim.Set")) FGameplayTag AnimSetTag, 
		const USkeletalMeshComponent* InMesh) const;

	/** Looks up the montage using the tag and the anim set for the provided mesh. Defaults to GetMesh when Mesh is not provided.*/
	UFUNCTION(BlueprintPure)
	virtual const UAnimMontage* GetMontageByTag(
		UPARAM(meta = (Categories = "Anim.Montage")) FGameplayTag MontageTag, 
		UPARAM(meta = (Categories = "Anim.Set")) FGameplayTag AnimSetTag = FGameplayTag(), 
		USkeletalMeshComponent* InMesh = nullptr) const;
};
