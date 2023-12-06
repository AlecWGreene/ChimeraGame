// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNodeUtils.h"
#include "K2Node.h"

#include "BPNode_CreateTargetDataFilterHandle.generated.h"

UCLASS()
class CHIMERAGAS_BLUEPRINTNODES_API UBPNode_CreateTargetDataFilterHandle : public UK2Node
{
	GENERATED_BODY()

	// Data goes here

public:
	virtual void AllocateDefaultPins() override;
	virtual void PostReconstructNode() override;

public:

};