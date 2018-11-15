#include "Plano.h"

Plano::Plano(Ogre::SceneNode* sceneNode, string name, string matName, Vector3 u, Ogre::Real f, int width, int height, int numWsegments,
	int numHsegments, bool normals, int numTexCoords, float numUtile, float numVtile, Vector3 v) {

	sceneNode_ = sceneNode;                                // nodo que apuntara a la entidad plano
	name_ = name;

	// creamos malla
	MeshManager::getSingleton().createPlane(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,           // el gestor de mallas tiene una unica instancia en toda la app (singleton)
		Plane(u, f), width, height, numWsegments, numHsegments, normals, numTexCoords, numUtile, numVtile, v); // De el utilizamos el metodo para crear una malla de un plano con nombre "Plano"

	plano = sceneNode_->getCreator()->createEntity(name);  // mandamos al gestor de la escena crear una entidad plano con esa malla
	sceneNode_->attachObject(plano);                       // la adjuntamos al nodo plano
	setMaterial(matName);                                  // y le agregamos el material

  /*Definir en un archivo de texto ("IG2App.material") un material de nombre "nombre" con
	una unidad de textura para la imagen 1d_debug.png (mejor cambiar el nombre) y coeficientes de
	reflexión (0.5, 0.5, 0.5). El archivo debe estar en media\IG2App, junto con los archivos de las imágenes que utilice
	(copiarlas de media\materials\textures). Añadir scroll_anim 0.1 0.0 para que se mueva horizontalmente*/
}

void Plano::setReflex(Camera* camRef, Vector3 u, Ogre::Real f, int width, int height) {
	mp = new MovablePlane(u, f);                           // creamos el panel sobre el que se quiere realizar el reflejo
	sceneNode_->attachObject(mp);                          // lo adjuntamos al nodo

	camRef->enableReflection(mp);                          // configuramos el reflejo sobre el plano
	camRef->enableCustomNearClipPlane(mp);

	// añadimos una textura, en el mismo grupo de recursos que la malla del panel, para usarla de RenderTarget y de textura del panel
	TexturePtr rttTex = TextureManager::getSingleton().createManual("texRtt", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
		width, height, 0, PF_R8G8B8, TU_RENDERTARGET);

	// añadimos un puerto de vista al RenderTarget con la nueva cámara
	RenderTexture* renderTexture = rttTex->getBuffer()->getRenderTarget();
	Viewport * vpt = renderTexture->addViewport(camRef);
	vpt->setClearEveryFrame(true);
	vpt->setBackgroundColour(ColourValue::White);           // color de fondo blanco (asi el reflejo se vera oscuro)

	// añadimos la nueva unidad de textura al material del panel
	TextureUnitState* tu = plano->getSubEntities()[0]->getMaterial()->getTechniques()[0]->getPasses()[0]->createTextureUnitState("texRtt");
	tu->setColourOperation(LBO_MODULATE);                   // multiplicar->el reflejo tendera a ser mas oscuro
	// LBO_ADD / LBO_ALPHA_BLEND / LBO_REPLACE              // añadir->el reflejo tendera a ser mas claro
	tu->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	tu->setProjectiveTexturing(true, camRef);

	renderTexture->addListener(this);
}

// nos avisa antes del renderizado
void Plano::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) {
	plano->setVisible(false);                               // oculta el panel (la entidad) y pone luz ambiente
	sceneNode_->getCreator()->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
}

// nos avisa despues del renderizado
void Plano::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) {
	plano->setVisible(true);                                // restablece los cambios
	sceneNode_->getCreator()->setAmbientLight(ColourValue(0, 0, 0));
}

Plano::~Plano() {
	delete mp;
	mp = nullptr;
}