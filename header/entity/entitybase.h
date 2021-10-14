//
// Created by 钟瑞 on 2021/6/27.
//
#ifndef LEARNOPENGL_ENTITYBASE_H
#define LEARNOPENGL_ENTITYBASE_H
using namespace std;
#include<string>
#include<vector>
#include<mesh/mesh.h>
#include<material/material.h>
#include<entity/transform.h>

///构建物体的步骤：
///创建一个物体对象
///设置材质属性
enum EntityType{
    E_Entity_None,
    E_Entity_Light,
    E_Entity_Cube,
    E_Entity_Plane,
    E_Entity_Quad,
    E_Entity_Skybox,
    E_Entity_Sphere,
    E_Entity_Cone,
    E_Entity_Model,
};

class EntityBase{
protected:
    int instid;
    string name;
    vector<Mesh> meshes;
    Material m_material;
    Transform m_transform;
    EntityType m_EntityType;


public:
    EntityBase(){

    }

    virtual void SetMesh(Mesh mesh){
        meshes.push_back(mesh);
    }

    virtual EntityType GetEntityType() const{
        return m_EntityType;
    }

    virtual void SetMaterial(Material mat){
        m_material = mat;
    }

    virtual Material& GetMaterial(){
        return m_material;
    }

    virtual Transform GetTransform(){
        return m_transform;
    }

    virtual void SetPosition(glm::vec3 pos){
        m_transform.SetPosition(pos);
    }

    virtual void BindTextures(){
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        auto textures = m_material.GetMatTexture();
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].TextureType();
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if(name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if(name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(m_material.GetMatShader().ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].TextureID());
        }

//        auto shadowid = glGetUniformLocation(m_material.GetMatShader().ID,"shadowMap");
//        if(shadowid>=0){
//            glActiveTexture(shadowid);
//            glBindTexture(GL_TEXTURE_2D,m_material.GetShadowTexture());
//        }
    }

    virtual void DrawEntity(){
        BindTextures();

        vector<Mesh>::iterator it = meshes.begin();
        for(;it != meshes.end();++it){
            it->DrawMesh();
        }

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

};

class SceneEntity{
private:
    list<EntityBase*> m_AlphaObjs;
    list<EntityBase*> m_UnAlphaObjs;
    EntityBase* m_SkyBox;
    list<EntityBase*> m_Lights;
public:
    SceneEntity(){
        m_SkyBox = nullptr;

        m_AlphaObjs.clear();
        m_UnAlphaObjs.clear();
        m_Lights.clear();
    }

    ~SceneEntity(){
        m_SkyBox = nullptr;
    }

    void AddSceneObject(EntityBase* entity){
        if(nullptr == entity){
            return;
        }

        if(entity->GetEntityType() == EntityType::E_Entity_Light){
            AddLightObj(entity);
        }

        if(entity->GetEntityType() == EntityType::E_Entity_Skybox){
            m_SkyBox = entity;
        }else{
            auto mat = entity->GetMaterial();
            if(mat.GetAlpha()){
                AddAlaphaObj(entity);
            }else{
                AddUnAlaphaObj(entity);
            }
        }
    }

    void RemoveSceneObject(EntityBase *entity){

    }

    list<EntityBase*> GetAlphaObj(){
        return m_AlphaObjs;
    }

    list<EntityBase*> GetUnAlphaObj(){
        return m_UnAlphaObjs;
    }

    list<EntityBase*> GetLightObj(){
        return m_Lights;
    }

    EntityBase* GetSkyBox(){
        return m_SkyBox;
    }

private:
    void AddLightObj(EntityBase* light){
        m_Lights.push_back(light);
    }

    void AddAlaphaObj(EntityBase* entity){
        m_AlphaObjs.push_back(entity);
    }

    void AddUnAlaphaObj(EntityBase* entity){
        m_UnAlphaObjs.push_back(entity);
    }
};
#endif //LEARNOPENGL_ENTITYBASE_H
