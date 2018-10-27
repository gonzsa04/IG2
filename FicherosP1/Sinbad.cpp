#include "Sinbad.h"

Sinbad::Sinbad(Ogre::SceneNode* sceneNode, std::string mesh, float duracion, PosicionesAnimacion posAnim) : GameObject(sceneNode, mesh), duracion_(duracion) {
	sceneNode_->setPosition(450, 100, -200);                         // cambiamos posicion y escala del nodo mSinbadNode respecto a su padre,
	sceneNode_->setScale(20, 20, 20);                                // el nodo mPlano

	espada1 = sceneNode_->getCreator()->createEntity("Sword.mesh");  // creamos dos espadas y las situamos
	espada2 = sceneNode_->getCreator()->createEntity("Sword.mesh");
	ent->attachObjectToBone("Sheath.R", espada1);                    // en sus fundas
	ent->attachObjectToBone("Sheath.L", espada2);


	// ------------------------------------------------------ANIMACIONES--------------------------------------------------------
	animations.resize(5);                                            // introducimos todas las animaciones ya creadas que podran hacer a sinbad:
	animations[0] = ent->getAnimationState("Dance");                 // bailar
	animations[1] = ent->getAnimationState("RunBase");               // correr->piernas
	animations[2] = ent->getAnimationState("RunTop");                // correr brazos

	sceneNode_->setInitialState();       // establecemos la transformacion actual del nodo como el estado inicial de la animacion
										 // a partir de ese transform se daran las transformaciones de la anim, por eso debe estar inicializado antes

	createRunPlaneAnim(posAnim);         // creamos una animacion personalizada que hara que recorra el plano
	updateAnim();                        // actualizamos animacion actual (inicialmente DANCING)
}

void Sinbad::setAnimation(string name, bool b, bool loop) {  // establece una animacion a true o false y hace que sea loop o no
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i] != nullptr && animations[i]->getAnimationName() == name) {
			animations[i]->setEnabled(b);
			animations[i]->setLoop(loop);
		}
	}
};

void Sinbad::createRunPlaneAnim(PosicionesAnimacion posAnim) {  // crea la animacion de correr alrededor del plano
	Animation* animation = sceneNode_->getCreator()->createAnimation("recorrePlano", duracion_); // creamos una anim con un nombre y duracion
	NodeAnimationTrack* track = animation->createNodeTrack(0);                                   // y un camino de nodos que sinbad ira siguiendo
	track->setAssociatedNode(sceneNode_);

	Vector3 keyframePos;                                     // posicion que tendra sinbad en cada keyframe
	Real longitudPaso = duracion_ / 8.0;                     // longitud que avanzara cada tramo (de un kf a otro)
	TransformKeyFrame * kf;                                  // el camino constara de 8 keyFrames
	Ogre::Vector3 src(0, 0, 1);                              // su orientacion inicial (la que llevase al iniciar la animacion)
	Ogre::Vector3 dest(0, 0, -1);                            // su orientacion destino (mirando hacia -Z)
	Ogre::Quaternion quat = src.getRotationTo(dest);         // lo que sinbad debe girar para pasar de su orientacion inicial a la destino

	kf = track->createNodeKeyFrame(longitudPaso * 0);        // Keyframe 0: origen
	keyframePos = { 0, 0, 0 };                               // se mantiene en la posicion en la que esta
	kf->setTranslate(keyframePos);

	kf = track->createNodeKeyFrame(longitudPaso * 1);        // Keyframe 1: llega a la primera esquina
	keyframePos += Ogre::Vector3::UNIT_Z * posAnim.anchuraZ; // recorre en Z la anchura del plano
	kf->setTranslate(keyframePos);

	kf = track->createNodeKeyFrame(longitudPaso * 2);        // Keyframe 2: dobla la primera esquina
	keyframePos += Ogre::Vector3::UNIT_Z * posAnim.diagonal; // recorre en X y en Z la sitancia necesaria para pasar la diagonal
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_X * posAnim.diagonal;
	kf->setTranslate(keyframePos);

	dest = { -1, 0, 0 };                                     // su nueva orientacion destino sera mirando hacia -X
	quat = src.getRotationTo(dest);                          // calculamos lo que debe girar para llegar a ella
	kf->setRotation(quat);                                   // y le giramos

	kf = track->createNodeKeyFrame(longitudPaso * 3);        // Keyframe 3: llega a la segunda esquina
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_X * posAnim.largoX; // recorre en -X el largo del plano
	kf->setTranslate(keyframePos);

	kf->setRotation(quat);                                   // le giramos para que mantenga su orientacion hasta la proxima vez que gire

	kf = track->createNodeKeyFrame(longitudPaso * 4);        // Keyframe 2: dobla la segunda esquina 
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Z * posAnim.diagonal;
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_X * posAnim.diagonal;
	kf->setTranslate(keyframePos);

	dest = { 0, 0, -1 };                                     // nueva orientacion (mirando a -Z)
	quat = src.getRotationTo(dest);
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 5);        // llega a la tercera esquina
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Z * posAnim.anchuraZ;
	kf->setTranslate(keyframePos);

	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 6);        // dobla la tercera esquina
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Z * posAnim.diagonal;
	keyframePos += Ogre::Vector3::UNIT_X * posAnim.diagonal;
	kf->setTranslate(keyframePos); // Origen

	dest = { 1, 0, 0 };                                      // nueva orientacion (mirando a X)
	quat = src.getRotationTo(dest);
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 7);        // llega a la cuarta esquina
	keyframePos += Ogre::Vector3::UNIT_X * posAnim.largoX;
	kf->setTranslate(keyframePos);

	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 8);        // dobla la cuarta esquina
	keyframePos += Ogre::Vector3::UNIT_Z * posAnim.diagonal;
	keyframePos += Ogre::Vector3::UNIT_X * posAnim.diagonal;
	kf->setTranslate(keyframePos); // Origen

	dest = { 0, 0, 1 };                                      // la orientacion vuelve a ser la misma que la inicial
	quat = src.getRotationTo(dest);
	kf->setRotation(quat);

	animations[3] = sceneNode_->getCreator()->createAnimationState("recorrePlano");  // lo añadimos al resto de animaciones
}

