//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_CUBEMESH_H
#define LEARNOPENGL_CUBEMESH_H
#include "TSingleton.h"
#include "mesh.h"

class CubeMesh: public Mesh{
protected:

    size_t m_DataLength;
    float* cubeverts;
    float cubvertsarray[288];
    void InitCubverts(){
         float tmpverts [] = {
                -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f, 0.0f, 0.0f,//0
                0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f, 1.0f, 0.0f,//1
                0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f, 1.0f, 1.0f,//2
                0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f, 1.0f, 1.0f,//2
                -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f, 0.0f, 1.0f,//3
                -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f, 0.0f, 0.0f,//0

                -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,  0.0f, 0.0f,//4
                0.5f, -0.5f,  0.5f,      0.0f,  0.0f, 1.0f,  1.0f, 0.0f,//5
                0.5f,  0.5f,  0.5f,      0.0f,  0.0f, 1.0f,  1.0f, 1.0f,//6
                0.5f,  0.5f,  0.5f,      0.0f,  0.0f, 1.0f,  1.0f, 1.0f,//6
                -0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,  0.0f, 1.0f,//7
                -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,  0.0f, 0.0f,//4

                -0.5f,  0.5f,  0.5f,     -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,//7
                -0.5f,  0.5f, -0.5f,     -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,//3
                -0.5f, -0.5f, -0.5f,     -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,//0
                -0.5f, -0.5f, -0.5f,     -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,//0
                -0.5f, -0.5f,  0.5f,     -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,//4
                -0.5f,  0.5f,  0.5f,     -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,//7

                0.5f,  0.5f,  0.5f,      1.0f,  0.0f,  0.0f, 1.0f, 0.0f,//6
                0.5f,  0.5f, -0.5f,      1.0f,  0.0f,  0.0f, 1.0f, 1.0f,//2
                0.5f, -0.5f, -0.5f,      1.0f,  0.0f,  0.0f, 0.0f, 1.0f,//1
                0.5f, -0.5f, -0.5f,      1.0f,  0.0f,  0.0f, 0.0f, 1.0f,//1
                0.5f, -0.5f,  0.5f,      1.0f,  0.0f,  0.0f, 0.0f, 0.0f,//5
                0.5f,  0.5f,  0.5f,      1.0f,  0.0f,  0.0f, 1.0f, 0.0f,//6

                -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f, 0.0f, 1.0f,//0
                0.5f, -0.5f, -0.5f,      0.0f, -1.0f,  0.0f, 1.0f, 1.0f,//1
                0.5f, -0.5f,  0.5f,      0.0f, -1.0f,  0.0f, 1.0f, 0.0f,//5
                0.5f, -0.5f,  0.5f,      0.0f, -1.0f,  0.0f, 1.0f, 0.0f,//5
                -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f, 0.0f, 0.0f,//4
                -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f, 0.0f, 1.0f,//0

                -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f, 0.0f, 1.0f,//3
                0.5f,  0.5f, -0.5f,      0.0f,  1.0f,  0.0f, 1.0f, 1.0f,//2
                0.5f,  0.5f,  0.5f,      0.0f,  1.0f,  0.0f, 1.0f, 0.0f,//6
                0.5f,  0.5f,  0.5f,      0.0f,  1.0f,  0.0f, 1.0f, 0.0f,//6
                -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f, 0.0f, 0.0f,//7
                -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f, 0.0f, 1.0f//3
        };

         cubeverts = tmpverts;
         m_DataLength = sizeof(tmpverts);
        for(int i=0;i<288;i++){
            cubvertsarray[i] = tmpverts[i];
        }
    }

    void InitVertData() override{

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, m_DataLength, nullptr, GL_DYNAMIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, m_DataLength, cubeverts, GL_STATIC_DRAW);
        void *ptr =glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
        if(nullptr != ptr){
            memcpy((char *)ptr,cubvertsarray,m_DataLength);
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

    CubeMesh(){
        cubeverts = nullptr;
        InitCubverts();
        vector<Vertex> verts;
        vector<unsigned int> indices;
        for(int i=0;i<36;i++){
            Vertex vertit;
            vertit.Position = glm::vec3 (cubeverts[8*i],cubeverts[8*i+1],cubeverts[8*i+2]);
            vertit.Normal = glm::vec3(cubeverts[8*i+3],cubeverts[8*i+4],cubeverts[8*i+5]);
            vertit.TexCoords = glm::vec3(cubeverts[8*i+6],cubeverts[8*i+7],0.0f);
            verts.push_back(vertit);
            indices.push_back(i);
        }

        this->vertices = verts;
        this->indices = indices;
        SetupMesh();
    }
};

#endif //LEARNOPENGL_CUBEMESH_H
