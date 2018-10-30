#pragma once

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreMeshManager.h"
#include <string>
#include <vector>

enum TipoEvent {
	COLISION
};

class GameObject                          // clase abstracta raíz de los objetos de la aplicación
{
private:
	static std::vector<GameObject*> appListeners;
protected:
	Ogre::SceneNode* sceneNode_;          // nodo de la escena que apuntara a GameObject
	Ogre::Entity* ent;
	bool active = true;

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

	bool getActive() { return active; }
	void setActive(bool b) { active = b; }

	static void addAppListener(GameObject* go) { appListeners.push_back(go); }
	static void fireAppEvent(TipoEvent evt, GameObject* go) { 
		for (int i = 0; i < appListeners.size(); i++)
			appListeners[i]->receive(evt, go); 
	}
	virtual void receive(TipoEvent evt, GameObject* go) = 0;

	Ogre::Entity* getEntity() { return ent; }

	~GameObject() {}
};
