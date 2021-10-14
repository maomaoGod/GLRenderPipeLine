//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_SCENE_H
#define LEARNOPENGL_SCENE_H

#include <vector>
#include <list>
#include "Rendering/IRenderSystem.h"
#include "entity/entitybase.h"
#include "SceneManager.h"
#include "camera/camera.h"
using namespace std;

class Scene{

private:
    list<Camera*> m_SceneCameras;

public:
    Scene(){
        SceneManager::Instance()->Initialize();
    }

    void AddSceneObj(EntityBase* entity){
        SceneManager::Instance()->AddSceneObj(entity);
    }

    void RemoveSceneObj(EntityBase* entity){
        SceneManager::Instance()->RemoveSceneObj(entity);
    }

    void AddCamera(Camera * cam){
        if(nullptr != cam){
            m_SceneCameras.push_back(cam);
        }
    }

    void RemoveCamera(Camera* cam){
        if(nullptr != cam){
            m_SceneCameras.remove(cam);
        }
    }

    void DoRender(){

        std::list<Camera*>::iterator cmit = m_SceneCameras.begin();
        for(;cmit != m_SceneCameras.end();++cmit){
            Camera* cm = *cmit;
            if(nullptr != cm){
                cm->DoRender();
            }
        }
    }

    void ProcessKeyInput(Camera_Movement direction, float deltaTime){
        std::list<Camera*>::iterator cmit = m_SceneCameras.begin();
        for(;cmit != m_SceneCameras.end();++cmit){
            Camera* cm = *cmit;
            if(nullptr != cm){
                cm->ProcessKeyboard(direction,deltaTime);
            }
        }
    }

    void ProcessMouseScroll(float x,float y){
        std::list<Camera*>::iterator cmit = m_SceneCameras.begin();
        for(;cmit != m_SceneCameras.end();++cmit){
            Camera* cm = *cmit;
            if(nullptr != cm){
                cm->ProcessMouseScroll(y);
            }
        }
    }

    void ProcessMouseMove(float x,float y){
        std::list<Camera*>::iterator cmit = m_SceneCameras.begin();
        for(;cmit != m_SceneCameras.end();++cmit){
            Camera* cm = *cmit;
            if(nullptr != cm){
                cm->ProcessMouseMovement(x, y);
            }
        }
    }

    //frame buffer 的尺寸是window的尺寸的2倍，因此在framebuffer 改变的时候调整view port
    void ProcessWindowSizeChange(float width,float height){
        std::list<Camera*>::iterator cmit = m_SceneCameras.begin();
        for(;cmit != m_SceneCameras.end();++cmit){
            Camera* cm = *cmit;
            if(nullptr != cm){
                cm->ProcessWindowSizeChange(width,height);
            }
        }
    }
};

#endif //LEARNOPENGL_SCENE_H
