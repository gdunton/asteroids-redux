//-----------------------------------------------------------
// File: MeteorShowerLevel.h
// Description: Singleton that stores all model files. Models
//				are represented by a list of points. Saves space 
//				by releasing pointers to each model but this
//				disables modifying models
//-----------------------------------------------------------

#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "../Graphics/Model2D.h"

class ModelManager
{
public:

	static Model2D CreateAsteroidModel1();
	static Model2D CreateAsteroidModel2();
	static Model2D CreateAsteroidModel3();
	static Model2D CreatePlayerModel();

	static Model2D CreateQuadModel();
	static Model2D CreateCircle();
};


#endif
