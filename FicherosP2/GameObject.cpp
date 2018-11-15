#include "GameObject.h"
std::vector<GameObject*> GameObject::appListeners;

GameObject::GameObject(Ogre::SceneNode* sceneNode, std::string mesh) : sceneNode_(sceneNode) {
	ent = sceneNode_->getCreator()->createEntity(mesh);             // inicializamos la entidad ent con la malla indicada
	sceneNode_->attachObject(ent);									// hacemos que el nodo sceneNode apunte a la entidad ent
}

void GameObject::fireAppEvent(TipoEvent evt, GameObject* go) {
	for (int i = 0; i < appListeners.size(); i++)
		appListeners[i]->receive(evt, go);
}
