//
// Created by 钟瑞 on 2021/10/12.
//

#ifndef LEARNOPENGL_POSTPROCESSMGR_H
#define LEARNOPENGL_POSTPROCESSMGR_H


#include "TSingleton.h"
#include "shader.h"
#include <list>
#include <string>
#include <map>
using namespace std;

class PostProcessMgr:public TSingleton<PostProcessMgr>{
    friend class TSingleton<PostProcessMgr>;

private:
    map<string,Shader> m_PostEffects;
    list<string> m_PostEffectBuildIn;
    string m_PrefixName;
    string m_CurEffect;


protected:
    PostProcessMgr(){
        m_PostEffects.clear();
        m_PostEffectBuildIn.clear();
        m_PostEffectBuildIn.push_back("default");
        m_PostEffectBuildIn.push_back("inversion");
        m_PostEffectBuildIn.push_back("grayscale");
        m_PostEffectBuildIn.push_back("blur");
        m_PostEffectBuildIn.push_back("edge_detection");

        m_PrefixName = "../Resources/Shaders/PostProcess/effect_";
        m_CurEffect = "default";
        Initialize();
    }

public:

    void Initialize() override{

        auto it = m_PostEffectBuildIn.begin();
        for(;it != m_PostEffectBuildIn.end();++it){
            if(m_PostEffects.find(*it) != m_PostEffects.end()){

            }else{
                Shader tmp;
                auto vert = m_PrefixName+*it+".vs";
                auto frag = m_PrefixName+*it+".fs";
                tmp.InitShader(vert.c_str(),frag.c_str());

                m_PostEffects.insert(pair<string,Shader>(*it,tmp));
            }
        }

    }

    void UnInitialize() override {
        m_PostEffects.clear();
    }

    void ChangeToNextEffect(){
        auto it = m_PostEffectBuildIn.begin();
        for(;it!= m_PostEffectBuildIn.end();++it){
            if(it->compare(m_CurEffect) == 0){
                auto newit = ++it;
                if(newit == m_PostEffectBuildIn.end()){
                    m_CurEffect = *m_PostEffectBuildIn.begin();
                }else{
                    m_CurEffect = *newit;
                }

                break;
            }
        }
    }

    Shader* GetCurPostEffect(){
        return FindPostEffect(m_CurEffect);
    }

    Shader* FindPostEffect(string name){
        Shader* result = nullptr;
        if(m_PostEffects.find(name) != m_PostEffects.end()){
            result = &m_PostEffects[name];
        }else{
            result = &m_PostEffects["default"];
        }

        return result;
    }
};

#endif //LEARNOPENGL_POSTPROCESSMGR_H
