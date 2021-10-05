// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineComponent.h"
#include "StateMachineLayer.h"

// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	Load();
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (UStateMachineLayer* layer : LayerInstances) {
		layer->Update(DeltaTime);
	}
}

void UStateMachineComponent::Load() {
	if (Layers.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("[%s]: Failed to load states!"), *GetClass()->GetFName().ToString());
		return;
	}

	LayerInstances.Reserve(Layers.Num());
	for (const auto& LayerClass : Layers) {
		if (!LayerClass) {
			return;
		}

		UStateMachineLayer* layer = NewObject<UStateMachineLayer>(this, LayerClass.Get(), LayerClass->GetFName());

		int loaded_states = 0;
		if (!layer->Load(this, loaded_states)) {
			UE_LOG(LogTemp, Error, TEXT("[%s]: Failed to load states!"), *LayerClass->GetFName().ToString());
			return;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("[%s]: Loaded %d states!"), *LayerClass->GetFName().ToString(), loaded_states);
		}

		LayerInstances.Add(layer);
	}
}

bool UStateMachineComponent::AttemptTransitionTo(TSubclassOf<UStateMachineLayer> Layer, TSubclassOf<UState> State) {
	FSetElementId layer_id = Layers.FindId(Layer);
	if (!layer_id.IsValidId()) return false;

	return LayerInstances[layer_id.AsInteger()]->AttemptTransitionTo(State);
}

bool UStateMachineComponent::IsLayerInCurrentState(TSubclassOf<UStateMachineLayer> Layer, TSubclassOf<UState> State) {
	FSetElementId layer_id = Layers.FindId(Layer);
	if (!layer_id.IsValidId()) return false;

	return LayerInstances[layer_id.AsInteger()]->IsCurrentState(State);
}


