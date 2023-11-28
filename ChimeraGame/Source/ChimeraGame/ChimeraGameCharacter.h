// Copyright Alec Greene. All Rights Reserved.

#pragma once

#include "ChimeraCharacter.h"
#include "ChimeraGameCharacter.generated.h"

UCLASS(Blueprintable, BlueprintType, config = Game)
class CHIMERAGAME_API AChimeraGameCharacter : public AChimeraCharacter
{
	GENERATED_BODY()

	//----- Overrides -----//
public:
	AChimeraGameCharacter();

	virtual const class UChimeraAnimSet* GetAnimSetForMesh(const USkeletalMeshComponent* InMesh) const override;

	//----- Components -----//
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> ShieldMesh_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> ShieldMesh_Right;

public:

	USkeletalMeshComponent* GetWeaponMesh_Left() const { return WeaponMesh_Left; }
	USkeletalMeshComponent* GetWeaponMesh_Right() const { return WeaponMesh_Right; }
	USkeletalMeshComponent* GetShieldMesh_Left() const { return ShieldMesh_Left; }
	USkeletalMeshComponent* GetShieldMesh_Right() const { return ShieldMesh_Right; }

	//----- Class Settings -----//
public:

	// agreene 2023/11/27 - #ToDo Move this to async loading
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const class UChimeraAnimSet> AnimSet;
};