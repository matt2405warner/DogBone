//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GR_SHADER_H
#define DOGBONE_GR_SHADER_H

#include "GR_API.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <unordered_map>

namespace dogb::GR
{
class GR_API Shader
{
    friend class ShaderLibrary;
public:
    Shader() = default;
    virtual ~Shader() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    [[nodiscard]] const std::string &name() const { return m_name; }

    static std::unique_ptr<Shader> create(const std::string &path);
    static std::unique_ptr<Shader> create(
            const std::string &name,
            const std::string &vert_src,
            const std::string &frag_src);

    virtual void setMat3(const std::string &name, const glm::mat3 &data) const = 0;
    virtual void setMat4(const std::string &name, const glm::mat4 &data) const = 0;

    virtual void setFloat(const std::string &name, float data) const  =0 ;
    virtual void setFloatv(const std::string& name, float* data, uint32_t count) const = 0;
    virtual void setFloat2(const std::string &name, const glm::vec2 & data) const  =0;
    virtual void setFloat3(const std::string &name, const glm::vec3& data) const  =0;
    virtual void setFloat4(const std::string &name, const glm::vec4 &data) const  =0;

    virtual void setInt(const std::string &name, int data) const  =0;
    virtual void setIntv(const std::string &name, int* data, uint32_t count) const = 0;

protected:
    std::string m_name;
};

class GR_API ShaderLibrary
{
public:
    bool add(std::shared_ptr<Shader> shader);
    std::shared_ptr<Shader> load(const std::string &filepath);
    std::shared_ptr<Shader> load(
            const std::string &name,
            const std::string &filepath);

    std::shared_ptr<Shader> get(const std::string &name);

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};

} // namespace dogb::GR

#endif // DOGBONE_GR_SHADER_H
