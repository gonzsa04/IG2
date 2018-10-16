#include "Sinbad.h"

Sinbad::Sinbad(Ogre::SceneNode* sceneNode, std::string mesh) : GameObject(sceneNode, mesh) {
	espada1 = sceneNode_->getCreator()->createEntity("Sword.mesh");  // creamos dos espadas y las situamos
	espada2 = sceneNode_->getCreator()->createEntity("Sword.mesh");
	ent->attachObjectToBone("Sheath.R", espada1);                    // en sus fundas
	ent->attachObjectToBone("Sheath.L", espada2);
	animations.resize(3);
	animations[0] = ent->getAnimationState("Dance");
	animations[1] = ent->getAnimationState("RunBase");
	animations[2] = ent->getAnimationState("RunTop");
	updateAnim();
}

void Sinbad::setAnimation(string name, bool b) {         // cambia de animacion, la activa y hace que se repita
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i]->getAnimationName() == name) {
			animations[i]->setEnabled(b);
			animations[i]->setLoop(b);
		}
	}
};

bool Sinbad::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == SDLK_r) {           // si pulsamos "r" pasara de bailar a correr y viceversa
		if (actualAnim == DANCING) 	actualAnim = RUNNING;
		else actualAnim = DANCING;
		updateAnim();
	}
	return true;
}

void Sinbad::updateAnim() {
	if (actualAnim == RUNNING) {
		setAnimation("RunBase", true);
		setAnimation("RunTop", true);
		setAnimation("Dance", false);
		ent->detachObjectFromBone(espada1);
		ent->attachObjectToBone("Handle.R", espada1);
	}
	else if (actualAnim == DANCING) {
		setAnimation("RunBase", false);
		setAnimation("RunTop", false);
		setAnimation("Dance", true);
		ent->detachObjectFromBone(espada1);
		ent->attachObjectToBone("Sheath.R", espada1);
	}
}