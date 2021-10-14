//
// Created by 钟瑞 on 2021/10/11.
//

#ifndef LEARNOPENGL_QUAD_H
#define LEARNOPENGL_QUAD_H

using namespace std;
#include <vector>
#include <mesh/mesh.h>
#include <entity/entitybase.h>
#include "MeshManager.h"

class Quad:public EntityBase{
public:
    Quad(){
        m_EntityType = E_Entity_Quad;

        Mesh* pmesh = MeshManager::Instance()->GetMeshByName("quad");

        meshes.push_back(*pmesh);
    }

    void DrawEntity() override{
        for(int i=0;i<meshes.size();i++){
            meshes[i].DrawMesh();
        }
    }
};

#endif //LEARNOPENGL_QUAD_H
