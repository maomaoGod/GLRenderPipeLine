//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_MATERIAL_H
#define LEARNOPENGL_MATERIAL_H

#include "shader.h"
#include "texture/texture.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

//材质的定义
//材质的shader只能设置一次
//材质可以动态的设置贴图

//贴图索引规则：
//默认规则：
//§ Diffuse = 0
//§ Specular = 1
//§ Normal = 2
//§ Height = 3
//§ Shadow = 4
//也可以自定义规则

class Material{
public:
    enum TexIndexRule{
        E_Index_None,       //默认规则
        E_Index_Custom,     //自定义规则
    };

    enum TexIndex{
        E_Tex_Diffuse = 0,
        E_Tex_Specular = 1,
        E_Tex_Normal = 2,
        E_Tex_Height = 3,
        E_Tex_Shadow = 4,
        E_Tex_IDMax,
    };

    class TexUnit{
    private:
        string m_TexName;
        unsigned int m_TexId;
        TexIndex m_TexIndex;

    public:
        TexUnit(){

        }

        void SetTexName(string name){
            m_TexName = name;
        }

        string GetTexName() const{
            return m_TexName;
        }

        void SetTexId(unsigned int id){
            m_TexId = id;
        }

        unsigned int GetTexId() const{
            return m_TexId;
        }

        void SetTexIdx(TexIndex idx){
            m_TexIndex = idx;
        }

        TexIndex GetTexIdx() const{
            return m_TexIndex;
        }
    };

private:
    Shader m_Shader;
    bool m_bAlpha;
    TexIndexRule m_TexRule;
    map<TexIndex,TexUnit*> m_Tex2Unit;

protected:

    virtual void BindShaderSample2D(){
        m_Shader.use();
        auto texid =  glGetUniformLocation(m_Shader.ID,"diffuseMap");
        if(texid>=0) {
            m_Shader.setInt("diffuseMap", TexIndex::E_Tex_Diffuse);
        }

        texid = glGetUniformLocation(m_Shader.ID,"specularMap");
        if(texid>=0){
            m_Shader.setInt("specularMap",TexIndex::E_Tex_Specular);
        }

        texid = glGetUniformLocation(m_Shader.ID,"normalMap");
        if(texid>=0){
            m_Shader.setInt("normalMap",TexIndex::E_Tex_Normal);
        }

        texid = glGetUniformLocation(m_Shader.ID,"heightMap");
        if(texid>=0){
            m_Shader.setInt("heightMap",TexIndex::E_Tex_Height);
        }

        texid = glGetUniformLocation(m_Shader.ID,"shadowMap");
        if(texid>=0){
            m_Shader.setInt("shadowMap",TexIndex::E_Tex_Shadow);

        }
    }

    virtual void InitShaderTexture(){
        m_Tex2Unit.clear();

        if(m_TexRule == TexIndexRule::E_Index_None){
            {
                TexUnit* txunit = new TexUnit();
                txunit->SetTexName("diffuseMap");
                txunit->SetTexIdx(TexIndex::E_Tex_Diffuse);
                m_Tex2Unit.insert(pair<TexIndex,TexUnit*>(TexIndex::E_Tex_Diffuse,txunit));
            }

            {
                TexUnit* txunit = new TexUnit();
                txunit->SetTexName("specularMap");
                txunit->SetTexIdx(TexIndex::E_Tex_Specular);
                m_Tex2Unit.insert(pair<TexIndex,TexUnit*>(TexIndex::E_Tex_Specular,txunit));
            }

            {
                TexUnit* txunit = new TexUnit();

                txunit->SetTexName("normalMap");
                txunit->SetTexIdx(TexIndex::E_Tex_Normal);
                m_Tex2Unit.insert(pair<TexIndex,TexUnit*>(TexIndex::E_Tex_Normal,txunit));
            }

            {
                TexUnit* txunit = new TexUnit();
                txunit->SetTexName("heightMap");
                txunit->SetTexIdx(TexIndex::E_Tex_Height);
                m_Tex2Unit.insert(pair<TexIndex,TexUnit*>(TexIndex::E_Tex_Height,txunit));
            }

            {
                TexUnit* txunit = new TexUnit();
                txunit->SetTexName("shadowMap");
                txunit->SetTexIdx(TexIndex::E_Tex_Shadow);
                m_Tex2Unit.insert(pair<TexIndex,TexUnit*>(TexIndex::E_Tex_Shadow,txunit));
            }

        }else{

        }
    }

