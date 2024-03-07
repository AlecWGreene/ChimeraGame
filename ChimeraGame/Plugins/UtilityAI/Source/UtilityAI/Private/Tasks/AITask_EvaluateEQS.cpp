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

			UEnvQueryItemType* ItemTypeCDO = Result->ItemType->GetDefaultObject<UEnvQueryItemType>();
			if (ItemTypeCDO->IsA(UEnvQueryItemType_VectorBase::StaticClass()))
			{
				UEnvQueryItemType_VectorBase* VectorItemTypeCDO = Cast<UEnvQueryItemType_VectorBase>(ItemTypeCDO);
				for (const FEnvQueryItem& Item : Result->Items)
				{
					FVector ItemLocation = VectorItemTypeCDO->GetItemLocation(Result->RawData.GetData() + Item.DataOffset);
					LocationItems.Add(ItemLocation);
				}
			}
			else if (ItemTypeCDO->IsA(UEnvQueryItemType_ActorBase::StaticClass()))
			{
				UEnvQueryItemType_ActorBase* ActorItemTypeCDO = Cast<UEnvQueryItemType_ActorBase>(ItemTypeCDO);
				for (const FEnvQueryItem& Item : Result->Items)
				{
					AActor* ItemActor = ActorItemTypeCDO->GetActor(Result->RawData.GetData() + Item.DataOffset);
					ActorItems.Add(ItemActor);
				}
			}

			OnComplete.Broadcast(*Result.Get(), LocationItems, ActorItems);
		}
	}

	EndTask();
}