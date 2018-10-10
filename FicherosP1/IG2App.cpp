#include "IG2App.h"

#include <OgreEntity.h>
#include <OgreInput.h>
#include <SDL_keycode.h>
#include <OgreMeshManager.h>

using namespace Ogre;

bool IG2App::keyPressed(const OgreBites::KeyboardEvent& evt)
{
  if (evt.keysym.sym == SDLK_ESCAPE)
  {
    getRoot()->queueEndRendering();
  }
  else if (evt.keysym.sym == SDLK_p)   //si se pulsa p el plano gira en el eje x
  {
	  giroPlano();
  }
  
  return true;
}

void IG2App::giroPlano() {
	mPlano->pitch(Ogre::Radian(0.3));
}

void IG2App::shutdown()
{
  mShaderGenerator->removeSceneManager(mSM);  
  mSM->removeRenderQueueListener(mOverlaySystem);  
					
  mRoot->destroySceneManager(mSM);  

  delete mTrayMgr;  mTrayMgr = nullptr;
  delete mCamMgr; mCamMgr = nullptr;
  
  // do not forget to call the base 
  IG2ApplicationContext::shutdown();
}

void IG2App::setup(void)
{
  // do not forget to call the base first
  IG2ApplicationContext::setup();

  mSM = mRoot->createSceneManager();  

  // register our scene with the RTSS
  mShaderGenerator->addSceneManager(mSM);

  mSM->addRenderQueueListener(mOverlaySystem);

  mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);  
  mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
  addInputListener(mTrayMgr);

  addInputListener(this);   
  setupScene();
}

void IG2App::setupScene(void)
{
	//----------------------------------CAMARA--------------------------------
  Camera* cam = mSM->createCamera("Cam");
  cam->setNearClipDistance(1); 
  cam->setFarClipDistance(10000);
  cam->setAutoAspectRatio(true);
  //cam->setPolygonMode(Ogre::PM_WIREFRAME); 

  mCamNode = mSM->getRootSceneNode()->createChildSceneNode("nCam");
  mCamNode->attachObject(cam);

  mCamNode->setPosition(0, 0, 1000);
  mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
  //mCamNode->setDirection(Ogre::Vector3(0, 0, -1));  

  // and tell it to render into the main window
  Viewport* vp = getRenderWindow()->addViewport(cam);
  //vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
  
  //----------------------------------CAMARAREF--------------------------------

  Camera* camRef = mSM->createCamera("CamRef");     // camara para hacer el reflejo
  camRef->setNearClipDistance(1);
  camRef->setFarClipDistance(10000);
  camRef->setAutoAspectRatio(true);
  //cam->setPolygonMode(Ogre::PM_WIREFRAME); 

  mCamNode->attachObject(camRef);

  //-----------------------------------LUCES---------------------------------

  // without light we would just get a black screen 

  Light* luz = mSM->createLight("Luz");
  luz->setType(Ogre::Light::LT_DIRECTIONAL);
  luz->setDiffuseColour(0.75, 0.75, 0.75);

  //mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");      //asi la luz seria hija del nodo raiz y no se moveria
  mLightNode = mCamNode->createChildSceneNode("nLuz");                       //de esta forma es hija de la camara y se mueve con ella
  mLightNode->attachObject(luz);

  mLightNode->setDirection(Ogre::Vector3(0, 0, -1));  //vec3.normalise();
  //lightNode->setPosition(0, 0, 1000);
 
  //---------------------------------PLANO----------------------------------

  mPlano = mSM->getRootSceneNode()->createChildSceneNode("nPlano"); // hacemos que el nodo mPlano sea hijo del nodo raiz
  plano = new Plano(mPlano, "Plano", camRef);                       // creamos el plano, que por debajo se adjuntara al nodo mPlano

  /*Definir en un archivo de texto ("IG2App.material") un material de nombre "nombre" con
	una unidad de textura para la imagen 1d_debug.png (mejor cambiar el nombre) y coeficientes de
	reflexión (0.5, 0.5, 0.5). El archivo debe estar en media\IG2App, junto con los archivos de las imágenes que utilice 
	(copiarlas de media\materials\textures). Añadir scroll_anim 0.1 0.0 para que se mueva horizontalmente*/

  //---------------------------------TOY------------------------------------

  mToy = mPlano->createChildSceneNode("nToy");                      // hacemos que el nodo mToy sea hijo del nodo mPlano
  toy = new Toy(mToy, "sphere.mesh");                               // inicializamos la entidad toy, a la que apuntara mToy
  addInputListener(toy);                                            // añadimos toy a la lista de oyentes para poder recibir eventos de teclado

  //-------------------------------SINBAD-----------------------------------

  mSinbadNode = mPlano->createChildSceneNode("nSinbad");            // hacemos que el nodo mSinbadNode sea hijo del nodo mPlano
  sinbad = new Sinbad(mSinbadNode, "Sinbad.mesh");                  // creamos a Sinbad, que por debajo se adjuntara al nodo mSinbadNode
  addInputListener(sinbad);

  sinbad->setPosition(400, 100, -300);                              // cambiamos posicion y escala del nodo mSinbadNode respecto a su padre,
  sinbad->setScale(20, 20, 20);                                     // el nodo mPlano
  sinbad->setAnimation("Dance");
  //mSinbadNode->yaw(Ogre::Degree(-45));
  //mSinbadNode->showBoundingBox(true);
  //mSinbadNode->setVisible(false);

  //------------------------------------------------------------------------

  mCamMgr = new OgreBites::CameraMan(mCamNode);
  addInputListener(mCamMgr);
  mCamMgr->setStyle(OgreBites::CS_ORBIT);  
  
  //mCamMgr->setTarget(mSinbadNode);  
  //mCamMgr->setYawPitchDist(Radian(0), Degree(30), 100);

  //------------------------------------------------------------------------

}

