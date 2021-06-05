#pragma once


namespace HEngine {

    class Model;
    class Shader;
    class Camera;
    

    class Scene 
    {
    public:
        Scene() = default;

        void setupScene();
    
        std::vector<Model> entityList;
        std::vector<Shader*> shaderList;
        std::vector<Camera*> CameraList;
    };

}
