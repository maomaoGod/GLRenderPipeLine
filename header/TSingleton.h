//
// Created by 钟瑞 on 2021/9/30.
//

#ifndef LEARNOPENGL_TSINGLETON_H
#define LEARNOPENGL_TSINGLETON_H

template<typename T>
class TSingleton{
protected:
    TSingleton(const TSingleton<T> &);
    TSingleton<T>& operator=(const TSingleton<T> &);

public:
    static T* Instance(){
        static T _instance;
        return &_instance;
    }

    virtual ~TSingleton(){

    }

    virtual void Initialize(){

    }

    virtual void UnInitialize(){

    }

protected:
    TSingleton(){

    }
};

#endif //LEARNOPENGL_TSINGLETON_H
