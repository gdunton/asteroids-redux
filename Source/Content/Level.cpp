#include "Level.h"
#include "../GameMain/Globals.h"
#include "../Graphics/Camera.h"

namespace
{
	Camera createCamera(const Vector2& pos, const Vector2& size)
	{
		return Camera(pos, size, WINDOW_WIDTH, WINDOW_HEIGHT);
	}
}

std::vector<Camera> Level::CreateCameraArray()
{
	// Initialize the camera array
	std::vector<Camera> cameras;
	cameras.reserve(9);

	// Center-Center
	cameras.emplace_back(createCamera(Vector2(0, 0), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));
	// Left-Center
	cameras.emplace_back(createCamera(Vector2(-WORLD_WIDTH, 0), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));
	// Right-Center
	cameras.emplace_back(createCamera(Vector2(WORLD_WIDTH, 0), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));
	// Left-Top
	cameras.emplace_back(createCamera(Vector2(-WORLD_WIDTH, WORLD_HEIGHT), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));
	// Center-Top
	cameras.emplace_back(createCamera(Vector2(0, WORLD_HEIGHT), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));
	// Right-Top
	cameras.emplace_back(createCamera(Vector2(WORLD_WIDTH, WORLD_HEIGHT), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));
	// Left-Bottom
	cameras.emplace_back(createCamera(Vector2(-WORLD_WIDTH, -WORLD_HEIGHT), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));
	// Center-Bottom
	cameras.emplace_back(createCamera(Vector2(0, -WORLD_HEIGHT), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));
	// Right-Bottom
	cameras.emplace_back(createCamera(Vector2(WORLD_WIDTH, -WORLD_HEIGHT), Vector2(WORLD_WIDTH * 0.99f, WORLD_HEIGHT * 0.985f)));

	return cameras;
}
