#pragma once
#include "GameObject.h"
#include "OgreInput.h" 
#include "SDL_keycode.h"
#include <OgreAnimationState.h>

using namespace std;

class Sinbad : public GameObject, public OgreBites::InputListener
{
private:
	Ogre::AnimationState* animState;              // controla la animacion actual de Sinbad
	Ogre::Entity *espada1, *espada2;
public:
	Sinbad(Ogre::SceneNode* sceneNode, std::string mesh) : GameObject(sceneNode, mesh) {
		espada1 = sceneNode_->getCreator()->createEntity("Sword.mesh");  // creamos dos espadas y las situamos
		espada2 = sceneNode_->getCreator()->createEntity("Sword.mesh");
		ent->attachObjectToBone("Sheath.R", espada1);                    // en sus fundas
		ent->attachObjectToBone("Sheath.L", espada2);
	}

	void setAnimation(string name) {              // cambia de animacion, la activa y hace que se repita
		animState = ent->getAnimationState(name);
		animState->setEnabled(true);
		animState->setLoop(true);
	};

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) {
		if (evt.keysym.sym == SDLK_r) {           // si pulsamos "r" pasara de bailar a correr y viceversa
			if (animState->getAnimationName() == "Dance") {
				setAnimation("RunBase");
				//ent->attachObjectToBone("Sheath.R", espada1);   // cuando baila guarda su espada derecha
			}
			else {
				setAnimation("Dance");
				//ent->attachObjectToBone("Handle.R", espada1);   // cuando corre saca su espada derecha
			}
		}
		return true;
	}

	//metodo heredado de InputListener. Le indica al gestor de la animacion el tiempo transcurrido para que esta avance
	virtual void frameRendered(const Ogre::FrameEvent & evt) {
		animState->addTime(evt.timeSinceLastFrame);
	}
	virtual ~Sinbad() {}
};

