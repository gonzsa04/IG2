#ifndef __IG2App_H__
#define __IG2App_H__

#include "IG2ApplicationContext.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include "Toy.h"
#include "Sinbad.h"
#include "Plano.h"
#include "Bomb.h"

using namespace std;


class IG2App : public  OgreBites::IG2ApplicationContext, OgreBites::InputListener 
{
public:
  explicit IG2App() : IG2ApplicationContext("IG2App") { };  // new -> setup()  
  virtual ~IG2App() { };   // delete -> shutdown()  
 
protected:
  virtual void setup();
  virtual void shutdown();
  virtual void setupScene();
  void giroPlano();

  virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);  // InputListener

  virtual void frameRendered(const Ogre::FrameEvent & evt);
      
  Ogre::SceneManager* mSM = nullptr;     //nodo del gestor de la escena, sera hijo del nodo raiz
  OgreBites::TrayManager* mTrayMgr = nullptr;

  // nodos de los elementos de la escena
  Ogre::SceneNode* mLightNode = nullptr;
  Ogre::SceneNode* mCamNode = nullptr;
  Ogre::SceneNode* mPlano = nullptr;
  Ogre::SceneNode* mToy = nullptr;
  Ogre::SceneNode* mBomba = nullptr;
  Ogre::SceneNode* mSinbadNode = nullptr;

  OgreBites::CameraMan* mCamMgr = nullptr;
  Toy* toy = nullptr;
  Sinbad* sinbad = nullptr;
  Plano* plano = nullptr;
  Bomb* bomba = nullptr;

  bool cameraSigue = false;
};

#endif
