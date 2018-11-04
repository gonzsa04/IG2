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

	// EVENTOS
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void frameRendered(const Ogre::FrameEvent & evt);
	virtual void receive(TipoEvent evt, GameObject* go);

	// ANIMACIONES
	void setAnimation(string name, bool b, bool loop);    // establece una animacion a true o false y hace que sea loop o no
	void setAnimationState(ActualAnim newAnim);           // cambia el estado de la animacion actual y la actualiza
	void updateAnim();                                    // dependiendo del estado en que nos encontremos activara una animacion u otra
	void createRunPlaneAnim(PosicionesAnimacion posAnim); // crea la animacion de correr alrededor del plano
	void createRunToBombAnim();                           // crea la animacion de ir hacia la bomba
	void createDyingAnim();                               // crea la animacion de morir al llegar a la bomba

public:
	Sinbad(Ogre::SceneNode* sceneNode, std::string mesh, float duracion = 5.0, PosicionesAnimacion posAnim = { 400, 600, 150 });
	virtual ~Sinbad() {}
};

