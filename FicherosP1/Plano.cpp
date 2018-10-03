#include "Plano.h"

Plano::Plano(Ogre::SceneNode* sceneNode, string name, Camera* camRef, Vector3 u, Ogre::Real f, int width, int height, int numWsegments,
	int numHsegments, bool normals, int numTexCoords, float numUtile, float numVtile, Vector3 v) {

	sceneNode_ = sceneNode;

	MeshManager::getSingleton().createPlane(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, //el gestor de mallas tiene una unica instancia en toda la app (singleton)
		Plane(u, f), width, height, numWsegments, numHsegments, normals, numTexCoords, numUtile, numVtile, v);    //De el utilizamos el metodo para crear una malla de un plano con nombre "Plano"

	plano = sceneNode_->getCreator()->createEntity(name);
	sceneNode_->attachObject(plano);//creamos una entidad con la malla de nombre "Plano" antes creada

	mp = new MovablePlane(u, f);
	sceneNode_->attachObject(mp);

	camRef->enableReflection(mp);
	camRef->enableCustomNearClipPlane(mp);

	TexturePtr rttTex = TextureManager::getSingleton().createManual("texRtt", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
		width, height, 0, PF_R8G8B8, TU_RENDERTARGET);

	RenderTexture* renderTexture = rttTex->getBuffer()->getRenderTarget();	Viewport * vpt = renderTexture->addViewport(camRef);
	vpt->setClearEveryFrame(true);
	vpt->setBackgroundColour(ColourValue::Black);

	TextureUnitState* tu = plano->getSubEntities()[0]->getMaterial()->getTechniques()[0]->getPasses()[0]->createTextureUnitState("texRtt");
	tu->setColourOperation(LBO_MODULATE); // backgroundColour -> black
	// LBO_ADD / LBO_ALPHA_BLEND / LBO_REPLACE
	tu->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	tu->setProjectiveTexturing(true, camRef);
}