#pragma once

#include "glm/glm.hpp"
#include <string>
class Shader {
   public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind();
    void Unbind();

    void SetUniformMat4(const std::string& name, const glm::mat4& value);
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformFloat3(const std::string& name, const glm::vec3& value);
    void SetUniformInt(const std::string& name, int value);

   private:
    unsigned int rendererID;

    int GetUniformLocation(const std::string& name) const;

    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int LinkProgram(unsigned int vertexShader,
                             unsigned int fragmentShader);
};