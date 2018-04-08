//-----------------------------------------------------------
// File: MeteorShowerLevel.h
// Description: Singleton that stores all model files. Models
//				are represented by a list of points. Saves space 
//				by releasing pointers to each model but this
//				disables modifying models
//-----------------------------------------------------------

#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "..\Graphics\Model2D.h"

class ModelManager : public Singleton< ModelManager >
{
public:
	friend Singleton< ModelManager >;

private:
	std::map< String, Model2D > m_modelMap;

public:
	static Model2D* GetModel( const String& modelName );

protected:
	ModelManager();
	~ModelManager();

private:
	Model2D* InGetModel( const String& modelName );

	void LoadModelFiles();
	
	void CreateAsteroidModel1();
	void CreateAsteroidModel2();
	void CreateAsteroidModel3();
	void CreatePlayerModel();
	void CreatePlayerModelAlt();

	void CreateQuadModel();
	void CreateCircle();
};



#endif