
#include "Core/Shader.h"
#include "glm/fwd.hpp"
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>

std::string LoadShaderSource(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    std::string vertexCode = LoadShaderSource(vertexSrc);
    std::string fragmentCode = LoadShaderSource(fragmentSrc);

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentShader =
        CompileShader(GL_FRAGMENT_SHADER, fragmentCode);
    rendererID = LinkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() { glDeleteProgram(rendererID); }

void Shader::Bind() { glUseProgram(rendererID); }

void Shader::Unbind() { glUseProgram(0); }

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniformFloat(const std::string& name, float value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) glUniform1f(loc, value);
}

void Shader::SetUniformFloat3(const std::string& name, const glm::vec3& value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::SetUniformInt(const std::string& name, int value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) glUniform1i(loc, value);
}

int Shader::GetUniformLocation(const std::string& name) const {
    return glGetUniformLocation(rendererID, name.c_str());
}

unsigned int Shader::CompileShader(unsigned int type,
                                   const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "[Shader] Shader compilation error:\n" << infoLog << "\n";
    }

    return shader;
}

unsigned int Shader::LinkProgram(unsigned int vertexShader,
                                 unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "[Shader] Program link error:\n" << infoLog << "\n";
    }

    return program;
}