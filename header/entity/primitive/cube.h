//
// Created by 钟瑞 on 2021/6/27.
//

#ifndef LEARNOPENGL_CUBE_H
#define LEARNOPENGL_CUBE_H
using namespace std;
#include <vector>
#include <mesh/mesh.h>
#include <entity/entitybase.h>
#include "MeshManager.h"
#include <shader.h>

class Cube:public EntityBase{

public:
    Cube(){
        m_EntityType = E_Entity_Cube;

        Mesh* cbmesh = MeshManager::Instance()->GetMeshByName("cube");
        if(nullptr == cbmesh){
            //error
        }

        meshes.push_back(*cbmesh);
    }

    void DrawEntity() override{
        for(int i=0;i<meshes.size();i++){
            meshes[i].DrawMesh();
        }
    }
};
#endif //LEARNOPENGL_CUBE_H
