#include "camera.h"


Camera::Camera(glm::vec4 worldSize)
{
	this->worldSize = worldSize;
	width = worldSize.z / TILE_WIDTH;
	height = worldSize.w / TILE_HEIGHT;
}

void Camera::resize(glm::vec4 worldSize)
{
	this->worldSize = worldSize;
	width = worldSize.z / TILE_WIDTH;
	height = worldSize.w / TILE_HEIGHT;
	correct();
}

void Camera::correct()
{
	if (offset.x > -worldSize.x) offset.x = -worldSize.x;
	if (offset.x < -worldSize.x - worldSize.z + TARGET_WIDTH) offset.x = -worldSize.x - worldSize.z + TARGET_WIDTH;
	if (offset.y > -worldSize.y) offset.y = -worldSize.y;
	if (offset.y < -worldSize.y - worldSize.w + TARGET_HEIGHT) offset.y = worldSize.y - worldSize.w + TARGET_HEIGHT;

	if (worldSize.z < TARGET_WIDTH) offset.x = (TARGET_WIDTH - worldSize.z) / 2;
	if (worldSize.w < TARGET_HEIGHT) offset.y = (TARGET_HEIGHT - worldSize.w) / 2;
}

glm::vec4 Camera::getTileRect(int x, int y)
{
	return glm::vec4(worldSize.x + (x * TILE_WIDTH), worldSize.y + (y * TILE_HEIGHT),
		TILE_WIDTH, TILE_HEIGHT);
}

void Camera::target(glm::vec4 target)
{
	offset.x = -(target.x + (target.z / 2)) + (TARGET_WIDTH / 2);
	offset.y = -(target.y + (target.w / 2)) + (TARGET_HEIGHT / 2);

	correct();
}

