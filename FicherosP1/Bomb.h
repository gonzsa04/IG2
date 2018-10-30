#pragma once
#include "GameObject.h"
#include "OgreInput.h"
#include "SDL_keycode.h"
#include <OgreAnimation.h>
#include <OgreKeyFrame.h>
#include <OgreAnimationState.h>
#include <OgreFrameListener.h>
#include <OgreParticleSystem.h>

using namespace Ogre;

class Bomb : public GameObject, public OgreBites::InputListener
{
private:
	AnimationState* animState;   // animacion de vaiven arriba y abajo
	ParticleSystem* pSys = nullptr;

public:
	Bomb(SceneNode* sceneNode, std::string mesh = "uv_sphere.mesh", float duracion = 3.0, float tamDesp = 10.0, std::string materialName = "Bomba");

	//metodo heredado de InputListener
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) {
		if (evt.keysym.sym == SDLK_b) {        // al pulsar 'b' la bomba explota
			pSys->setEmitting(true);
		}
		return true;
	}

	virtual void frameRendered(const Ogre::FrameEvent & evt) {
		animState->addTime(evt.timeSinceLastFrame);  // hacemos que la animacion avance
	}

	virtual void receive(TipoEvent evt, GameObject* go) {
		if (evt == COLISION)pSys->setEmitting(true);
	}

	virtual ~Bomb() {}
};

