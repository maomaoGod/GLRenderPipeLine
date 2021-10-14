//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_MATERIAL_H
#define LEARNOPENGL_MATERIAL_H

#include "shader.h"
#include "texture/texture.h"
#include <vector>
#include <string>
using namespace std;

//材质的定义
//材质的shader只能设置一次
//材质可以动态的设置贴图
class Material{
private:
    Shader m_Shader;
    vector<Texture> m_Textures;
    bool m_bAlpha;
    unsigned int m_iShadowTexture;
public:
    Shader GetMatShader() const{
        return m_Shader;
    }

    vector<Texture> GetMatTexture() const{
        return m_Textures;
    }

    unsigned int GetShadowTexture(){
        return m_iShadowTexture;
    }

public:
    Material(){
        m_bAlpha = false;
    }

    void SetAlpha(bool balpha){
        m_bAlpha = balpha;
    }

    bool GetAlpha() const{
        return m_bAlpha;
    }

    void SetUpShader(Shader &shader){
        m_Shader = shader;
    }

    void SetUpShader(string vert,string frag,string geo){

        if(geo.empty()){
            m_Shader.InitShader(vert.c_str(),frag.c_str());
        }else{
            m_Shader.InitShader(vert.c_str(),frag.c_str(),geo.c_str());
        }
    }

    void SetUpTextures(vector<string> txs){
        if(!txs.empty()){
            vector<string>::iterator it = txs.begin();
            for(;it!= txs.end();++it){
                Texture txit(*it);
                if(txit.TextureID()>0){
                    m_Textures.push_back(txit);
                }
            }
        }
    }

    void SetUpTextures(vector<Texture> txs){

        m_Textures.clear();
        m_Textures.insert(m_Textures.begin(),txs.begin(),txs.end());
    }

    void SetUpShadowTexture(unsigned int shadowtx){
        m_iShadowTexture = shadowtx;
    }

};

#endif //LEARNOPENGL_MATERIAL_H
