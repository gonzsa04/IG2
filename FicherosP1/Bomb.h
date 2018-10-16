#pragma once
#include "GameObject.h"
class Bomb : public GameObject
{
public:
	Bomb(Ogre::SceneNode* sceneNode, std::string mesh = "uv_sphere.mesh") : GameObject(sceneNode, mesh) {}
	virtual ~Bomb() {}
};

