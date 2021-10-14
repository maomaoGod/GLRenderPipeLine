//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_PLANEMESH_H
#define LEARNOPENGL_PLANEMESH_H

#include "mesh.h"

class PlaneMesh: public Mesh{
protected:
    float planeVertices[32];
    size_t m_DataLength;
    void InitPlaneVerts(){
        //默认是 x,z平面 ，其他方向的旋转得到
        float tmpverts []= {
                // positions          // texture Coords
//                5.0f, -0.5f,  5.0f,  2.0f, 0.0f,    //0
//                -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,   //1
//                -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,   //2
//                5.0f, -0.5f, -5.0f,  2.0f, 2.0f     //3

                0.5f, 0.0f,  0.5f, 0.0f,1.0f,0.0f,  1.0f, 0.0f,    //0
                -0.5f, 0.0f,  0.5f, 0.0f,1.0f,0.0f,  0.0f, 0.0f,   //1
                -0.5f, 0.0f, -0.5f, 0.0f,1.0f,0.0f,  0.0f, 1.0f,   //2
                0.5f, 0.0f, -0.5f,  0.0f,1.0f,0.0f, 1.0f, 1.0f     //3

//                0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//                0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
//                1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
//
//                0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//                1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
//                1.0f,  0.5f,  0.0f,  1.0f,  0.0f
        };

        m_DataLength = sizeof(tmpverts);
        memcpy(planeVertices,tmpverts,m_DataLength);
    }

    void InitVertData() override{

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, m_DataLength, nullptr, GL_DYNAMIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, m_DataLength, cubeverts, GL_STATIC_DRAW);
        void *ptr =glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        if(nullptr != ptr){
            memcpy((char *)ptr,planeVertices,m_DataLength);
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void InitIndexData() override {
        Mesh::InitIndexData();
    }

    void InitAttributeData() override{
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }

public:
    PlaneMesh(){
        InitPlaneVerts();
        vector<Vertex> verts;
        for(int i=0;i<4;i++){
            Vertex vertit;
            vertit.Position = glm::vec3 (planeVertices[8*i],planeVertices[8*i+1],planeVertices[8*i+2]);
            vertit.Normal = glm::vec3(planeVertices[8*i+3],planeVertices[8*i+4],planeVertices[8*i+5]);
            vertit.TexCoords = glm::vec3(planeVertices[8*i+6],planeVertices[8*i+7],0.0f);
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

#endif //LEARNOPENGL_PLANEMESH_H
