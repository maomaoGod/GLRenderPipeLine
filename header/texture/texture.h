//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

//纹理的使用分为三部
//1、创建纹理对象
//2、加载并分配一个纹理id
//3、激活并绑定纹理 (在渲染流程中处理)
#include <stbimage/stb_image.h>
#include<string>
using namespace std;

class Texture{
private:
    unsigned int m_txId;
    string m_txType;
    string m_txPath;
public:
    Texture(){

    }

    Texture(string txpath){

        if(!txpath.empty()){
            unsigned int txid = LoadTexture(txpath.c_str());
            if(txid>0){}
            m_txPath = txpath;
            m_txId = txid;
        }
    }

    unsigned int LoadTexture(char const * path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            GLenum cutparam = GL_REPEAT;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4){
                format = GL_RGBA;
                cutparam = GL_CLAMP_TO_EDGE;
            }

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, cutparam);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, cutparam);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    unsigned int TextureID(){
        return m_txId;
    }

    void TextureID(unsigned int txid){
        m_txId = txid;
    }

    string  TextureType(){
        return m_txType;
    }

    void TextureType(string txtype){
        m_txType = txtype;
    }


    string TexturePath(){
        return m_txPath;
    }

    void TexturePath(string txpath){
        m_txPath = txpath;
    }
};

#endif //LEARNOPENGL_TEXTURE_H
