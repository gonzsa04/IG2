#pragma once

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreMeshManager.h"
#include <string>

class GameObject                          // clase abstracta ra�z de los objetos de la aplicaci�n
{
protected:
	Ogre::SceneNode* sceneNode_;          // nodo de la escena que apuntara a GameObject
	Ogre::Entity* ent;
public:
	GameObject() {}

	GameObject(Ogre::SceneNode* sceneNode, std::string mesh) : sceneNode_(sceneNode) {
		ent = sceneNode_->getCreator()->createEntity(mesh);             //inicializamos la entidad ent con la malla indicada
		sceneNode_->attachObject(ent);												  //hacemos que el nodo sceneNode apunte a la entidad ent
	}

	void setPosition(int x, int y, int z) {
		sceneNode_->setPosition(x, y, z);
	}

	void setScale(int x, int y, int z) {
		sceneNode_->setScale(x, y, z);
	}

	~GameObject() {}
};
