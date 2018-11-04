#pragma once
#include "GameObject.h"
#include <OgreCameraMan.h>
#include <OgreMovablePlane.h>
#include <OgreTextureManager.h>
#include <OgreRenderTexture.h>
#include <OgreViewport.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreSubEntity.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreRenderTargetListener.h>

using namespace std;
using namespace Ogre;

class Plano : public GameObject, RenderTargetListener
{
private:
	Ogre::Entity* plano = nullptr;        // entidad plano a la que le añadimos textura y material
	MovablePlane* mp = nullptr;           // descripcion geometrica/matematica del plano
	string name_;                         // nombre del plano
public:
	Plano(Ogre::SceneNode* sceneNode, string name, Camera* camRef, Vector3 u = Vector3::UNIT_Y, Ogre::Real f = 0, int width = 1080, int height = 800, int numWsegments = 100,
		int numHsegments = 80, bool normals = true, int numTexCoords = 1, float numUtile = 1.0, float numVtile = 1.0, Vector3 v = -Vector3::UNIT_Z, string matName = "Plano");

	void setMaterial(string name) { plano->setMaterialName(name); }

	virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

	virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

	virtual void receive(TipoEvent evt, GameObject* go) {}

	virtual ~Plano();
};
