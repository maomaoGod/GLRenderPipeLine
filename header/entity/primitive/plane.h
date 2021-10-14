//
// Created by 钟瑞 on 2021/6/27.
//

#ifndef LEARNOPENGL_PLANE_H
#define LEARNOPENGL_PLANE_H
using namespace std;
#include <vector>
#include <mesh/mesh.h>
#include <entity/entitybase.h>
#include "MeshManager.h"

class Plane:public EntityBase{
public:
    Plane(){
        m_EntityType = E_Entity_Plane;

        Mesh* pmesh = MeshManager::Instance()->GetMeshByName("plane");

        meshes.push_back(*pmesh);
    }

    void DrawEntity() override{
        for(int i=0;i<meshes.size();i++){
            meshes[i].DrawMesh();
        }
    }
};

#endif //LEARNOPENGL_PLANE_H
