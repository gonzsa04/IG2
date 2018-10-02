#pragma once

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreMeshManager.h"
#include <string>

class GameObject
{
private:
	Ogre::SceneNode* sceneNode_;         //nodo de la escena que apuntara a GameObject
public:
	GameObject() {}

	GameObject(Ogre::SceneNode* sceneNode, std::string mesh) : sceneNode_(sceneNode) {
		Ogre::Entity* ent = sceneNode_->getCreator()->createEntity(mesh);             //inicializamos la entidad ent con la malla indicada
		sceneNode_->attachObject(ent);												  //hacemos que el nodo mGameObjectNode apunte a la entidad ent
	}

	void setPosition(int x, int y, int z) {
		sceneNode_->setPosition(x, y, z);
	}

	void setScale(int x, int y, int z) {
		sceneNode_->setScale(x, y, z);
	}

	~GameObject() {
		delete sceneNode_;
		sceneNode_ = nullptr;
	}
};
