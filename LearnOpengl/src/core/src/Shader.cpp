#include "Shader.h"
#include "log.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>

// #include "ShaderSource.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

	std::string vertexCode = readFromFile(vertexPath);
	std::string fragmentCode = readFromFile(fragmentPath);

	PipelineShader pipelineShader;
	pipelineShader.VertexShader = createShader(vertexCode, GL_VERTEX_SHADER);
	pipelineShader.FragmentShader = createShader(fragmentCode, GL_FRAGMENT_SHADER);

	shaderProgram = createProgram(pipelineShader);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
	std::string vertexCode = readFromFile(vertexPath);
	std::string fragmentCode = readFromFile(fragmentPath);
	std::string geometryCode = readFromFile(geometryPath);

	PipelineShader pipelineShader;
	pipelineShader.VertexShader = createShader(vertexCode, GL_VERTEX_SHADER);
	pipelineShader.FragmentShader = createShader(fragmentCode, GL_FRAGMENT_SHADER);
	pipelineShader.GeometryShader = createShader(geometryCode, GL_GEOMETRY_SHADER);

	shaderProgram = createProgram(pipelineShader);
}

void Shader::use() { 
	CheckCall(glUseProgram(shaderProgram)); 
}

void Shader::setBool(const std::string& name, bool value) const {
	CheckCall(glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value));
}

void Shader::setInt(const std::string& name, int value) const {
	CheckCall(glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value));
}

void Shader::setFloat(const std::string& name, float value) const {
	CheckCall(glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value));
}

GLuint Shader::createShader(const std::string& codeStr, GLuint shaderType)
{
	GLuint shader;
	int success;
	char infoLog[512];
	const char* fShaderCode = codeStr.c_str();

	shader = glCreateShader(shaderType);
	CheckCall(glShaderSource(shader, 1, &fShaderCode, NULL));
	CheckCall(glCompileShader(shader));
	CheckCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success) {
		CheckCall(glGetShaderInfoLog(shader, 512, NULL, infoLog));
		LOGE("ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED {}", infoLog);
	}
	return shader;
}

GLuint Shader::createProgram(const PipelineShader& pipelineShader)
{
	GLuint _program = glCreateProgram();
	std::bitset<PipelineType::Max> shaderFlag;

	if (pipelineShader.VertexShader != 0) {
		shaderFlag.set(PipelineType::VertexShader);
		CheckCall(glAttachShader(_program, pipelineShader.VertexShader));
	}
	if (pipelineShader.TessellationShader != 0) {
		shaderFlag.set(PipelineType::TessellationShader);
		CheckCall(glAttachShader(_program, pipelineShader.TessellationShader));
	}
	if (pipelineShader.GeometryShader != 0) {
		shaderFlag.set(PipelineType::GeometryShader);
		CheckCall(glAttachShader(_program, pipelineShader.GeometryShader));
	}
	if (pipelineShader.FragmentShader != 0) {
		shaderFlag.set(PipelineType::FragmentShader);
		CheckCall(glAttachShader(_program, pipelineShader.FragmentShader));
	}

	if (shaderFlag.any()) {
		int success;
		char infoLog[512];
		CheckCall(glLinkProgram(_program));
		CheckCall(glGetProgramiv(_program, GL_LINK_STATUS, &success));
		if (!success) {
			CheckCall(glGetProgramInfoLog(_program, 512, NULL, infoLog));
			LOGE("ERROR::LinkProgram::LINK_FAILED {}", infoLog);
		}
	}

	// 然后删除顶点着色器和片段着色器
	if (shaderFlag.test(PipelineType::VertexShader)) {
		CheckCall(glDeleteShader(pipelineShader.VertexShader));
	}
	if (shaderFlag.test(PipelineType::TessellationShader)) {
		CheckCall(glDeleteShader(pipelineShader.TessellationShader));
	}
	if (shaderFlag.test(PipelineType::GeometryShader)) {
		CheckCall(glDeleteShader(pipelineShader.GeometryShader));
	}
	if (shaderFlag.test(PipelineType::FragmentShader)) {
		CheckCall(glDeleteShader(pipelineShader.FragmentShader));
	}

	LOGD("ShaderProgram create ok!");
	return _program;
}

std::string Shader::readFromFile(const std::string& path)
{
	std::string code;
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(path);
		std::stringstream vCodeStream;
		vCodeStream << file.rdbuf();

		file.close();

		code = vCodeStream.str();
	}
	catch (std::ifstream::failure e) {
		LOGE("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ {}", e.what());
	}

	return code;
}
