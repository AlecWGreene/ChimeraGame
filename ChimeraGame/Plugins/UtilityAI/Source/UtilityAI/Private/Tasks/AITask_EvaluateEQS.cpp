// Copyright Alec Greene. All Rights Reserved.
#include "Tasks/AITask_EvaluateEQS.h"

#include "AIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"

UAITask_EvaluateEQS::UAITask_EvaluateEQS(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OnQueryComplete = FQueryFinishedSignature::CreateUObject(this, &ThisClass::HandleEQSRequestComplete);
}

UAITask_EvaluateEQS* UAITask_EvaluateEQS::EvaluateEQS(AAIController* Controller, UEnvQuery* QueryTemplate, EAITaskPriority Priority)
{
	if (IsValid(Controller) && QueryTemplate)
	{
		UAITask_EvaluateEQS* NewTask = UAITask::NewAITask<UAITask_EvaluateEQS>(*Controller, Priority);
		NewTask->EQSRequest.QueryTemplate = QueryTemplate;
		return NewTask;
	}

	return nullptr;
}

void UAITask_EvaluateEQS::Activate()
{
	if (EQSRequest.QueryTemplate && OwnerController 
		&& OwnerController->GetBlackboardComponent() 
		&& OwnerController->GetPawn())
	{
		Super::Activate();
		EQSRequest.Execute(
			*OwnerController->GetPawn(), 
			OwnerController->GetBlackboardComponent(), 
			OnQueryComplete);
	}
	else
	{
		EndTask();
	}
}

void UAITask_EvaluateEQS::HandleEQSRequestComplete(TSharedPtr<FEnvQueryResult> Result)
{
	if (IsActive() && Result.IsValid())
	{
		if (OnComplete.IsBound() && Result->ItemType)
		{
			TArray<FVector> LocationItems;
			TArray<AActor*> ActorItems;
			if (Result->ItemType->IsChildOf(UEnvQueryItemType_VectorBase::StaticClass()))
			{
				for (const FEnvQueryItem& Item : Result->Items)
				{
					FVector ItemLocation = GetMutableDefault<UEnvQueryItemType_VectorBase>()->GetItemLocation(Result->RawData.GetData() + Item.DataOffset);
					LocationItems.Add(ItemLocation);
				}
			}
			else if (Result->ItemType->IsChildOf(UEnvQueryItemType_ActorBase::StaticClass()))
			{
				
				for (const FEnvQueryItem& Item : Result->Items)
				{
					AActor* ItemActor = GetMutableDefault<UEnvQueryItemType_ActorBase>()->GetActor(Result->RawData.GetData() + Item.DataOffset);
					ActorItems.Add(ItemActor);
				}
			}

			OnComplete.Broadcast(*Result.Get(), LocationItems, ActorItems);
		}
	}

	EndTask();
}