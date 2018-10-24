#include "Sinbad.h"

Sinbad::Sinbad(Ogre::SceneNode* sceneNode, std::string mesh, float duracion, PosicionesAnimacion posAnim) : GameObject(sceneNode, mesh), duracion_(duracion) {
	sceneNode_->setPosition(450, 100, -200);                         // cambiamos posicion y escala del nodo mSinbadNode respecto a su padre,
	sceneNode_->setScale(20, 20, 20);                                // el nodo mPlano

	espada1 = sceneNode_->getCreator()->createEntity("Sword.mesh");  // creamos dos espadas y las situamos
	espada2 = sceneNode_->getCreator()->createEntity("Sword.mesh");
	ent->attachObjectToBone("Sheath.R", espada1);                    // en sus fundas
	ent->attachObjectToBone("Sheath.L", espada2);


	// ------------------------------------------------------ANIMACIONES--------------------------------------------------------
	animations.resize(5);                                            // introducimos todas las animaciones ya creadas que podra hacer sinbad
	animations[0] = ent->getAnimationState("Dance");                 // bailar
	animations[1] = ent->getAnimationState("RunBase");               // correr->piernas
	animations[2] = ent->getAnimationState("RunTop");                // correr brazos

	// creamos una animacion personalizada que hara que recorra el plano
	sceneNode_->setInitialState();       // establecemos la transformacion actual del nodo como el estado inicial de la animacion
										 // a partir de ese transform se daran las transformaciones de la anim, por eso debe estar inicializado antes
	createRunPlaneAnim(posAnim);

	updateAnim();    // actualizamos animacion actual
}

void Sinbad::setAnimation(string name, bool b, bool loop) {         // establece la animacion dada a true o false
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i] != nullptr && animations[i]->getAnimationName() == name) {
			animations[i]->setEnabled(b);
			animations[i]->setLoop(loop);
		}
	}
};

void Sinbad::createRunPlaneAnim(PosicionesAnimacion posAnim) {
	Animation* animation = sceneNode_->getCreator()->createAnimation("recorrePlano", duracion_);
	NodeAnimationTrack* track = animation->createNodeTrack(0);
	track->setAssociatedNode(sceneNode_);
	Vector3 keyframePos;
	Real longitudPaso = duracion_ / 8.0; // uniformes
	TransformKeyFrame * kf; // 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)
	Ogre::Vector3 src(0, 0, 1); // eje Y
	Ogre::Vector3 dest(0, 0, -1);
	Ogre::Quaternion quat = src.getRotationTo(dest);

	kf = track->createNodeKeyFrame(longitudPaso * 0); // Keyframe 0: origen
	keyframePos = { 0, 0, 0 };
	kf->setTranslate(keyframePos); // Origen: Vector3

	kf = track->createNodeKeyFrame(longitudPaso * 1); // Keyframe 1: arriba
	keyframePos += Ogre::Vector3::UNIT_Z * posAnim.anchuraZ;
	kf->setTranslate(keyframePos); // Arriba

	kf = track->createNodeKeyFrame(longitudPaso * 2); // Keyframe 2: origen
	keyframePos += Ogre::Vector3::UNIT_Z * posAnim.diagonal;
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_X * posAnim.diagonal;
	kf->setTranslate(keyframePos); // Origen

	dest = { -1, 0, 0 };
	quat = src.getRotationTo(dest);
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 3); // Keyframe 1: arriba
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_X * posAnim.largoX;
	kf->setTranslate(keyframePos); // Arriba

	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 4); // Keyframe 2: origen
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Z * posAnim.diagonal;
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_X * posAnim.diagonal;
	kf->setTranslate(keyframePos); // Origen

	dest = { 0, 0, -1 };
	quat = src.getRotationTo(dest);
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 5); // Keyframe 1: arriba
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Z * posAnim.anchuraZ;
	kf->setTranslate(keyframePos); // Arriba

	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 6); // Keyframe 2: origen
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Z * posAnim.diagonal;
	keyframePos += Ogre::Vector3::UNIT_X * posAnim.diagonal;
	kf->setTranslate(keyframePos); // Origen

	dest = { 1, 0, 0 };
	quat = src.getRotationTo(dest);
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 7); // Keyframe 1: arriba
	keyframePos += Ogre::Vector3::UNIT_X * posAnim.largoX;
	kf->setTranslate(keyframePos);

	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 8); // Keyframe 2: origen
	keyframePos += Ogre::Vector3::UNIT_Z * posAnim.diagonal;
	keyframePos += Ogre::Vector3::UNIT_X * posAnim.diagonal;
	kf->setTranslate(keyframePos); // Origen

	dest = { 0, 0, 1 };
	quat = src.getRotationTo(dest);
	kf->setRotation(quat);

	animations[3] = sceneNode_->getCreator()->createAnimationState("recorrePlano");  // lo añadimos al resto de animaciones
}

