// Copyright Alec Greene. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Tasks/AITask.h"

#include "AITask_EvaluateEQS.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAITask_EvaluateEQS_Complete, const FEnvQueryResult&, Result, const TArray<FVector>&, LocationItems,const TArray<AActor*>&, ActorItems);

/**
 * Based on UAITask_EQS from Epic Games, but more BP friendly.
 */
UCLASS()
class UTILITYAI_API UAITask_EvaluateEQS : public UAITask
{
	GENERATED_BODY()

public:

	UAITask_EvaluateEQS(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FAITask_EvaluateEQS_Complete OnComplete;

	UFUNCTION(BlueprintCallable, Category = "AI|Tasks", meta = (DefaultToSelf = "Controller", BlueprintInternalUseOnly = "true"))
	static UAITask_EvaluateEQS* EvaluateEQS(AAIController* Controller, class UEnvQuery* QueryTemplate, EAITaskPriority Priority = EAITaskPriority::High);

protected:

	virtual void Activate() override;

	void HandleEQSRequestComplete(TSharedPtr<FEnvQueryResult> Result);

protected:

	FEQSParametrizedQueryExecutionRequest EQSRequest;

	/** Internal delegate passed into EQS. */
	FQueryFinishedSignature OnQueryComplete;
};