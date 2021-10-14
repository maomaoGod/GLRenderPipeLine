//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_MESHMANAGER_H
#define LEARNOPENGL_MESHMANAGER_H
#include "TSingleton.h"
#include "mesh/mesh.h"
#include "mesh/cubemesh.h"
#include "mesh/planemesh.h"
#include "mesh/quadmesh.h"
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

class MeshManager:public TSingleton<MeshManager>{
    friend class TSingleton<MeshManager>;
private:
    map<string,Mesh*> m_NameToMesh;
    set<string> m_PrimitiveMeshs;
protected:
    MeshManager(){
        Initialize();
    }
public:
    void Initialize() override{
        m_PrimitiveMeshs.insert("cube");        //立方体
        m_PrimitiveMeshs.insert("plane");       //平面
        m_PrimitiveMeshs.insert("sphere");      //球体
        m_PrimitiveMeshs.insert("capsule");     //胶囊体
        m_PrimitiveMeshs.insert("cone");        //圆锥体
        m_PrimitiveMeshs.insert("tetrahedron"); //四面体
        m_PrimitiveMeshs.insert("torus");       //圆环
        m_PrimitiveMeshs.insert("quad");        //面片
    }

    Mesh* GetMeshByName(string name){
        Mesh * result = nullptr;

        if(m_NameToMesh.find(name) != m_NameToMesh.end()){
            result = m_NameToMesh[name];
        }else{
            std::set<string>::iterator it = m_PrimitiveMeshs.find(name);

            if(it != m_PrimitiveMeshs.end()){
                if(name.compare("cube")==0){
                    result = new CubeMesh();
                    m_NameToMesh.insert(std::pair<string ,Mesh*>("cube",result));
                }else if(name.compare("plane")==0){
                    result = new PlaneMesh();
                    m_NameToMesh.insert(std::pair<string,Mesh*>(name,result));
                } else if(name.compare("quad") == 0){
                    result = new QuadMesh();
                    m_NameToMesh.insert(std::pair<string,Mesh*>(name,result));
                }

            }else{
                //specila impliment
            }
        }

        return result;
    }
};

#endif //LEARNOPENGL_MESHMANAGER_H
