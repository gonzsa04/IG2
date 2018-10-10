#pragma once
#include "GameObject.h"
#include "OgreInput.h" 
#include "SDL_keycode.h"
#include <OgreAnimationState.h>

using namespace std;

class Sinbad : public GameObject, public OgreBites::InputListener
{
private:
	Ogre::AnimationState* animState;
	Ogre::Entity *espada1, *espada2;
public:
	Sinbad(Ogre::SceneNode* sceneNode, std::string mesh) : GameObject(sceneNode, mesh) {
		espada1 = sceneNode_->getCreator()->createEntity("Sword.mesh");
		espada2 = sceneNode_->getCreator()->createEntity("Sword.mesh");
		ent->attachObjectToBone("Handle.R", espada1);
		ent->attachObjectToBone("Sheath.L", espada2);
	}
	void setAnimation(string name) { 
		animState = ent->getAnimationState(name);
		animState->setEnabled(true);
		animState->setLoop(true);
	};
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) {
		if (evt.keysym.sym == SDLK_r) { 
			if (animState->getAnimationName() == "Dance") setAnimation("RunBase");
			else setAnimation("Dance");
		}
		return true;
	}

	//metodo heredado de InputListener
	virtual void frameRendered(const Ogre::FrameEvent & evt) {
		animState->addTime(evt.timeSinceLastFrame);
	}
	virtual ~Sinbad() {}
};

