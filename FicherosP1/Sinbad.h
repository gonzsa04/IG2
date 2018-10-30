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

// posibles estados de animacion en los que puede estar Sinbad
enum ActualAnim {
	DANCING, 
	RUNNING,
	RUNTOBOMB,
	DYING
};

// longitudes que ha de recorrer Sinbad en su animacion de correr alrededor del plano
struct PosicionesAnimacion {
	float anchuraZ;
	float largoX;
	float diagonal;
};

class Sinbad : public GameObject, public OgreBites::InputListener
{
private:
	Ogre::Entity *espada1, *espada2;
	std::vector<Ogre::AnimationState*> animations;        // vector que guardara todas las animaciones de Sinbad
	ActualAnim actualAnim = DANCING;                      // inicialmente estara bailando
	float duracion_;                                      // duracion de la animacion de correr alrededor del plano
	bool muerto = false;                                  // sinbad morira cuando llegue a la bomba

	void setAnimation(string name, bool b, bool loop);    // establece una animacion a true o false y hace que sea loop o no
	void createRunPlaneAnim(PosicionesAnimacion posAnim); // crea la animacion de correr alrededor del plano
	void createRunToBombAnim();
	void createDyingAnim();                               // crea la animacion de ir hacia la bomba y morir al llegar a ella

public:
	Sinbad(Ogre::SceneNode* sceneNode, std::string mesh, float duracion = 5.0, PosicionesAnimacion posAnim = { 400, 600, 150 });

	void setAnimation(ActualAnim newAnim) {               // cambia el estado de la animacion actual y la actualiza
		actualAnim = newAnim;
		updateAnim();
	};

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);

	void updateAnim();

	//metodo heredado de InputListener. Le indica a las animaciones activas el tiempo transcurrido para que estas avancen
	virtual void frameRendered(const Ogre::FrameEvent & evt) {
		for (int i = 0; i < animations.size(); i++) {
			if (animations[i] != nullptr && animations[i]->getEnabled()) {
				animations[i]->addTime(evt.timeSinceLastFrame);
				if (actualAnim == RUNTOBOMB && animations[i]->hasEnded()) {
					createDyingAnim();               // una vez hecho esto, creamos la animacion
					setAnimation(DYING);
				}
			}
		}
	}

	virtual void receive(TipoEvent evt, GameObject* go) {
		if (evt == COLISION) {
			createRunToBombAnim();
			setAnimation(RUNTOBOMB);
		}
	}

	virtual ~Sinbad() {}
};

