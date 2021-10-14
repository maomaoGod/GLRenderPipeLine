//
// Created by 钟瑞 on 2021/10/13.
//

#ifndef LEARNOPENGL_LIGHT_H
#define LEARNOPENGL_LIGHT_H
#include "entitybase.h"

enum LightType{
    E_Light_Direction,      //平行光
    E_Light_Spot,           //探照灯
    E_Light_Point           //点光源
};

class Light:public EntityBase{
private:
    glm::vec3 m_LightDir;
    glm::mat4 m_LightProj;
    glm::mat4 m_LightView;
    LightType m_LightType;
    float near_plane, far_plane;

private:
    void InitLightInfo(){
        InitLightProj();
        InitLightView();
    }

    void InitLightProj(){
        switch (m_LightType) {
            case E_Light_Direction:
                //平行光使用正交矩阵
                m_LightProj = glm::ortho(-10.0f,
                                         10.0f,
                                         -10.0f,
                                         10.0f,
                                         near_plane,
                                         far_plane);
                break;
        }
    }

    void InitLightView(){

        auto pos = *m_transform.GetPosition();
        glm::vec3 front = glm::vec3(pos -glm::vec3(0.0,0.0,0.0));
        auto Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        auto Right = glm::normalize(glm::cross(Front, glm::vec3(0.0,1.0,0.0)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        auto Up    = glm::normalize(glm::cross(Right, Front));

        m_LightView = glm::lookAt(pos,
                                  glm::vec3( 0.0f, 0.0f,  0.0f),
                                  Up);
    }

public:
    Light(){
        Mesh* cbmesh = MeshManager::Instance()->GetMeshByName("cube");
        if(nullptr == cbmesh){
            //error
        }
        meshes.push_back(*cbmesh);

        near_plane = 1.0f, far_plane = 7.5f;
        m_LightType = E_Light_Direction;
        m_EntityType = E_Entity_Light;
        m_LightDir = glm::vec3 (0.0f);
        InitLightInfo();
    }

    void DrawEntity() override{
        for(int i=0;i<meshes.size();i++){
            meshes[i].DrawMesh();
        }
    }

    void SetPosition(glm::vec3 pos) override{
        EntityBase::SetPosition(pos);
        InitLightInfo();
    }

    glm::mat4 GetLightProj(){
        return m_LightProj;
    }

    glm::mat4 GetLightView(){
        return m_LightView;
    }
};

#endif //LEARNOPENGL_LIGHT_H
