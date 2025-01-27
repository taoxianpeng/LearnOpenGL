#include "Shader.h"
#include "log.h"

#include <fstream>
#include <iostream>
#include <sstream>

// #include "ShaderSource.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		LOGE("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ {}", e.what());
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	CheckCall(glShaderSource(vertexShader, 1, &vShaderCode, NULL));
	CheckCall(glCompileShader(vertexShader));
	CheckCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
	if (!success) {
		// 获取编译信息，存储到字符数组中，然后打印出来
		CheckCall(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
		LOGE("ERROR::SHADER::VERTEX::COMPLIATION_FAILED {}", infoLog);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	CheckCall(glShaderSource(fragmentShader, 1, &fShaderCode, NULL));
	CheckCall(glCompileShader(fragmentShader));
	CheckCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
	if (!success) {
		CheckCall(glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
		LOGE("ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED {}", infoLog);
	}

	shaderProgram = glCreateProgram();
	CheckCall(glAttachShader(shaderProgram, vertexShader));
	CheckCall(glAttachShader(shaderProgram, fragmentShader));
	CheckCall(glLinkProgram(shaderProgram));

	CheckCall(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
	if (!success) {
		CheckCall(glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog));
		LOGE("ERROR::LinkProgram::LINK_FAILED {}", infoLog);
	}

	// 然后删除顶点着色器和片段着色器
	CheckCall(glDeleteShader(vertexShader));
	CheckCall(glDeleteShader(fragmentShader));
}

void Shader::use() { 
	CheckCall(glUseProgram(shaderProgram)); 
	LOGD("Shader use ID: {}", shaderProgram);
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

Shader::~Shader() { CheckCall(glDeleteProgram(shaderProgram)); }
