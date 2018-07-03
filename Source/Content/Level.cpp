
#include "Level.h"
#include "../GameMain/Globals.h"
#include "../Graphics/Camera.h"

std::vector<Camera> Level::CreateCameraArray() {

	// Initialize the camera array
	std::vector<Camera> cameras;
	cameras.reserve(9);

	cameras.emplace_back(Vector2(0, 0), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT);		 // Center-Center
	cameras.emplace_back(Vector2(-WORLD_WIDTH, 0), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT);	 // Left-Center
	cameras.emplace_back(Vector2(WORLD_WIDTH, 0), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT);	 // Right-Center
	cameras.emplace_back(Vector2(-WORLD_WIDTH, WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT); // Left-Top
	cameras.emplace_back(Vector2(0, WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT);    // Center-Top
	cameras.emplace_back(Vector2(WORLD_WIDTH, WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT);  // Right-Top
	cameras.emplace_back(Vector2(-WORLD_WIDTH, -WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT);// Left-Bottom
	cameras.emplace_back(Vector2(0, -WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT);	 // Center-Bottom
	cameras.emplace_back(Vector2(WORLD_WIDTH, -WORLD_HEIGHT), Vector2(WORLD_WIDTH*0.99f, WORLD_HEIGHT*0.985f), WINDOW_WIDTH, WINDOW_HEIGHT); // Right-Bottom

	return cameras;
}
