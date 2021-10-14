//
// Created by 钟瑞 on 2021/10/11.
//

#ifndef LEARNOPENGL_SCREENRENDERQUAD_H
#define LEARNOPENGL_SCREENRENDERQUAD_H
using namespace std;
#include <vector>
#include <mesh/mesh.h>
#include <entity/entitybase.h>
#include "MeshManager.h"

class ScreenRenderQuad:public EntityBase{
private:
    //深度图小窗口
    unsigned int depthframebuffer;
    unsigned int depthtextureColorbuffer;
    unsigned int depthrenderbufferobj;
    Shader m_ObjRenderShader;   //渲染物体所需的shader


public:
    ScreenRenderQuad(){
        m_EntityType = E_Entity_Quad;

        Mesh* pmesh = MeshManager::Instance()->GetMeshByName("quad");

        meshes.push_back(*pmesh);
    }

    void InitFrameBuffer(unsigned int dfb,unsigned int dcb,unsigned int drb){
        depthframebuffer = dfb;
        depthtextureColorbuffer = dcb;
        depthrenderbufferobj = drb;
    }

    void InitObjRenderShader(string vs,string fs){
        m_ObjRenderShader.InitShader(vs.c_str(),fs.c_str());
        //m_ObjRenderShader.use();
    }

    unsigned int GetFrameBuffer() const{
        return depthframebuffer;
    }

    unsigned int GetTextureBuffer() const{
        return depthtextureColorbuffer;
    }

    unsigned int GetRenderBuffer() const{
        return depthrenderbufferobj;
    }

    Shader& GetRenderObjShader() {
        return m_ObjRenderShader;
    }

    void DrawEntity() override{
        Mesh mesh = meshes[0];
        glBindVertexArray(mesh.VAO);
        glBindTexture(GL_TEXTURE_2D, depthtextureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
};
#endif //LEARNOPENGL_SCREENRENDERQUAD_H
