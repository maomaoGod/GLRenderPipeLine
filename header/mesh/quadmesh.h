//
// Created by 钟瑞 on 2021/10/11.
//

#ifndef LEARNOPENGL_QUADMESH_H
#define LEARNOPENGL_QUADMESH_H

#include "mesh.h"

//二维的面片
class QuadMesh: public Mesh{
protected:
    float quadVertices[24];
    size_t m_DataLength;
    void InitQuadVerts(){
        float tmpquad[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
                // positions   // texCoords
                -1.0f,  1.0f,  0.0f, 1.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                1.0f, -1.0f,  1.0f, 0.0f,

                -1.0f,  1.0f,  0.0f, 1.0f,
                1.0f, -1.0f,  1.0f, 0.0f,
                1.0f,  1.0f,  1.0f, 1.0f

//                0.2f,  0.3f,  0.0f, 1.0f,
//                0.2f,  -0.3f,  0.0f, 0.0f,
//                1.0f,  -0.3f,  1.0f, 0.0f,
//
//                0.2f,  0.3f,  0.0f, 1.0f,
//                1.0f,  -0.3f,  1.0f, 0.0f,
//                1.0f,  0.3f,  1.0f, 1.0f
//                -0.3f,  1.0f,  0.0f, 1.0f,
//                -0.3f,  0.7f,  0.0f, 0.0f,
//                0.3f,  0.7f,  1.0f, 0.0f,
//
//                -0.3f,  1.0f,  0.0f, 1.0f,
//                0.3f,  0.7f,  1.0f, 0.0f,
//                0.3f,  1.0f,  1.0f, 1.0f
        };

        m_DataLength = sizeof(tmpquad);
        memcpy(quadVertices,tmpquad,m_DataLength);
    }

    void InitVertData() override{

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, m_DataLength, nullptr, GL_DYNAMIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, m_DataLength, cubeverts, GL_STATIC_DRAW);
        void *ptr =glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        if(nullptr != ptr){
            memcpy((char *)ptr,quadVertices,m_DataLength);
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void InitIndexData() override {
        Mesh::InitIndexData();
    }

    void InitAttributeData() override{
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

public:
    QuadMesh(){
        InitQuadVerts();
        vector<Vertex> verts;
        for(int i=0;i<4;i++){
            Vertex vertit;
            vertit.Position = glm::vec3 (quadVertices[4*i],quadVertices[4*i+1],0.0f);
            vertit.TexCoords = glm::vec3(quadVertices[4*i+2],quadVertices[4*i+3],0.0f);
            verts.push_back(vertit);
        }

        vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);

        this->indices = indices;
        this->vertices = verts;

        SetupMesh();
    }
};

#endif //LEARNOPENGL_QUADMESH_H
