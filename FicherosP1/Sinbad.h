#pragma once

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreMeshManager.h"
#include "GameObject.h"

class Sinbad : public GameObject
{
public:
	Sinbad(Ogre::SceneNode* sceneNode, std::string mesh) :GameObject(sceneNode, mesh) {}
	virtual ~Sinbad() {}
};

