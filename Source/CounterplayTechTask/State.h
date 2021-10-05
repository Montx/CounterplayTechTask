// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.generated.h"


/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class COUNTERPLAYTECHTASK_API UState : public UObject
{
	GENERATED_BODY()

public:
	UState() = default;

	// Sets the pointer to the owning layer context
	FORCEINLINE void SetLayerContext(class UStateMachineLayer* Layer) { LayerContext = Layer; }

	// Called for every layer as soon as an instance of it is created
	// Use it to initialize layer properties
	UFUNCTION(BlueprintNativeEvent, Category = "State Logic")
	void Setup();

	// Called when the state enters
	UFUNCTION(BlueprintNativeEvent, Category = "State Logic")
	void Enter();


	// Called when the a transition happens to check if the transition is valid
	UFUNCTION(BlueprintNativeEvent, Category = "State Logic")
	bool CanEnter();

	// Called when the state leaves
	UFUNCTION(BlueprintNativeEvent, Category = "State Logic")
	void Exit();

	// Called every frame by the owning State Macine Layer
	UFUNCTION(BlueprintNativeEvent, Category = "State Logic")
	void Update(float DeltaTime);

	// Called every frame by the owning State Macine Layer in to check for passive transitions
	UFUNCTION(BlueprintNativeEvent, Category = "State Logic")
	TSubclassOf<UState> CheckForStateTransition();

	// Called when a global transition is attempted to check for valid global transitions
	UFUNCTION(BlueprintCallable, Category = "State Transitions")
	bool CanTransitionTo(TSubclassOf<UState> State);

public:
	// The state class name
	UPROPERTY(BlueprintReadOnly, Category = "State Info")
	FName Name;

	// States this state is allowed to globaly transition to
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Global Transitions")
	TSet<TSubclassOf<UState>> AllowedStates;
	
protected:
	virtual void Setup_Implementation();
	virtual void Enter_Implementation();
	virtual bool CanEnter_Implementation();
	virtual void Exit_Implementation();
	virtual void Update_Implementation(float DeltaTime);
	virtual TSubclassOf<UState> CheckForStateTransition_Implementation();

protected:
	// Pointer to owning State Machine Layer
	UPROPERTY(BlueprintReadOnly, Category = "State Info")
	class UStateMachineLayer* LayerContext;
};
