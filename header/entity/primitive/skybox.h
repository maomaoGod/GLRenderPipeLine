//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_SKYBOX_H
#define LEARNOPENGL_SKYBOX_H

#include "entity/entitybase.h"

class Skybok:public EntityBase{
private:
    Shader m_SkyBoxShader;
    unsigned int m_CubeTexture;
    glm::mat4 m_ViewMat;
    glm::mat4 m_ProjMat;

public:
    Skybok(){
        m_EntityType = E_Entity_Skybox;

        Mesh* cbmesh = MeshManager::Instance()->GetMeshByName("cube");
        if(nullptr == cbmesh){
            //error
        }
        meshes.push_back(*cbmesh);
        m_CubeTexture =  InitCubeMap();
        string shaderpathprefix = "../Resources/Shaders/";
        m_SkyBoxShader.InitShader((shaderpathprefix+"skybox.vs").c_str(), (shaderpathprefix+"skybox.fs").c_str());
    }

    void UpdateViewProj(glm::mat4 view,glm::mat4 proj){
        m_ViewMat = glm::mat4(glm::mat3(view));
        m_ProjMat = proj;
    }

    void DrawEntity() override{
        Mesh mesh = meshes[0];

        glDepthFunc(GL_LEQUAL);
        m_SkyBoxShader.use();
        m_SkyBoxShader.setInt("skybox",0);
        m_SkyBoxShader.setMat4("view", m_ViewMat);
        m_SkyBoxShader.setMat4("projection",m_ProjMat);
        glBindVertexArray(mesh.VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
    }

private:
    unsigned int InitCubeMap()
    {
        string texrefix = "../Resources/textures/skybox/";
        vector<std::string> faces
                {
                        texrefix+"right.jpg",
                        texrefix+ "left.jpg",
                        texrefix+"top.jpg",
                        texrefix+"bottom.jpg",
                        texrefix+"front.jpg",
                        texrefix+"back.jpg"
                };

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }


};

#endif //LEARNOPENGL_SKYBOX_H
