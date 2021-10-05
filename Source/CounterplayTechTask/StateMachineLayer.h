// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateMachineLayer.generated.h"

class UStateMachineComponent;
class UState;

/**
 * A state machine layer, a representation of a state machine capable of managing a group of states inheriting from UState.
 * 
 * This classes exposes a set of UClass references to subclasses of UState, which guarantees state uniqueness.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class COUNTERPLAYTECHTASK_API UStateMachineLayer : public UObject
{
	GENERATED_BODY()

public:
	virtual ~UStateMachineLayer() = default;

	// Called by the owning StateMachineComponent, used to allocate instances of subclasses of UState
	bool Load(const UStateMachineComponent* Owner, int& OutTotalLoadedStates);

	// Called for every layer as soon as an instance of it is created
	// Use it to initialize layer properties
	UFUNCTION(BlueprintNativeEvent, Category = "State Layer Logic")
	void Setup();

	/**
	* Attemps to transition to a state.
	*
	* \param State The state which we want to transition to
	* \return True If the transition was successful, False otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "State Layer Logic")
	bool AttemptTransitionTo(TSubclassOf<UState> State);

	/**
	* Queries for current state
	*
	* \param State The state we want to compared equality against
	* \return Whether or not the current state is the one we are querying
	*/
	UFUNCTION(BlueprintCallable, Category = "State Layer Logic")
	bool IsCurrentState(TSubclassOf<UState> State);

	// Called every frame by the State Machine Component
	UFUNCTION()
	void Update(float DeltaTime);

public:
	// A set of unique subclasses of UState 
	UPROPERTY(EditDefaultsOnly, Category = "State Machine Layer")
	TSet<TSubclassOf<UState>> StatesClasses;

	// Initial state uclass reference
	UPROPERTY(EditDefaultsOnly, Category = "State Machine Layer")
	TSubclassOf<UState> InitialStateClassRef;

protected:
	virtual void Setup_Implementation();

private:
	// Tries to apply the actual transition fo states
	UFUNCTION()
	bool TransitionToState(TSubclassOf<UState> State);

private:
	// An array of pointers to the actual UState instances
	UPROPERTY()
	TArray<UState*> StatesInstances;

	// Pointer to initial state
	UPROPERTY()
	UState* InitialState;

	// Pointer to current state
	UPROPERTY()
	UState* CurrentState;

	// Pointer to owning state machine component
	UPROPERTY(BlueprintReadOnly, Category = "Layer Info", meta = (AllowPrivateAccess = "true"))
	const UStateMachineComponent* OwnerComponent;

	// Pointer to owning actor
	UPROPERTY(BlueprintReadOnly, Category = "Layer Info", meta = (AllowPrivateAccess = "true"))
	AActor* User;
};
