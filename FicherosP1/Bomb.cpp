#include "Bomb.h"

Bomb::Bomb(SceneNode* sceneNode, std::string mesh, float duracion, float tamDesp, std::string name) : GameObject(sceneNode, mesh) {
	sceneNode_->setScale(0.3, 0.3, 0.3);

	ent->setMaterialName(name);

	Animation* animation = sceneNode_->getCreator()->createAnimation("animVV", duracion);
	NodeAnimationTrack* track = animation->createNodeTrack(0);
	track->setAssociatedNode(sceneNode_);
	Vector3 keyframePos = { 0, 0, 0 };
	Real longitudPaso = duracion / 4.0; // uniformes
	TransformKeyFrame * kf; // 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)

	kf = track->createNodeKeyFrame(longitudPaso * 0); // Keyframe 0: origen
	kf->setTranslate(keyframePos); // Origen: Vector3


	kf = track->createNodeKeyFrame(longitudPaso * 1); // Keyframe 1: arriba
	keyframePos += Ogre::Vector3::UNIT_Y * tamDesp;
	kf->setTranslate(keyframePos); // Arriba
	

	kf = track->createNodeKeyFrame(longitudPaso * 2); // Keyframe 2: origen
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Y * tamDesp;
	kf->setTranslate(keyframePos); // Origen

	kf = track->createNodeKeyFrame(longitudPaso * 3); // Keyframe 3: abajo
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Y * tamDesp;
	kf->setTranslate(keyframePos); // Abajo

	kf = track->createNodeKeyFrame(longitudPaso * 4); // Keyframe 4: origen
	keyframePos += Ogre::Vector3::UNIT_Y * tamDesp;
	kf->setTranslate(keyframePos); // Origen

	sceneNode_->setInitialState();

	animState = sceneNode_->getCreator()->createAnimationState("animVV");
	animState->setEnabled(true);
	animState->setLoop(true);
}