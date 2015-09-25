#pragma once
#include <GrowingArray.h>

namespace CommonUtilities
{
	class InputWrapper;
}

class GameState;

class StateStack
{
public:
	StateStack();
	~StateStack();

	void PushSubGameState(GameState* aSubGameState);
	void PushMainGameState(GameState* aMainGameState);
	bool UpdateCurrentState();
	void RenderCurrentState();
	void OnResizeCurrentState(int aWidth, int aHeight);
	void Clear();

	void SetInputWrapper(CU::InputWrapper* anInputWrapper);

private:
	CU::GrowingArray<CU::GrowingArray<GameState*, int>, int> myGameStates;
	void PopSubGameState();
	void PopMainGameState();
	void RenderStateAtIndex(int aIndex);

	int myMainIndex;
	int mySubIndex;

	CU::InputWrapper* myInputWrapper;
};

inline void StateStack::SetInputWrapper(CU::InputWrapper* anInputWrapper)
{
	myInputWrapper = anInputWrapper;
}