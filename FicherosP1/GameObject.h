#pragma once

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreMeshManager.h"
#include <string>
#include <vector>

enum TipoEvent { // tipos de evento que puede haber
	COLISION
};

class GameObject                                   // clase abstracta, padre de los objetos de la aplicación
{
private:
	static std::vector<GameObject*> appListeners;  // vector de oyentes
protected:
	Ogre::SceneNode* sceneNode_;                   // nodo de la escena que apuntara a GameObject
	Ogre::Entity* ent;
	bool active = true;

public:
	GameObject() {}

	GameObject(Ogre::SceneNode* sceneNode, std::string mesh);

	void setPosition(int x, int y, int z) { sceneNode_->setPosition(x, y, z); }
	void setScale(int x, int y, int z) { sceneNode_->setScale(x, y, z); }

	bool getActive() { return active; }
	void setActive(bool b) { active = b; }

	// eventos
	static void addAppListener(GameObject* go) { appListeners.push_back(go); } // añade un nuevo oyente
	static void fireAppEvent(TipoEvent evt, GameObject* go); // lanza un evento y hace que todos sus oyentes lo reciban
	virtual void receive(TipoEvent evt, GameObject* go) = 0; // metodo recibidor de eventos (las clases heredadas lo redefiniran)

	Ogre::Entity* getEntity() { return ent; }

	virtual ~GameObject() {}
};
