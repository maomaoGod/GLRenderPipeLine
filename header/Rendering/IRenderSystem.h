//
// Created by 钟瑞 on 2021/9/28.
//

#ifndef LEARNOPENGL_IRENDERSYSTEM_H
#define LEARNOPENGL_IRENDERSYSTEM_H
#include<vector>
#include<list>
#include "entity/entitybase.h"

using namespace std;



class IRenderSystem{

public:
    enum ERenderStage{
        E_Render_None,
        E_Render_Normal,
        E_Render_Depth,
        E_Render_Stencil,
        E_Render_Shadow,
        E_Render_PostProcess,
    };

protected:
    glm::vec3 m_ViewPos;
    glm::mat4 m_View;
    glm::mat4 m_Projection;
    static const unsigned int SCR_WIDTH = 800;
    static const unsigned int SCR_HEIGHT = 600;
    int m_WindowWidth;
    int m_WindowHeight;
    bool m_bEnableDepthView;
    bool m_bEnablePostProcess;
    bool m_bEnableShadowView;
    ERenderStage m_RenderStage;
public:
    virtual void UpdateViewPos(glm::vec3 viewpos){
        m_ViewPos = viewpos;
    }
    virtual void UpdateViewProjection(glm::mat4 view,glm::mat4 proj)=0;
    virtual void BeginRender()=0;
    virtual void RenderObjects(SceneEntity entities) = 0;
    virtual void EndRender() = 0;

    virtual void UpdateWindowSize(float width,float height){
        m_WindowWidth = width;
        m_WindowHeight = height;
    }

protected:
    GLenum BuildFrameBuffer(unsigned int &fbo,unsigned int &rtt,unsigned int &rbo){

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // 创建一个texture attachment
        // texture attatchment 用于存储color
        glGenTextures(1, &rtt);
        glBindTexture(GL_TEXTURE_2D, rtt);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rtt, 0);

        //创建一个renderbuffer object
        // renderbuffer object 用来存储 depth and stencil attachment
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (result != GL_FRAMEBUFFER_COMPLETE)
            cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return result;
    }
};

#endif //LEARNOPENGL_IRENDERSYSTEM_H
