// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"

class UStateMachineLayer;
class UState;

/**
 * Generic State Machine Actor Component.
 * 
 * The state machine component can be composed by multiple state layers with multiple states, and is resposible for managing the state layers.
 *
 * State Layers: State layers can be seen as the actual state machine, they are resposible for managing the multiple states.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COUNTERPLAYTECHTASK_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Attemps to transition to a state inside a layer.
	*
	* \param Layer The layer in which we are attempting the state transition
	* \param State The state which we want to transition to
	* \return True If the transition was successful, False otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "State Machine Component")
	bool AttemptTransitionTo(TSubclassOf<UStateMachineLayer> Layer, TSubclassOf<UState> State);

	/**
	* Queries layer for current state
	*
	* \param Layer The layer to query for current state
	* \param State The state we want to compared equality against
	* \return Whether or not the current state is the one we are querying 
	*/
	UFUNCTION(BlueprintCallable, Category = "State Machine Component")
	bool IsLayerInCurrentState(TSubclassOf<UStateMachineLayer> Layer, TSubclassOf<UState> State);

public:
	// A set of unique subclass of UStateMachineLayers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine Component")
	TSet<TSubclassOf<UStateMachineLayer>> Layers;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Called on BeginPlayer, used to allocate instances of given UStateMachineLayers
	void Load();

private:
	// An array of pointers to the actual UStateMachineLayer instances
	UPROPERTY()
	TArray<UStateMachineLayer*> LayerInstances;
};
