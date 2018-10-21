#include "Bomb.h"

Bomb::Bomb(SceneNode* sceneNode, std::string mesh, float duracion, float tamDesp, std::string materialName) : GameObject(sceneNode, mesh) {
	sceneNode_->setScale(0.3, 0.3, 0.3);  // modificamos escala
	ent->setMaterialName(materialName);   // y añadimos el material deseado

	// ------------------------------------------------------ANIMACIONES--------------------------------------------------------
	// creamos una animacion personalizada para que parezca que flota en el agua
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

	// activamos la animacion que hemos creado y hacemos que se repita en loop
	animState = sceneNode_->getCreator()->createAnimationState("flota");
	animState->setEnabled(true);
	animState->setLoop(true);
}