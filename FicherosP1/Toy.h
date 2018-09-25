#pragma once

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreMeshManager.h"

class Toy
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
		mNariz->setPosition(0, 0, 0);
		mNariz->setScale(0.1, 0.1, 0.1);
		mCabeza->setPosition(0, 0, 0);
		mCabeza->setScale(20, 20, 20);
		mCuerpo->setPosition(0, 0, 0);
		mCuerpo->setScale(20, 20, 20);
		mOmbligo->setPosition(0, 0, 0);
		mOmbligo->setScale(20, 20, 20);
	}
	~Toy() {}
};

