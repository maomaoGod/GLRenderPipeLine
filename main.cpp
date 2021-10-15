#include <glad/glad.h>
#include <glfw/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera/camera.h>
#include <entity/model/model.h>
#include <entity/primitive/cube.h>
#include <entity/primitive/plane.h>
#include <entity/primitive/skybox.h>
#include <entity/light.h>
#include <Scene.h>


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_size_callback(GLFWwindow* wwindow,int width,int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
GLenum BuildFrameBuffer(unsigned int &fbo,unsigned int &rtt,unsigned int &rbo);


float lastX = (float)Camera::SCR_WIDTH / 2.0;
float lastY = (float)Camera::SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Scene * mainscene = nullptr;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation

    // --------------------
    GLFWwindow* window = glfwCreateWindow(Camera::SCR_WIDTH, Camera::SCR_HEIGHT, "GraphPipeLine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window,window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------

    //glEnable(GL_STENCIL_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // build and compile shaders
    // -------------------------
    string shaderpathprefix = "../Resources/Shaders/";

    //创建场景
    Scene scene;
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    scene.AddCamera(&camera);
    //创建cube
    map<string,string> txpath;
    EntityBase* cub1 = new Cube();
    glm::vec3 tmpvec = glm::vec3(1.0f,1.0f,1.0f);
    cub1->GetTransform().SetScale(tmpvec );
    tmpvec = glm::vec3(-1.0f,0.6f,-1.0f);
    cub1->GetTransform().SetPosition(tmpvec);
    Material cb1mat;
    cb1mat.SetUpShader(shaderpathprefix+"/Shadow/shadow_mapping.vs",shaderpathprefix+"/Shadow/shadow_mapping.fs","");
    //cb1mat.SetUpShader(shaderpathprefix+"/5.2.framebuffers.vs",shaderpathprefix+"/5.2.framebuffers.fs","");
    txpath.clear();
    txpath.insert( pair<string,string>("diffuseMap","../Resources/textures/container.jpg"));
    cb1mat.SetUpTextures(txpath);
    cub1->SetMaterial(cb1mat);
    cub1->SetCastShadow(true);

    EntityBase* cub2 = new Cube();
    tmpvec = glm::vec3(1.0f,1.0f,1.0f);
    cub2->GetTransform().SetScale(tmpvec );
    tmpvec = glm::vec3(2.0f,0.0f,0.0f);
    cub2->GetTransform().SetPosition(tmpvec);
    cub2->SetMaterial(cb1mat);
    cub2->SetCastShadow(true);

    //创建floor
    EntityBase* floor = new Plane();
    tmpvec = glm::vec3(10.0f,1.0f,10.0f);
    floor->GetTransform().SetScale(tmpvec );
    tmpvec = glm::vec3(0.0f,-0.5f,0.0f);
    floor->GetTransform().SetPosition(tmpvec);
    Material planemat;
    //planemat.SetUpShader(shaderpathprefix+"/5.2.framebuffers.vs",shaderpathprefix+"/5.2.framebuffers.fs","");
    planemat.SetUpShader(shaderpathprefix+"/Shadow/shadow_mapping.vs",shaderpathprefix+"/Shadow/shadow_mapping.fs","");
    txpath.clear();
    txpath.insert(pair<string,string>("diffuseMap", "../Resources/textures/metal.png"));
    planemat.SetUpTextures(txpath);
    floor->SetMaterial(planemat);

    //添加一些草、玻璃
    vector<glm::vec3> grasspos;
    grasspos.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    grasspos.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    grasspos.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    grasspos.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    grasspos.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));

    auto grassit = grasspos.begin();
    Material grassmat;
    grassmat.SetAlpha(true);
    grassmat.SetUpShader(shaderpathprefix+"/5.2.framebuffers.vs",shaderpathprefix+"/5.2.framebuffers.fs","");
    txpath.clear();
    txpath.insert(pair<string,string>("diffuseMap","../Resources/textures/grass.png"));
    grassmat.SetUpTextures(txpath);
    for(;grassit != grasspos.end();++grassit){
        EntityBase* grass = new Plane();
        tmpvec = *grassit;
        grass->GetTransform().SetPosition(tmpvec);
        grass->GetTransform().SetRotation(glm::vec3(glm::radians(-90.0f),0.0f,0.0f));
        grass->SetMaterial(grassmat);
        //scene.AddSceneObj(grass);
    }

    grassit = grasspos.begin();
    Material glassmat;
    glassmat.SetAlpha(true);
    glassmat.SetUpShader(shaderpathprefix+"/5.2.framebuffers.vs",shaderpathprefix+"/5.2.framebuffers.fs","");
    txpath.clear();
    txpath.insert(pair<string,string>("diffuseMap","../Resources/textures/window.png"));
    glassmat.SetUpTextures(txpath);
    for(;grassit != grasspos.end();++grassit){
        EntityBase* glass = new Plane();
        tmpvec = *grassit+glm::vec3(0.0f,0.0f,1.1f);
        glass->GetTransform().SetPosition(tmpvec);
        glass->GetTransform().SetRotation(glm::vec3(glm::radians(-90.0f),0.0f,0.0f));
        glass->SetMaterial(glassmat);
        //scene.AddSceneObj(glass);
    }

    //创建一个天空盒
    EntityBase* skybox = new Skybok();
    scene.AddSceneObj(skybox);


    //添加灯光
    EntityBase* light = new Light();
    glm::vec3 lightPos(-2.0f, 2.0f, -3.0f);
    light->SetPosition(lightPos);
    tmpvec = glm::vec3(0.2f,0.2f,0.2f);
    light->GetTransform().SetScale(tmpvec );
    light->SetMaterial(cb1mat);
    scene.AddSceneObj(light);

    //创建一个模型
    Model suitmodel("../Resources/objects/nanosuit2/nanosuit.obj");
    Material normalMat;
    Shader normalShader("../Resources/Shaders/model_loading.vs", "../Resources/Shaders/model_loading.fs");

