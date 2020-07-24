//
// Created by matt on 2020-07-12.
//

#ifndef DOGBONE_GR_OPENGLSHADER_H
#define DOGBONE_GR_OPENGLSHADER_H

#include "GR_API.h"

#include "GR_Shader.h"

#include <string>
#include <unordered_map>

#include <glad/glad.h>

namespace dogb::GR::OpenGL
{
class GR_API GLShader : public GR::Shader
{
public:
    explicit GLShader(const std::string& filepath);
    GLShader(const std::string& name, const std::string& vert_src, const std::string& frag_src);
    ~GLShader() override;

    void bind() const override;
    void unbind() const override;

    void setMat3(const std::string &name, const glm::mat3 &data) const override;
    void setMat4(const std::string &name, const glm::mat4 &data) const override;

    void setFloatv(const std::string& name, float* data, uint32_t count) const override ;
    void setFloat(const std::string &name, float data) const override;
    void setFloat2(const std::string &name, const glm::vec2 & data) const override ;
    void setFloat3(const std::string &name, const glm::vec3& data) const override ;
    void setFloat4(const std::string &name, const glm::vec4 &data) const override ;

    void setInt(const std::string &name, int data) const override ;
    void setIntv(const std::string &name, int* data, uint32_t count) const override ;

private:
    static std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
    static std::string readFile(const std::string& filepath);
    void compile(const std::unordered_map<GLenum, std::string>& sources);

    uint32_t m_ID{};
};
}

#endif // DOGBONE_GR_OPENGLSHADER_H
