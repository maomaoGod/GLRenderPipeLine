//
// Created by 钟瑞 on 2021/10/9.
//

#ifndef LEARNOPENGL_FORWARDRENDERSYSTEM_H
#define LEARNOPENGL_FORWARDRENDERSYSTEM_H

#include "Rendering/IRenderSystem.h"
#include "camera/camera.h"
#include "shader.h"
#include "entity/primitive/screenrenderquad.h"
#include "entity/primitive/skybox.h"
#include "entity/light.h"
#include "postprocess/postprocessmgr.h"

#include<list>
using namespace std;

class ForwardRenderSystem: public IRenderSystem{

private:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    //深度图面片
    ScreenRenderQuad* m_DepthScreenQuad;

    //阴影面片
    ScreenRenderQuad* m_ShadowRenderQuad;

    //后处理面片
    ScreenRenderQuad* m_PostProcessQuad;

    list<EntityBase*> m_CurRenderEntities;

    glm::mat4 lightmat;
    glm::vec3 lightpos;
private:
    ScreenRenderQuad* BuildOneScreenRenderQuad(){
        ScreenRenderQuad* result = nullptr;
        unsigned int depthframebuffer = 0;
        unsigned int depthrenderbufferobj = 0;
        unsigned int depthtextureColorbuffer = 0;

        BuildFrameBuffer(depthframebuffer,depthtextureColorbuffer,depthrenderbufferobj);

        result = new ScreenRenderQuad();

        result->InitFrameBuffer(depthframebuffer,depthtextureColorbuffer,depthrenderbufferobj);

        return result;
    }


    void InitDepthRenderBuffer(){
        m_DepthScreenQuad = BuildOneScreenRenderQuad();
        Material quadmat;
        quadmat.SetUpShader("../Resources/Shaders/DepthView/depth_screen.vs",
                            "../Resources/Shaders/DepthView/depth_screen.fs",
                            "");
        glm::vec3 tmpvec = glm::vec3(0.4f,0.3f,1.0f);
        m_DepthScreenQuad->GetTransform().SetScale(tmpvec );
        tmpvec = glm::vec3(0.6f,0.7f,0.0f);
        m_DepthScreenQuad->GetTransform().SetPosition(tmpvec);
        m_DepthScreenQuad->SetMaterial(quadmat);
        m_DepthScreenQuad->InitObjRenderShader("../Resources/Shaders/DepthView/depth_view.vs",
                                               "../Resources/Shaders/DepthView/depth_view.fs");
    }

    void InitPostRenderBuffer(){
        m_PostProcessQuad = BuildOneScreenRenderQuad();
        Material quadmat;
        glm::vec3 tmpvec = glm::vec3(1.0f,1.0f,1.0f);
        m_PostProcessQuad->GetTransform().SetScale(tmpvec );
        tmpvec = glm::vec3(0.0f,0.0f,0.0f);
        m_PostProcessQuad->GetTransform().SetPosition(tmpvec);
        m_PostProcessQuad->SetMaterial(quadmat);
    }

    void InitShadowRenderBuffer(){
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);
        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_ShadowRenderQuad = BuildOneScreenRenderQuad();
        m_ShadowRenderQuad->InitFrameBuffer(depthMapFBO,depthMap,0);
        glm::vec3 tmpvec = glm::vec3(0.4f,0.3f,1.0f);
        m_ShadowRenderQuad->GetTransform().SetScale(tmpvec );
        tmpvec = glm::vec3(0.6f,0.1f,0.0f);
        m_ShadowRenderQuad->GetTransform().SetPosition(tmpvec);
        Material quadmat;
        quadmat.SetUpShader("../Resources/Shaders/Shadow/shadow_screen.vs",
                            "../Resources/Shaders/Shadow/shadow_screen.fs",
                            "");
        m_ShadowRenderQuad->SetMaterial(quadmat);
        m_ShadowRenderQuad->InitObjRenderShader("../Resources/Shaders/Shadow/light_shadow.vs",
                                                "../Resources/Shaders/Shadow/light_shadow.fs");
    }

