#pragma once

#include <HTypes.h>
#include <fstream>
#include <string>
#include <unordered_map>

//#include <glad/glad.h>

//enum GLenum;

namespace HEngine {

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    Shader(const std::string& filepath);
    ~Shader();

    virtual void SetBool(const std::string& name, bool value);
    virtual void SetInt(const std::string& name, int value);
    virtual void SetFloat(const std::string& name, const float value);
    virtual void SetFloat3(const std::string& name, const Vec3& value);
    virtual void SetFloat4(const std::string& name, const Vec4& value);
    virtual void SetMat4(const std::string& name, const Mat4& value);

    void UploadUniformBool(const std::string& name, bool value);
    void UploadUniformInt(const std::string& name, int value);

    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformFloat2(const std::string& name, const Vec2& value);
    void UploadUniformFloat3(const std::string& name, const Vec3& value);
    void UploadUniformFloat4(const std::string& name, const Vec4& value);

    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UploadUniformMat4(const std::string& name, const Mat4& matrix);

    std::string readFile(const std::string& filepath);
    std::unordered_map<uint32_t, std::string> preProcess(const std::string& source);
    void compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

    void Bind() const;
    void Unbind() const;

private:
    uint32_t m_RendererID;
};

}
