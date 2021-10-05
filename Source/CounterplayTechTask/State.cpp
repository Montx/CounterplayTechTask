// Fill out your copyright notice in the Description page of Project Settings.


#include "State.h"
#include "StateMachineLayer.h"

void UState::Setup_Implementation() {}

void UState::Enter_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("[%s] State Enter: %s"), *LayerContext->GetClass()->GetFName().ToString(), *Name.ToString());
}

bool UState::CanEnter_Implementation() {
	return true;
}

void UState::Exit_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("[%s] State Exit: %s"), *LayerContext->GetClass()->GetFName().ToString(), *Name.ToString());
}

void UState::Update_Implementation(float DeltaTime) {}

TSubclassOf<UState> UState::CheckForStateTransition_Implementation() {
	return {};
}

bool UState::CanTransitionTo(TSubclassOf<UState> State) {
	if (State == GetClass()) return false;

	if (AllowedStates.Num() == 0) return true;

	return AllowedStates.Contains(State);
}