    virtual TexUnit* FindTexUnitByName(string name){
        TexIndex idx = TexIndex::E_Tex_IDMax;
        if(name.compare("diffuseMap") == 0){
            idx = TexIndex::E_Tex_Diffuse;
        }else if(name.compare("specularMap") == 0){
            idx = TexIndex::E_Tex_Diffuse;
        }else if(name.compare("normalMap") == 0){
            idx = TexIndex::E_Tex_Normal;
        }else if(name.compare("heightMap") == 0){
            idx = TexIndex::E_Tex_Height;
        }else if(name.compare("shadowMap") == 0){
            idx = TexIndex::E_Tex_Shadow;
        }

        return FindTexUnitByIndex(idx);
    }

    virtual TexUnit* FindTexUnitByIndex(TexIndex index){

        auto id = m_Tex2Unit.find(index);
        if(id != m_Tex2Unit.end()){
            return id->second;
        }

        return nullptr;
    }

public:
    Shader GetMatShader() const{
        return m_Shader;
    }

    unsigned int GetShadowTexture(){
        auto shadowunit = m_Tex2Unit.find(E_Tex_Shadow);
        if(shadowunit != m_Tex2Unit.end()){
            return (*shadowunit).second->GetTexId();
        }else{
            std::cout<<"error: cannot find shaodw texture"<<endl;
        }

        return 0;
    }

    TexIndexRule GetTexRule() const{
        return m_TexRule;
    }

public:
    Material(){
        m_TexRule = E_Index_None;
        m_bAlpha = false;
        InitShaderTexture();
    }

    void SetAlpha(bool balpha){
        m_bAlpha = balpha;
    }

    bool GetAlpha() const{
        return m_bAlpha;
    }

    void SetUpShader(Shader &shader,TexIndexRule texIndexRule = TexIndexRule::E_Index_None){
        m_Shader = shader;
        m_TexRule = texIndexRule;

        BindShaderSample2D();
    }

    void SetUpShader(string vert,string frag,string geo){

        if(geo.empty()){
            m_Shader.InitShader(vert.c_str(),frag.c_str());
        }else{
            m_Shader.InitShader(vert.c_str(),frag.c_str(),geo.c_str());
        }

        BindShaderSample2D();
    }

    void SetUpTextures(map<string,string> txs){
        if(!txs.empty()){
            auto it = txs.begin();
            vector<Texture> txtemp;
            for(;it!= txs.end();++it){
                Texture txit(it->second);
                txit.TextureType(it->first);
                txtemp.clear();
                txtemp.push_back(txit);
                SetUpTextures(txtemp);
            }
        }
    }

    void SetUpTextures(vector<Texture> txs){
        if(txs.size()>0){
            auto it = txs.begin();
            for(;it != txs.end();++it){
                auto txunit = FindTexUnitByName(it->TextureType());
                if(nullptr != txunit){
                    txunit->SetTexId(it->TextureID());
                }else{
                    cout<<"error:invalid texture type:"<<it->TextureType()<<endl;
                }
            }
        }
    }

    void SetUpShadowTexture(unsigned int shadowtx){
        auto shadowunit = m_Tex2Unit.find(E_Tex_Shadow);
        if(shadowunit != m_Tex2Unit.end()){
            (*shadowunit).second->SetTexId(shadowtx);
        }else{
            std::cout<<"error: cannot find shaodw texture"<<endl;
        }
    }

    virtual void UnBindTextures(){
        glActiveTexture(GL_TEXTURE0);
    }

    virtual void BindTextures(){


        if(m_TexRule == E_Index_None){
            auto it = m_Tex2Unit.begin();
            for(;it != m_Tex2Unit.end();++it){
                glActiveTexture(GL_TEXTURE0+it->second->GetTexIdx());
                glBindTexture(GL_TEXTURE_2D,it->second->GetTexId());
            }
        }else{
            // bind appropriate textures
//            unsigned int diffuseNr  = 1;
//            unsigned int specularNr = 1;
//            unsigned int normalNr   = 1;
//            unsigned int heightNr   = 1;
//            auto textures = m_Textures;
//            for(unsigned int i = 0; i < textures.size(); i++)
//            {
//                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
//                // retrieve texture number (the N in diffuse_textureN)
//                string number;
//                string name = textures[i].TextureType();
//                if(name == "texture_diffuse")
//                    number = std::to_string(diffuseNr++);
//                else if(name == "texture_specular")
//                    number = std::to_string(specularNr++); // transfer unsigned int to stream
//                else if(name == "texture_normal")
//                    number = std::to_string(normalNr++); // transfer unsigned int to stream
//                else if(name == "texture_height")
//                    number = std::to_string(heightNr++); // transfer unsigned int to stream
//
//                // now set the sampler to the correct texture unit
//                glUniform1i(glGetUniformLocation(m_Shader.ID, (name + number).c_str()), i);
//                // and finally bind the texture
//                glBindTexture(GL_TEXTURE_2D, textures[i].TextureID());
            }

    }
};

#endif //LEARNOPENGL_MATERIAL_H