void Sinbad::createDyingAnim() {
	Animation* animation = sceneNode_->getCreator()->createAnimation("dying", duracion_);
	NodeAnimationTrack* track = animation->createNodeTrack(0);
	track->setAssociatedNode(sceneNode_);
	Vector3 keyframePos;
	Real longitudPaso = duracion_/3; // uniformes
	TransformKeyFrame * kf; // 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)
	Ogre::Vector3 src = sceneNode_->_getDerivedOrientation()*Vector3::UNIT_Z;
	Ogre::Vector3 dest = { -sceneNode_->getPosition().x, 0 , -sceneNode_->getPosition().z };
	Ogre::Quaternion quat = src.getRotationTo(dest);

	kf = track->createNodeKeyFrame(longitudPaso * 0); // Keyframe 0: origen
	keyframePos = { 0, 0, 0 };
	kf->setTranslate(keyframePos); // Origen: Vector3

	kf = track->createNodeKeyFrame(longitudPaso * 1); // Keyframe 0: origen
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 2); // Keyframe 1: arriba
	keyframePos -= { sceneNode_->getPosition().x, 0, sceneNode_->getPosition().z };
	kf->setTranslate(keyframePos); // Arriba
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 2); // Keyframe 2: origen
	keyframePos.y -= 100;
	kf->setTranslate(keyframePos); // Arriba
	quat = { (Real)sqrt(0.5), -(Real)sqrt(0.5), 0, 0 };
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 3); // Keyframe 2: origen
	keyframePos += { 800, 0, 0 };
	kf->setTranslate(keyframePos); // Origen
	kf->setRotation(quat);

	animations[4] = sceneNode_->getCreator()->createAnimationState("dying");  // lo añadimos al resto de animaciones
}

bool Sinbad::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == SDLK_r) { // si pulsamos "r" pasara de bailar a correr y viceversa
		if (actualAnim == DANCING) 	actualAnim = RUNNING;
		else if (actualAnim == RUNNING) actualAnim = DANCING;
		updateAnim();
	}
	else if (evt.keysym.sym == SDLK_b) { // si pulsamos "b" pasara de lo que este haciendo a morir
		actualAnim = DYING;
		sceneNode_->setInitialState();
		createDyingAnim();
		updateAnim();
	}
	return true;
}

void Sinbad::updateAnim() {         // dependiendo del estado en que nos encontremos activara una animacion u otra
	if (actualAnim == RUNNING) {
		setAnimation("RunBase", true, true);
		setAnimation("RunTop", true, true);
		setAnimation("recorrePlano", true, true);
		setAnimation("Dance", false, false);
		ent->detachObjectFromBone(espada1);
		ent->attachObjectToBone("Handle.R", espada1);
	}
	else if (actualAnim == DANCING) {
		setAnimation("RunBase", false, false);
		setAnimation("RunTop", false, false);
		setAnimation("recorrePlano", false, false);
		setAnimation("Dance", true, true);
		ent->detachObjectFromBone(espada1);
		ent->attachObjectToBone("Sheath.R", espada1);
	}
	else if (actualAnim == DYING) {
		setAnimation("RunBase", true, true);
		setAnimation("dying", true, false);
		setAnimation("RunTop", false, false);
		setAnimation("recorrePlano", false, false);
		setAnimation("Dance", false, false);
		ent->detachObjectFromBone(espada1);
		ent->attachObjectToBone("Handle.R", espada1);
		ent->detachObjectFromBone(espada2);
		ent->attachObjectToBone("Handle.L", espada2);
	}
}