#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

//#include <glad/glad.h>

//enum GLenum;

namespace HEngine {



	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		Shader(const std::string& filepath);
		~Shader();

		std::string readFile(const std::string& filepath);
		std::unordered_map<uint32_t, std::string> preProcess(const std::string& source);
		void compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;

	};

}