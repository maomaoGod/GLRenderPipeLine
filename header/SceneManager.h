//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_SCENEMANAGER_H
#define LEARNOPENGL_SCENEMANAGER_H

#include "TSingleton.h"
#include <vector>
#include <list>
using namespace std;

class SceneManager:public TSingleton<SceneManager>{
    friend class TSingleton<SceneManager>;
private:
    SceneEntity m_SceneObjects;

private:
    SceneManager(){

    }

public:

    void AddSceneObj(EntityBase* entity){
        m_SceneObjects.AddSceneObject(entity);
    }

    void RemoveSceneObj(EntityBase* entity){
        m_SceneObjects.RemoveSceneObject(entity);
    }

    void Initialize() override{
    }

    void UnInitialize() override {

    }

    SceneEntity& GetSceneObjects(){
        return m_SceneObjects;
    }
};
#endif //LEARNOPENGL_SCENEMANAGER_H
