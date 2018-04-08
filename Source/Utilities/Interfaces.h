//-----------------------------------------------------------
// File: Interfaces.h
// Description: Some interface classes to make simplified access 
//				to some classes
//-----------------------------------------------------------

#ifndef INTERFACES_H
#define INTERFACES_H

// Interface for the window to call functions on the game
class IGame
{
public:
	virtual void VUpdate() = 0;
	virtual void VRender() = 0;

	virtual void VClose() = 0;
};

// Interface for sounds to make them stop playing
class IStoppable
{
public:
	virtual void VStop() = 0;
};

#endif