//    Shader suitexplodeShader((shaderpathprefix+"geometry_explode_shader.vs").c_str(),
//                             (shaderpathprefix+"geometry_explode_shader.fs").c_str(),
//                             (shaderpathprefix+"geometry_explode_shader.gs").c_str());

//    Shader suitnormalvisualShader((shaderpathprefix+"geometry_normal_visualization.vs").c_str(),
//                                  (shaderpathprefix+"geometry_normal_visualization.fs").c_str(),
//                                  (shaderpathprefix+"geometry_normal_visualization.gs").c_str());
    normalMat.SetUpShader(normalShader);
    suitmodel.SetMaterial(normalMat);
    tmpvec = glm::vec3(0.1f,0.1f,0.1f);
    suitmodel.GetTransform().SetScale(tmpvec );
    tmpvec = glm::vec3(4.0f,1.5f,0.0f);
    suitmodel.GetTransform().SetPosition(tmpvec);
    //scene.AddSceneObj(&suitmodel);

    Model back("../Resources/objects/backpack/backpack.obj");
    tmpvec = glm::vec3(0.4f,0.4f,0.4f);
    back.GetTransform().SetScale(tmpvec );
    back.SetMaterial(normalMat);
    //scene.AddSceneObj(&back);

    Model planet("../Resources/objects/planet/planet.obj");
    planet.SetMaterial(normalMat);
    tmpvec = glm::vec3(0.1f,0.1f,0.1f);
    planet.GetTransform().SetScale(tmpvec );
    tmpvec = glm::vec3(2.0f,3.5f,0.0f);
    planet.GetTransform().SetPosition(tmpvec);
    //scene.AddSceneObj(&planet);

    Model cyborg("../Resources/objects/cyborg/cyborg.obj");
    cyborg.SetMaterial(normalMat);
    tmpvec = glm::vec3(0.3f,0.3f,0.3f);
    cyborg.GetTransform().SetScale(tmpvec );
    tmpvec = glm::vec3(0.0f,0.0f,0.8f);
    cyborg.GetTransform().SetPosition(tmpvec);
    //scene.AddSceneObj(&cyborg);

    //Model sponza("../Resources/objects/crytek-sponza/sponza.obj");
    //sponza.SetMaterial(normalMat);
    //scene.AddSceneObj(&sponza);

    //将物体添加到场景中
    scene.AddSceneObj(cub1);
    scene.AddSceneObj(cub2);
    scene.AddSceneObj(floor);

    mainscene = &scene;

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        // 渲染
        scene.DoRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(nullptr != mainscene){
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            mainscene->ProcessKeyInput(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            mainscene->ProcessKeyInput(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            mainscene->ProcessKeyInput(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            mainscene->ProcessKeyInput(RIGHT, deltaTime);
        if(glfwGetKey(window,GLFW_KEY_P) == GLFW_PRESS){
            PostProcessMgr::Instance()->ChangeToNextEffect();
        }
    }

}

void window_size_callback(GLFWwindow* wwindow,int width,int height){
    if(nullptr != mainscene){
        //mainscene->ProcessWindowSizeChange(width,height);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if(nullptr != mainscene){
        mainscene->ProcessWindowSizeChange(width,height);
    }
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if(nullptr != mainscene){
        mainscene->ProcessMouseMove(xoffset,yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(nullptr != mainscene){
        mainscene->ProcessMouseScroll(xoffset,yoffset);
    }

}

