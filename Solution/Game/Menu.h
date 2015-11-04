#pragma once

namespace Prism
{
	class Sprite;
}

namespace CU
{
	class InputWrapper;
}

class Button;
class GameStateMessage;

class Menu
{
public:
	Menu(const std::string& aXMLPath);
	~Menu();
	
	void Render(CU::InputWrapper* anInputWrapper);

	bool Update(CU::InputWrapper* anInputWrapper);

	void OnResize(int aWidth, int aHeight);

	bool GetMainMenu() const;

private:
	bool myMainMenu;
	CU::GrowingArray<Button*> myButtons;
	Prism::Sprite* myBackground;
	Prism::Sprite* myCrosshair;
};

inline bool Menu::GetMainMenu() const
{
	return myMainMenu;
}