// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineLayer.h"
#include "StateMachineComponent.h"
#include "State.h"

bool UStateMachineLayer::Load(const UStateMachineComponent* Owner, int& OutTotalLoadedStates) {
	int total_states_classes = StatesClasses.Num();
	if (total_states_classes == 0) return false;

	if (!Owner) return false;

	OwnerComponent = Owner;
	User = OwnerComponent->GetOwner();

	OutTotalLoadedStates = 0;
	StatesInstances.Reserve(total_states_classes);

	for (const auto& state_class : StatesClasses) {
		if (!state_class) {
			return false;
		}

		UState* state = NewObject<UState>(this, state_class.Get(), state_class->GetFName());
		state->SetLayerContext(this);
		state->Name = state_class->GetFName();
		state->Setup();
		StatesInstances.Add(state);
		OutTotalLoadedStates++;

		if (InitialStateClassRef == state_class) {
			InitialState = state;
		}
	}

	if (!InitialState) {
		InitialState = StatesInstances[0];
	}

	Setup();
	return true;
}

void UStateMachineLayer::Update(float DeltaTime) {
	if (!CurrentState) {
		CurrentState = InitialState;
		CurrentState->Enter();
	}

	if (TSubclassOf<UState> target_state = CurrentState->CheckForStateTransition()) {
		if (target_state != CurrentState->GetClass())
			TransitionToState(target_state);
	}

	CurrentState->Update(DeltaTime);
}

bool UStateMachineLayer::AttemptTransitionTo(TSubclassOf<UState> State) {
	if (!CurrentState->CanTransitionTo(State)) return false;

	return TransitionToState(State);
}

bool UStateMachineLayer::IsCurrentState(TSubclassOf<UState> State) {
	return CurrentState->GetClass() == State;
}

bool UStateMachineLayer::TransitionToState(TSubclassOf<UState> State) {
	FSetElementId state_id = StatesClasses.FindId(State);
	if (!state_id.IsValidId()) return false;

	UState* NextState = StatesInstances[state_id.AsInteger()];
	if (!NextState->CanEnter()) return false;

	CurrentState->Exit();
	NextState->Enter();
	CurrentState = NextState;

	return true;
}

void UStateMachineLayer::Setup_Implementation() {}