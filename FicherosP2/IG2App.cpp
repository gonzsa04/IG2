#include "IG2App.h"

#include <OgreEntity.h>
#include <OgreInput.h>
#include <SDL_keycode.h>
#include <OgreMeshManager.h>

#include <OgreCompositorManager.h>

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
  else if (evt.keysym.sym == SDLK_c) { // si se pulsa c la camara establece como objetivo a sinbad o a la raiz de la escena
	  if(!cameraSigue) mCamMgr->setTarget(mSinbadNode);
	  else mCamMgr->setTarget(mSM->getRootSceneNode());
	  cameraSigue = !cameraSigue;
  }
  else if (evt.keysym.sym == SDLK_i)   // si se pulsa i, la interferencia comienza a parpadear o se quita si ya estaba puesta
  {
	  interference = !interference;
	  if (!interference) {             // si la interferencia se quita, inicializamos los valores 
		  CompositorManager::getSingleton().setCompositorEnabled(vp, "Interference", false);
		  interferenceEnable = false;
		  timeSinceLastFrame = 0;
	  }
	  else {                           // si no, la ponemos
		  CompositorManager::getSingleton().setCompositorEnabled(vp, "Interference", true);
	  }
  }
  
  return true;
}

void IG2App::frameRendered(const Ogre::FrameEvent & evt) {
	if (toy->getActive() && bomba->getActive()) {  // detectamos colision entre toy y la bomba (si estos no han colisionado ya)
		Sphere bombaCollision = bomba->getEntity()->getWorldBoundingSphere();
		Sphere toyCollision = toy->getEntity()->getWorldBoundingSphere();
		if (bombaCollision.intersects(toyCollision)) GameObject::fireAppEvent(COLISION, toy); // en tal caso lanzamos un evento
	}

	if (interference) { // si hay interferencias, se activan y desactivan cada timeToNextInterference segundos
		timeSinceLastFrame += evt.timeSinceLastFrame;
		if (timeSinceLastFrame >= timeToNextInterference) {
			CompositorManager::getSingleton().setCompositorEnabled(vp, "Interference", interferenceEnable);
			interferenceEnable = !interferenceEnable;
			timeSinceLastFrame = 0;
		}
	}
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
  vp = getRenderWindow()->addViewport(cam);
  //vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
  
  //----------------------------------CAMARAREF--------------------------------

  Camera* camRef = mSM->createCamera("CamRef");     // camara para hacer el reflejo
  camRef->setNearClipDistance(1);                   // mismo frustum y mismo nodo que la camara principal
  camRef->setFarClipDistance(10000);                // asi actualiza su posicion en funcion de como movamos la principal
  camRef->setAutoAspectRatio(true);

  mCamNode->attachObject(camRef);

  mCamMgr = new OgreBites::CameraMan(mCamNode);
  addInputListener(mCamMgr);
  mCamMgr->setStyle(OgreBites::CS_ORBIT);

  //mCamMgr->setTarget(mSinbadNode);  
  //mCamMgr->setYawPitchDist(Radian(0), Degree(30), 100);

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
  plano = new Plano(mPlano, "Plano");                               // creamos el plano, que por debajo se adjuntara al nodo mPlano
  plano->setReflex(camRef);

   //---------------------------------FONDO----------------------------------

  mSM->setSkyPlane(true, Plane(Vector3::UNIT_Z, -20), "mandelbrot1", 1, 1, true, 1.0, 100, 100);
  
  CompositorManager::getSingleton().addCompositor(vp, "Interference"); // a�adimos composicion (efecto de postprocesado)

  //---------------------------------TOY------------------------------------

  mToy = mPlano->createChildSceneNode("nToy");                      // hacemos que el nodo mToy sea hijo del nodo mPlano
  toy = new Toy(mToy, "uv_sphere.mesh", "Holes");                   // inicializamos la entidad toy, a la que apuntara mToy, con uv_sphere para que tenga coordenadas de textura
  addInputListener(toy);                                            // a�adimos toy a la lista de oyentes para poder recibir eventos de teclado

  //-------------------------------SINBAD-----------------------------------

  mSinbadNode = mPlano->createChildSceneNode("nSinbad");            // hacemos que el nodo mSinbadNode sea hijo del nodo mPlano
  sinbad = new Sinbad(mSinbadNode, "Sinbad.mesh");                  // creamos a Sinbad, que por debajo se adjuntara al nodo mSinbadNode
  addInputListener(sinbad);                                         // a�adimos Sinbad a la lista de oyentes para poder recibir eventos de teclado
  //mSinbadNode->yaw(Ogre::Degree(-45));
  //mSinbadNode->showBoundingBox(true);
  //mSinbadNode->setVisible(false);

  //-------------------------------BOMBA-----------------------------------

  mBomba = mPlano->createChildSceneNode("nBomba");                  // lo mismo con la bomba  
  bomba = new Bomb(mBomba);           
  addInputListener(bomba);

  //------------------------------EVENTOS----------------------------------

  GameObject::addAppListener(toy);    // GameObjects de la escena que pueden recibir eventos
  GameObject::addAppListener(sinbad);
  GameObject::addAppListener(bomba);

  //-----------------------------------------------------------------------
}

