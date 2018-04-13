//-----------------------------------------------------------
// File: Menu.cpp 
//-----------------------------------------------------------

#include "STD.h"
#include "Menu.h"
#include <utility>

#include "../GameMain/GameState.h"
#include "../Input/Keyboard.h"
#include "../Graphics/Model2D.h"
#include "../Content/ModelManager.h"

Menu::Menu(GameState* parent, std::shared_ptr<std::vector<MenuItem>> items, int _pointerOffset) :
	parentState( parent ), font(std::move(WHITE)), menuItems( items ), pointerOffset( _pointerOffset )
{
	// Calculate current Item Index by finding the menu item with the smallest Y position
	float y = 10000;

	for( size_t i = 0; i < menuItems->size(); ++i )
	{
		if( (*menuItems)[i].GetPosition().y <= y ) // if y smaller
		{
			// store smaller position and item index
			y = (*menuItems)[i].GetPosition().y;
			currentItemIndex = i;
		}
	}

	// Set the model
	pointerModel = ModelManager::GetModel( "Circle" );

	Keyboard::GetInstance().GetKeyboardState( prevKbState );
}

void Menu::Update()
{
	KeyboardState kbState;
	Keyboard::GetInstance().GetKeyboardState( kbState );

	// Check for up/down keyboard input
	if( kbState.GetKeyState( VK_UP ) == pressed &&
		prevKbState.GetKeyState( VK_UP ) == unpressed )
	{
		currentItemIndex++;
		currentItemIndex = currentItemIndex % menuItems->size();
	}
	else if( kbState.GetKeyState( VK_DOWN ) == pressed &&
		prevKbState.GetKeyState( VK_DOWN ) == unpressed )
	{
		currentItemIndex--;
		currentItemIndex = NegativeMod( currentItemIndex, menuItems->size() );
	}

	// Check for return
	if( kbState.GetKeyState( VK_RETURN ) == pressed &&
		prevKbState.GetKeyState( VK_RETURN ) == unpressed  )
	{
		(*menuItems)[currentItemIndex].Select();
	}

	prevKbState = kbState;
}

void Menu::Render() const
{
	// draw the pointer to symbolize the selected menu item
	Vector2 pointerPos1 = (*menuItems)[currentItemIndex].GetPosition();
	Vector2 pointerPos2 = pointerPos1;
	pointerPos1.x -= pointerOffset;
	pointerPos2.x += pointerOffset;
	pointerModel->Render( pointerPos1, Vector2(7, 7), 0 );
	pointerModel->Render( pointerPos2, Vector2(7, 7), 0 );

	// draw all the menu items
	for (MenuItem& item : *menuItems)
	{
		item.Render();
	}
}
