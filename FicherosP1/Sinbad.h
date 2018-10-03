#pragma once
#include "GameObject.h"

class Sinbad : public GameObject
{
public:
	Sinbad(Ogre::SceneNode* sceneNode, std::string mesh) : GameObject(sceneNode, mesh) {}
	virtual ~Sinbad() {}
};