public:
    ForwardRenderSystem(){
        m_bEnableDepthView = true;
        m_bEnablePostProcess = true;
        m_bEnableShadowView = false;
        m_View = glm::mat4(1.0f);
        m_Projection = glm::mat4 (1.0f);

        m_WindowWidth = SCR_WIDTH*2;
        m_WindowHeight = SCR_HEIGHT*2;
        InitDepthRenderBuffer();
        InitPostRenderBuffer();
        InitShadowRenderBuffer();
    }


    void UpdateViewProjection(glm::mat4 view,glm::mat4 proj) override{
        m_View = view;
        m_Projection = proj;
    }

    void BeginRender() override{
        //清理buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_RenderStage = E_Render_Normal;
        //glViewport(0,0,600,800);
    }

    void RenderObjects(SceneEntity entities) override{

        //深度图
        if(m_bEnableDepthView){
            BeginDepthRender();
            DoOneRenderLoop(entities);
            EndDepthRender();
        }

        //阴影
        auto lights = entities.GetLightObj();
        if(lights.size()>0){
            BeginShadowRender();
            DoOneRenderLoop(entities);
            EndShadowRender();
        }

        //常规渲染
        if(m_bEnablePostProcess){
            BeginPostRender();
            DoOneRenderLoop(entities);
            EndPostRender();
        }else{
            //常规渲染
            DoOneRenderLoop(entities);
        }

    }

    void BeginPostRender(){
        glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessQuad->GetFrameBuffer());
        glViewport(0,0,m_WindowWidth,m_WindowHeight);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
        // make sure we clear the framebuffer's content
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);
        m_RenderStage = E_Render_PostProcess;
    }

    void EndPostRender(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,m_WindowWidth,m_WindowHeight);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        m_RenderStage = E_Render_Normal;

        //执行post effect
        //禁用深度测试，否则离屏渲染的fragment可能会被discard
        glDisable(GL_DEPTH_TEST);

        //选择一种post effect
        auto cureffect =  PostProcessMgr::Instance()->GetCurPostEffect();
        cureffect->use();
        //设置位置
        glm::mat4 model = m_PostProcessQuad->GetTransform().GetModelMat();
        cureffect->setMat4("model",model);
        //绘制面片
        m_PostProcessQuad->DrawEntity();
    }

    void BeginDepthRender(){
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthScreenQuad->GetFrameBuffer());
        glViewport(0,0,m_WindowWidth,m_WindowHeight);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
        // make sure we clear the framebuffer's content
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);
        m_RenderStage = E_Render_Depth;
    }

    void EndDepthRender(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,m_WindowWidth,m_WindowHeight);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        m_RenderStage = E_Render_Normal;
    }

    void BeginShadowRender(){
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        auto fbo = m_ShadowRenderQuad->GetFrameBuffer();
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        m_RenderStage = E_Render_Shadow;
    }

    void EndShadowRender(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_WindowWidth,m_WindowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_RenderStage = E_Render_Normal;
    }

    void DoOneRenderLoop(SceneEntity sceneEntity){

        list<EntityBase*> entities;
        auto unalpha = sceneEntity.GetUnAlphaObj();
        auto alpha = sceneEntity.GetAlphaObj();
        if(alpha.size()>0){
            std::map<float, EntityBase*> sorted;
            auto it = alpha.begin();
            for(; it != alpha.end(); ++it)
            {
                float dist = glm::length(m_ViewPos - *((*it)->GetTransform().GetPosition()));
                sorted[dist] = *it;
            }

            m_CurRenderEntities.clear();
            auto unit = unalpha.begin();
            for(;unit != unalpha.end();++unit){
                m_CurRenderEntities.push_back(*unit);
            }

            auto ab = sorted.rbegin();
            for(;ab!= sorted.rend();++ab){
                m_CurRenderEntities.push_back(ab->second);
            }

            entities = m_CurRenderEntities;

        }else{
            entities = unalpha;
        }
        //对物体进行排序
        //先绘制不透明物体
        //再绘制透明物体
        std::list<EntityBase*>::iterator it = entities.begin();
        switch (m_RenderStage) {
            case IRenderSystem::E_Render_Normal:
            case IRenderSystem::E_Render_PostProcess:
            {
                //执行forward render：
                auto shadowid = m_ShadowRenderQuad->GetTextureBuffer();
                for(;it!= entities.end();it++){
                    EntityBase et = *(*it);

                    if(shadowid>0){
                        et.GetMaterial().SetUpShadowTexture(shadowid);
                    }

                    auto sdid = et.GetMaterial().GetMatShader();

                    sdid.use();
                    sdid.setMat4("view",m_View);
                    sdid.setMat4("projection",m_Projection);
                    sdid.setMat4("model", et.GetTransform().GetModelMat());
                    sdid.setMat4("lightSpaceMatrix",lightmat);
                    sdid.setVec3("viewPos",m_ViewPos);
                    sdid.setVec3("lightPos",lightpos);
                    et.DrawEntity();
                }

                auto et = sceneEntity.GetSkyBox();
                if(nullptr != et){
                    Skybok* skybox = dynamic_cast<Skybok*>(et);
                    skybox->UpdateViewProj(m_View,m_Projection);
                    skybox->DrawEntity();
                }
            }
                break;
            case IRenderSystem::E_Render_Shadow:
            {
                auto lights = sceneEntity.GetLightObj();
                if(lights.size()>0){
                    m_bEnableShadowView = true;
                    Shader sd = m_ShadowRenderQuad->GetRenderObjShader();
                    sd.use();
                    auto lightit = lights.begin();
                    for(;lightit != lights.end();++lightit){
                        Light* curlt = dynamic_cast<Light*>(*lightit);
                        glm::mat4 lightSpaceMatrix = curlt->GetLightProj() * curlt->GetLightView();
                        lightmat = lightSpaceMatrix;
                        lightpos = *(curlt->GetTransform().GetPosition());
                        sd.setMat4("lightSpaceMatrix",lightSpaceMatrix);
                        for(;it!= entities.end();it++){
                            EntityBase et = *(*it);
                            if(et.GetCastShadow()){
                                sd.setMat4("model", et.GetTransform().GetModelMat());
                                et.DrawEntity();
                            }
                        }
                    }

                }else{
                    m_bEnableShadowView = false;
                }
            }
                break;
            case IRenderSystem::E_Render_Depth:
            {
                auto m_DepthShader = m_DepthScreenQuad->GetRenderObjShader();
                m_DepthShader.use();
                m_DepthShader.setMat4("view",m_View);
                m_DepthShader.setMat4("projection",m_Projection);
                for(;it!= entities.end();it++){
                    EntityBase* et = *it;
                    m_DepthShader.setMat4("model", et->GetTransform().GetModelMat());
                    et->DrawEntity();
                }
            }
                break;
        }

    }

    void EndRender() override{

        if(m_bEnableShadowView){
            glDisable(GL_DEPTH_TEST);
            auto sd = m_ShadowRenderQuad->GetMaterial().GetMatShader();
            sd.use();
            sd.setFloat("near_plane", 1.0f);
            sd.setFloat("far_plane", 7.5f);
            DrawOneRenderQuad(m_ShadowRenderQuad);
        }

        if(m_bEnableDepthView){
            //禁用深度测试，否则离屏渲染的fragment可能会被discard
            glDisable(GL_DEPTH_TEST);
            m_DepthScreenQuad->GetMaterial().GetMatShader().use();
            DrawOneRenderQuad(m_DepthScreenQuad);
        }
    }

    void DrawOneRenderQuad(ScreenRenderQuad * quad){
        if(nullptr == quad){
            return;
        }

        glm::mat4 model = quad->GetTransform().GetModelMat();
        quad->GetMaterial().GetMatShader().setMat4("model",model);
        quad->DrawEntity();
    }
};

#endif //LEARNOPENGL_FORWARDRENDERSYSTEM_H
