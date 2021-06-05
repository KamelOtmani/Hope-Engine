#pragma once


namespace HEngine {

    class Model;
    class Shader;
    

    class Scene 
    {
    public:
        Scene() = default;

        void setupScene();
    
        std::vector<Model> entityList;
        std::vector<Shader*> shaderList;
    };

}
