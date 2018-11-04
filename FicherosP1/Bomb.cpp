#include "Bomb.h"

Bomb::Bomb(SceneNode* sceneNode, std::string mesh, std::string materialName, float duracion, float tamDesp) : 
	GameObject(sceneNode, mesh) {

	sceneNode_->setScale(0.3, 0.3, 0.3);  // modificamos escala
	ent->setMaterialName(materialName);   // y añadimos el material deseado

	//-------------------------------------------------------PARTICULAS---------------------------------------------------------

	pSys = sceneNode_->getCreator()->createParticleSystem("parSys", "Smoke");
	sceneNode_->attachObject(pSys);
	pSys->setEmitting(false);  // inicialmente desactivada

	// ------------------------------------------------------ANIMACIONES--------------------------------------------------------
	
	createAnimation(duracion, tamDesp);   // creamos una animacion personalizada para que parezca que flota en el agua

	// activamos la animacion que hemos creado y hacemos que se repita en loop
	animState = sceneNode_->getCreator()->createAnimationState("flota");
	animState->setEnabled(true);
	animState->setLoop(true);
}


// ------------------------------------------------------EVENTOS--------------------------------------------------------

bool Bomb::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == SDLK_b) {                   // al pulsar 'b' la bomba explota
		pSys->setEmitting(true);
	}
	return true;
}

void Bomb::frameRendered(const Ogre::FrameEvent & evt) {
	animState->addTime(evt.timeSinceLastFrame);       // hacemos que la animacion avance
}

void Bomb::receive(TipoEvent evt, GameObject* go) {   // redefinido de GameObject
	if (evt == COLISION) {
		setActive(false);
		pSys->setEmitting(true); // si el evento es de colision, empieza a salir humo
	}
}

//----------------------------------------------------------------------------------------------------------------------

void Bomb::createAnimation(float duracion, float tamDesp) {  // crea la animacion de vaiven arriba y abajo
	sceneNode_->setInitialState();       // establecemos la transformacion actual del nodo como el estado inicial de la animacion
										 // a partir de ese transform se daran las transformaciones de la anim, por eso debe estar inicializado antes

	Animation* animation = sceneNode_->getCreator()->createAnimation("flota", duracion); // creamos una animacion con un nombre y una duracion
	NodeAnimationTrack* track = animation->createNodeTrack(0);                           // establecemos un camino 0 para esa animacion
	track->setAssociatedNode(sceneNode_);                                                // a ese camino la asociamos sceneNode_ como punto de inicio
	Vector3 keyframePos;                                                                 // posicion de cada keyFrame de la animacion
	Real longitudPaso = duracion / 4.0;                                                  // longitud que se avanzara en cada paso
	TransformKeyFrame * kf;                                                              // tendra 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)

	// para cada paso de la animacion creamos un keyFrame, actualizamos la posicion que debe tener en ese keyFrame y se lo aplicamos
	kf = track->createNodeKeyFrame(longitudPaso * 0);                                    // Keyframe 0: origen
	keyframePos = { 0, 0, 0 };                                                           // posicion en el origen
	kf->setTranslate(keyframePos);                                                       // establecemos el keyFrame en la posicion dada

	kf = track->createNodeKeyFrame(longitudPaso * 1);                                    // Keyframe 1: arriba
	keyframePos += Ogre::Vector3::UNIT_Y * tamDesp;                                      // actualizamos la posicion dependiendo del desplazamiento (subiendo)
	kf->setTranslate(keyframePos);                                                       // establecemos el keyFrame en la posicion dada

	kf = track->createNodeKeyFrame(longitudPaso * 2);                                    // Keyframe 2: origen
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Y * tamDesp;                             // actualizamos la posicion dependiendo del desplazamiento (bajando)
	kf->setTranslate(keyframePos);

	kf = track->createNodeKeyFrame(longitudPaso * 3);                                    // Keyframe 3: abajo
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Y * tamDesp;
	kf->setTranslate(keyframePos);

	kf = track->createNodeKeyFrame(longitudPaso * 4);                                    // Keyframe 4: origen
	keyframePos += Ogre::Vector3::UNIT_Y * tamDesp;
	kf->setTranslate(keyframePos);
}