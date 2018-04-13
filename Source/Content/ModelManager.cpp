//-----------------------------------------------------------
// File: ModelManager.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "ModelManager.h"

template<> ModelManager* Singleton<ModelManager>::instance = nullptr;

//#define INVERT_MODEL_Y
#ifdef INVERT_MODEL_Y
#define Vector2(x,y) Vector2(x,y*-1)
#endif

ModelManager::ModelManager()
{
	LoadModelFiles();
}

ModelManager::~ModelManager()
{
}

Model2D* ModelManager::GetModel( const String& modelName )
{
	return ModelManager::GetInstance().InGetModel(modelName);
}

Model2D* ModelManager::InGetModel( const String& modelName )
{
	return &m_modelMap[modelName];
}

void ModelManager::LoadModelFiles()
{
	// Create each of the models
	CreateAsteroidModel1();
	CreateAsteroidModel2();
	CreateAsteroidModel3();
	CreatePlayerModel();
	CreatePlayerModelAlt();
	CreateQuadModel();
	CreateCircle();
}

void ModelManager::CreateAsteroidModel1()
{
	m_modelMap["Asteroid1"] = Model2D();
	Model2D& m = m_modelMap["Asteroid1"];
	m.points.emplace_back(-1.9f, 2.2f);
	m.points.emplace_back(-2.7f, 1.5f);
	m.points.emplace_back(-2.9f, 0.0f);
	m.points.emplace_back(-0.5f, -3.0f);
	m.points.emplace_back(1.7f,-2.0f);
	m.points.emplace_back(2.1f, 0.0f);
	m.points.emplace_back(1.9f, 0.7f);
	m.points.emplace_back(1.4f, 3.0f);
	m.points.emplace_back(0.0f, 3.3f);
	m.points.emplace_back(-1.9f, 2.2f);
	m.CalculateModelStats();
}

void ModelManager::CreateAsteroidModel2()
{
	m_modelMap["Asteroid2"] = Model2D();
	Model2D& m = m_modelMap["Asteroid2"];
	m.points.emplace_back(-2.0f, 3.2f);
	m.points.emplace_back(-3.1f, 2.0f);
	m.points.emplace_back(-3.3f, 0.0f);
	m.points.emplace_back(-3.1f, -2.2f);
	m.points.emplace_back(-1.4f, -2.7f);
	m.points.emplace_back(0.0f, -3.0f);
	m.points.emplace_back(2.0f, -2.4f);
	m.points.emplace_back(3.0f, -1.0f);
	m.points.emplace_back(3.1f, 1.5f);
	m.points.emplace_back(2.8f, 1.9f);
	m.points.emplace_back(-0.5f, 3.0f);
	m.points.emplace_back(-2.0f, 3.2f);
	m.CalculateModelStats();
}

void ModelManager::CreateAsteroidModel3()
{
	m_modelMap["Asteroid3"] = Model2D();
	Model2D& m = m_modelMap["Asteroid3"];
	m.points.emplace_back(-0.3f, 3.1f);
	m.points.emplace_back(-1.8f, 2.3f);
	m.points.emplace_back(-2.2f, 0.7f);
	m.points.emplace_back(-2.4f, -1.0f);
	m.points.emplace_back(-1.4f, -2.3f);
	m.points.emplace_back(-0.1f, -2.1f);
	m.points.emplace_back(1.1f, -1.4f);
	m.points.emplace_back(2.6f, -0.6f);
	m.points.emplace_back(3.0f, 1.0f);
	m.points.emplace_back(1.8f, 2.0f);
	m.points.emplace_back(0.7f, 3.0f);
	m.points.emplace_back(-0.3f, 3.1f);
	m.CalculateModelStats();
}

void ModelManager::CreatePlayerModel()
{
	m_modelMap["Player"] = Model2D();
	Model2D& m = m_modelMap["Player"];
	m.points.emplace_back(-1.0f, -1.0f);
	m.points.emplace_back(0.0f, 2.0f);
	m.points.emplace_back(1.0f, -1.0f);
	m.points.emplace_back(-1.0f, -1.0f);
	m.CalculateModelStats();
}

void ModelManager::CreatePlayerModelAlt()
{
	m_modelMap["PlayerAlt"] = Model2D();
	Model2D& m = m_modelMap["PlayerAlt"];
	m.points.emplace_back(-1.0f, -1.0f);
	m.points.emplace_back(0.0f, 2.0f);
	m.points.emplace_back(1.0f, -1.0f);
	m.points.emplace_back(-1.0f, -1.0f);
	m.CalculateModelStats();
}

void ModelManager::CreateQuadModel()
{
	m_modelMap["Quad"] = Model2D();
	Model2D& m = m_modelMap["Quad"];
	m.points.emplace_back(-1.0f, -1.0f);
	m.points.emplace_back(-1.0f, 1.0f);
	m.points.emplace_back(1.0f, 1.0f);
	m.points.emplace_back(1.0f, -1.0f);
	m.points.emplace_back(-1.0f, -1.0f);
	m.CalculateModelStats();
}

void ModelManager::CreateCircle()
{
	m_modelMap["Circle"] = Model2D();
	Model2D& m = m_modelMap["Circle"];
	m.points.emplace_back(1.0f, 0.0f);
	m.points.emplace_back(0.966f, 0.259f);
	m.points.emplace_back(0.866f, 0.5f);
	m.points.emplace_back(0.707f, 0.707f);
	m.points.emplace_back(0.5f, 0.866f);
	m.points.emplace_back(0.259f, 0.966f);
	m.points.emplace_back(0.0f, 1.0f);
	m.points.emplace_back(-0.259f, 0.966f);
	m.points.emplace_back(-0.5f, 0.866f);
	m.points.emplace_back(-0.707f, 0.707f);
	m.points.emplace_back(-0.866f, 0.5f);
	m.points.emplace_back(-0.966f, 0.259f);
	m.points.emplace_back(-1.0f, 0.0f);
	m.points.emplace_back(-0.966f, -0.259f);
	m.points.emplace_back(-0.866f, -0.5f);
	m.points.emplace_back(-0.707f, -0.707f);
	m.points.emplace_back(-0.5f, -0.866f);
	m.points.emplace_back(-0.259f, -0.966f);
	m.points.emplace_back(0.0f, -1.0f);
	m.points.emplace_back(0.259f, -0.966f);
	m.points.emplace_back(0.5f, -0.866f);
	m.points.emplace_back(0.707f, -0.707f);
	m.points.emplace_back(0.866f, -0.5f);
	m.points.emplace_back(0.966f, -0.259f);
	m.points.emplace_back(1.0f, 0.0f);

	m.CalculateModelStats();
}