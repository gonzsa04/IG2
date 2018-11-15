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
	AnimationState* animState;                            // activa la animacion de vaiven arriba y abajo
	ParticleSystem* pSys = nullptr;                       // sistema de particulas para el humo

	void createAnimation(float duracion, float tamDesp);  // crea la animacion de vaiven arriba y abajo

	// EVENTOS
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void frameRendered(const Ogre::FrameEvent & evt);
	virtual void receive(TipoEvent evt, GameObject* go);

public:
	Bomb(SceneNode* sceneNode, std::string mesh = "uv_sphere.mesh", std::string materialName = "Bomba", float duracion = 3.0, float tamDesp = 10.0);
	virtual ~Bomb() {}
};

