//-----------------------------------------------------------
// File: Menu.cpp 
//-----------------------------------------------------------

#include "STD.h"
#include "Menu.h"

#include "..\GameMain\GameState.h"
#include "..\Input\Keyboard.h"
#include "..\Graphics\Model2D.h"
#include "..\Content\ModelManager.h"

Menu::Menu()
{
	parentState = NULL;
	pointerModel = NULL;
	pointerOffset = 0;
	currentItemIndex = 0;
}

Menu::Menu(GameState* parent, std::shared_ptr<std::vector<MenuItem>> items, int _pointerOffset) :
	parentState( parent ), menuItems( items ), pointerOffset( _pointerOffset )
{
	font.Initialize( 20, false, WHITE);

	// Calculate current Item Index by finding the menu item with the smallest Y position
	int y = 10000;

	for( int i = 0; i < menuItems->size(); ++i )
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

Menu::~Menu()
{
	font.Destroy();
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

void Menu::Render()
{
	// draw the pointer to symbolize the selected menu item
	MyVector2 pointerPos1 = (*menuItems)[currentItemIndex].GetPosition();
	MyVector2 pointerPos2 = pointerPos1;
	pointerPos1.x -= pointerOffset;
	pointerPos2.x += pointerOffset;
	pointerModel->Render( pointerPos1, MyVector2(7, 7), 0 );
	pointerModel->Render( pointerPos2, MyVector2(7, 7), 0 );

	// draw all the menu items
	for (MenuItem& item : *menuItems)
	{
		item.Render();
	}
}