void Sinbad::createDyingAnim() {  // crea la animacion de ir hacia la bomba y morir al llegar a ella
	Animation* animation = sceneNode_->getCreator()->createAnimation("dying", duracion_);
	NodeAnimationTrack* track = animation->createNodeTrack(0);
	track->setAssociatedNode(sceneNode_);

	Vector3 keyframePos;
	Real longitudPaso = duracion_/3;
	TransformKeyFrame * kf;
	Ogre::Vector3 src = sceneNode_->_getDerivedOrientation()*Vector3::UNIT_Z;                // orientacion inicial que lleve
	Ogre::Vector3 dest = { -sceneNode_->getPosition().x, 0 , -sceneNode_->getPosition().z }; //orientacion final la de la bomba (en el 0, 0, 0)
	Ogre::Quaternion quat = src.getRotationTo(dest);

	kf = track->createNodeKeyFrame(longitudPaso * 0); // Keyframe 0: origen
	keyframePos = { 0, 0, 0 };
	kf->setTranslate(keyframePos);

	kf = track->createNodeKeyFrame(longitudPaso * 1); // Keyframe 1: gira para quedar orientado hacia la bomba (aun parado)
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 2); // Keyframe 2: va hacia la bomba (manteniendo la misma orientacion)
	keyframePos -= { sceneNode_->getPosition().x, 0, sceneNode_->getPosition().z };
	kf->setTranslate(keyframePos); // Arriba
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 2); // Keyframe 3: al llegar a ella cae al suelo boca arriba
	keyframePos.y -= 100;
	kf->setTranslate(keyframePos);
	quat = { (Real)sqrt(0.5), -(Real)sqrt(0.5), 0, 0 };
	kf->setRotation(quat);

	kf = track->createNodeKeyFrame(longitudPaso * 3); // Keyframe 4: se le lleva la corriente del agua (manteniendose boca arriba)
	keyframePos += { 800, 0, 0 };
	kf->setTranslate(keyframePos); // Origen
	kf->setRotation(quat);

	animations[4] = sceneNode_->getCreator()->createAnimationState("dying");  // lo añadimos al resto de animaciones
}

bool Sinbad::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (!muerto) {
		if (evt.keysym.sym == SDLK_r) {      // si pulsamos "r" pasara de bailar a correr y viceversa
			if (actualAnim == DANCING) 	setAnimation(RUNNING);
			else if (actualAnim == RUNNING) setAnimation(DANCING);
		}
		else if (evt.keysym.sym == SDLK_b) { // si pulsamos "b" pasara de lo que este haciendo a morir
			muerto = true;
			sceneNode_->setInitialState();   // volvemos a actualizar el initialState para que tenga en cuenta la pos actual y no la inicial
			createDyingAnim();               // una vez hecho esto, creamos la animacion
			setAnimation(DYING);
		}
	}
	return true;
}

void Sinbad::updateAnim() {      // dependiendo del estado en que nos encontremos activara una animacion u otra
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