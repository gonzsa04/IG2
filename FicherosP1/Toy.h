#pragma once
#include "OgreInput.h" 
#include "SDL_keycode.h"
#include "GameObject.h"
#include <OgreFrameListener.h>

class Toy: public OgreBites::InputListener, public GameObject
{
private:
	//partes del cuerpo (entidades)
	Ogre::Entity* cabeza;
	Ogre::Entity* nariz;
	Ogre::Entity* cuerpo;
	Ogre::Entity* ombligo;

	//nodos que apuntan a esas partes del cuerpo (habra una jerarquia entre ellos)
	Ogre::SceneNode* mCuello;
	Ogre::SceneNode* mCabeza;
	Ogre::SceneNode* mNariz;
	Ogre::SceneNode* mCuerpo;
	Ogre::SceneNode* mOmbligo;

	bool parado = true;

	// EVENTOS
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void frameRendered(const Ogre::FrameEvent & evt);
	virtual void receive(TipoEvent evt, GameObject* go);

	// COMPORTAMIENTO
	void movimiento(float vel = 1);

	void Giro45() {  // giramos toy entero en el eje y 45 grados
		mCuello->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(45));
	}

	void setInvisible();

public:
	Toy(Ogre::SceneNode* sceneNode, std::string mesh);

	virtual Ogre::Entity* getEntity() { return cuerpo; }

	virtual ~Toy() {}
};

