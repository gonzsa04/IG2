#pragma once
#include "GameObject.h"
#include "OgreInput.h" 
#include "SDL_keycode.h"
#include <OgreAnimationState.h>
#include <vector>
#include <OgreFrameListener.h>
#include <OgreAnimation.h>
#include <OgreKeyFrame.h>

using namespace std;
using namespace Ogre;

enum ActualAnim {
	DANCING,
	RUNNING
};

class Sinbad : public GameObject, public OgreBites::InputListener
{
private:
	Ogre::Entity *espada1, *espada2;
	std::vector<Ogre::AnimationState*> animations;
	ActualAnim actualAnim = DANCING;

	void setAnimation(string name, bool b);

public:
	Sinbad(Ogre::SceneNode* sceneNode, std::string mesh, float duracion = 3.0, float tamDesp = 10.0);

	void setAnimation(ActualAnim newAnim) {              // cambia de animacion, la activa y hace que se repita
		actualAnim = newAnim;
	};

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);

	void updateAnim();

	//metodo heredado de InputListener. Le indica al gestor de la animacion el tiempo transcurrido para que esta avance
	virtual void frameRendered(const Ogre::FrameEvent & evt) {
		for (int i = 0; i < animations.size(); i++) {
			if(animations[i]->getEnabled()) animations[i]->addTime(evt.timeSinceLastFrame);
		}
	}
	virtual ~Sinbad() {}
};

