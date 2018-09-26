#pragma once

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreMeshManager.h"
#include "OgreInput.h" 
#include "SDL_keycode.h"

class Toy: public OgreBites::InputListener
{
private:
	Ogre::SceneNode* sceneNode_;
	Ogre::Entity* cabeza;
	Ogre::Entity* nariz;
	Ogre::Entity* cuerpo;
	Ogre::Entity* ombligo;
	Ogre::SceneNode* mCuello;
	Ogre::SceneNode* mCabeza;
	Ogre::SceneNode* mNariz;
	Ogre::SceneNode* mCuerpo;
	Ogre::SceneNode* mOmbligo;
	bool parado = true;
public:
	Toy(Ogre::SceneNode* sceneNode): sceneNode_(sceneNode) {
		//creacion de entidades
		cabeza = sceneNode->getCreator()->createEntity("sphere.mesh");
		nariz = sceneNode->getCreator()->createEntity("sphere.mesh");
		cuerpo = sceneNode->getCreator()->createEntity("sphere.mesh");
		ombligo = sceneNode->getCreator()->createEntity("sphere.mesh");

		//jerarquia de los nodos
		mCuello = sceneNode->createChildSceneNode("nCuello");
		mCabeza = mCuello->createChildSceneNode("nCabeza");
		mCabeza->attachObject(cabeza);
		mNariz = mCabeza->createChildSceneNode("nNariz");
		mNariz->attachObject(nariz);
		mCuerpo = mCuello->createChildSceneNode("nCuerpo");
		mCuerpo->attachObject(cuerpo);
		mOmbligo = mCuerpo->createChildSceneNode("nOmbligo");
		mOmbligo->attachObject(ombligo);

		//posicionamiento y escalado relativos
		mNariz->setPosition(100, 15, 0);
		mNariz->setScale(0.175, 0.175, 0.175);
		mCabeza->setPosition(0, 140, 0);
		mCabeza->setScale(0.3, 0.3, 0.3);
		mCuerpo->setPosition(0, 60, 0);
		mCuerpo->setScale(0.6, 0.6, 0.6);
		mOmbligo->setPosition(100, 0, 0);
		mOmbligo->setScale(0.175, 0.175, 0.175);
	}

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) {
		if (evt.keysym.sym == SDLK_t) {
			movimiento();
		}
		else if (evt.keysym.sym == SDLK_y) {
			parado = !parado;
		}
		else if (evt.keysym.sym == SDLK_v) {
			Giro45();
		}
		return true;
	}

	virtual void frameRendered(const Ogre::FrameEvent & evt) {
		if (!parado) movimiento(evt.timeSinceLastFrame * 10);
	}

	void movimiento(float vel = 1) {
		mCabeza->yaw(Ogre::Radian(0.3*vel));
		mCuerpo->roll(Ogre::Radian(-0.3*vel));
		mCuello->translate(2*vel, 0, 0, Ogre::Node::TS_LOCAL);
	}

	void Giro45() {
		mCuello->rotate(Ogre::Vector3(0,1,0), Ogre::Degree(45));
	}

	~Toy() {}
};

