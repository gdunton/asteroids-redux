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
	Model2D CreateAsteroidModel1() const;
	Model2D CreateAsteroidModel2() const;
	Model2D CreateAsteroidModel3() const;
	Model2D CreatePlayerModel() const;

	Model2D CreateQuadModel() const;
	Model2D CreateCircle() const;
};


#endif
