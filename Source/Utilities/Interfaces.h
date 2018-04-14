//-----------------------------------------------------------
// File: Interfaces.h
// Description: Some interface classes to make simplified access 
//				to some classes
//-----------------------------------------------------------

#ifndef INTERFACES_H
#define INTERFACES_H

// Interface for sounds to make them stop playing
class IStoppable
{
public:
	virtual void VStop() = 0;
};

#endif