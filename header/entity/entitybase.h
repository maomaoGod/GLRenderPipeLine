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
    bool m_bCastShadow;

public:
    EntityBase(){
        m_bCastShadow = false;
    }

    virtual void SetCastShadow(bool bCast){
        m_bCastShadow = bCast;
    }

    virtual bool GetCastShadow() const{
        return m_bCastShadow;
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
        m_material.BindTextures();
    }

    virtual void DrawEntity(){
        m_material.BindTextures();

        vector<Mesh>::iterator it = meshes.begin();
        for(;it != meshes.end();++it){
            it->DrawMesh();
        }

        // always good practice to set everything back to defaults once configured.
        m_material.UnBindTextures();
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
