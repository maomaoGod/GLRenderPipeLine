//
// Created by 钟瑞 on 2021/10/11.
//

#ifndef LEARNOPENGL_TRANSFORM_H
#define LEARNOPENGL_TRANSFORM_H

#include <glm/glm.hpp>

class Transform{
private:
    glm::vec3 *m_Position;
    glm::vec3 *m_Scale;
    glm::vec3 *m_Rotation;

    glm::mat4 m_ModelMat;
public:
    Transform(){
        m_Position = new glm::vec3(0.0f);
        m_Scale = new glm::vec3 (1.0f);
        m_Rotation = new glm::vec3 (0.0f);
    }

    glm::vec3* GetPosition() const{
        return m_Position;
    }

    void SetPosition(glm::vec3 pos) {
        m_Position->x = pos.x;
        m_Position->y = pos.y;
        m_Position->z = pos.z;
    }

    glm::vec3* GetScale() const{
        return m_Scale;
    }

    void SetScale(glm::vec3 scale){
        m_Scale->x = scale.x;
        m_Scale->y = scale.y;
        m_Scale->z = scale.z;
    }

    glm::vec3* GetRotation() const{
        return m_Rotation;
    }

    void SetRotation(glm::vec3 rot){
        m_Rotation->x = rot.x;
        m_Rotation->y = rot.y;
        m_Rotation->z = rot.z;
    }

    glm::mat4 GetModelMat() {
        m_ModelMat = glm::mat4(1.0f);
        m_ModelMat = glm::translate(m_ModelMat, *m_Position);
        m_ModelMat = glm::scale(m_ModelMat,*m_Scale);
        m_ModelMat = glm::rotate(m_ModelMat,m_Rotation->x,glm::vec3(1.0f,0.0f,0.0f));
        m_ModelMat = glm::rotate(m_ModelMat,m_Rotation->y,glm::vec3(0.0f,1.0f,0.0f));
        m_ModelMat = glm::rotate(m_ModelMat,m_Rotation->z,glm::vec3(0.0f,0.0f,1.0f));

        return m_ModelMat;
    }
};

#endif //LEARNOPENGL_TRANSFORM_H
