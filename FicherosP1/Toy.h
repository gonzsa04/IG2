#pragma once
#include "OgreInput.h" 
#include "SDL_keycode.h"
#include "GameObject.h"

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
public:
	Toy(Ogre::SceneNode* sceneNode, std::string mesh) {
		//creacion de entidades
		//Accedemos al creador del nodo recibido, que sera mSM, y le decimos que use la malla de esfera
		cabeza = sceneNode->getCreator()->createEntity(mesh);  //si no se encuentra una malla, se busca en media/models
		nariz = sceneNode->getCreator()->createEntity(mesh);   //y se copia en media/IG2App para que pueda ser encontrada
		cuerpo = sceneNode->getCreator()->createEntity(mesh);
		ombligo = sceneNode->getCreator()->createEntity(mesh);

		//jerarquia de los nodos
		mCuello = sceneNode->createChildSceneNode("nCuello");     //el cuello sera el hijo directo del nodo recibido y el padre de los demas
		mCabeza = mCuello->createChildSceneNode("nCabeza");       //la cabeza y el cuerpo seran hijos del cuello, y a su vez la nariz sera hijo
		mCabeza->attachObject(cabeza);                            //de la cabeza y el ombligo hijo del cuerpo
		mNariz = mCabeza->createChildSceneNode("nNariz");
		mNariz->attachObject(nariz);
		mCuerpo = mCuello->createChildSceneNode("nCuerpo");
		mCuerpo->attachObject(cuerpo);
		mOmbligo = mCuerpo->createChildSceneNode("nOmbligo");
		mOmbligo->attachObject(ombligo);

		//posicionamiento y escalado relativos (cada uno respecto a su padre)
		mCuello->setPosition(-300, 0, 0);
		mNariz->setPosition(100, 15, 0);
		mNariz->setScale(0.175, 0.175, 0.175);
		mCabeza->setPosition(0, 140, 0);
		mCabeza->setScale(0.3, 0.3, 0.3);
		mCuerpo->setPosition(0, 60, 0);
		mCuerpo->setScale(0.6, 0.6, 0.6);
		mOmbligo->setPosition(100, 0, 0);
		mOmbligo->setScale(0.175, 0.175, 0.175);
	}

	//metodo heredado de InputListener
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) {
		if (evt.keysym.sym == SDLK_t) {        //si pulsamos t se mueve discretamente
			movimiento();
		}
		else if (evt.keysym.sym == SDLK_y) {   //si pulsamos y toy se para/anda
			parado = !parado;
		}
		else if (evt.keysym.sym == SDLK_v) {   //si pulsamos v toy gira 45 grados
			Giro45();
		}
		return true;
	}

	//metodo heredado de InputListener
	//si toy no esta parado, se mueve de forma continua utilizando el t desde el ultimo frame
	virtual void frameRendered(const Ogre::FrameEvent & evt) {
		if (!parado) movimiento(evt.timeSinceLastFrame * 10);
	}

	void movimiento(float vel = 1) {
		mCabeza->yaw(Ogre::Radian(0.3*vel));                    //la cabeza gira en el eje y
		mCuerpo->roll(Ogre::Radian(-0.3*vel));                  //el cuerpo gira en el eje z (en la direccion que se mueve toy)
		mCuello->translate(2*vel, 0, 0, Ogre::Node::TS_LOCAL);  //trasladamos toy entero en x (LOCAL para que se actualice de si mismo y no de su padre)
	}

	void Giro45() {
		mCuello->rotate(Ogre::Vector3(0,1,0), Ogre::Degree(45));
	}

	virtual ~Toy() {}
};

