//
// Created by matt on 2020-07-11.
//

#include "GR_Shader.h"

#include <utility>

#include "GR_OpenGLShader.h"

namespace dogb::GR
{
std::unique_ptr<Shader>
Shader::create(
        const std::string &name,
        const std::string &vert_src,
        const std::string &frag_src)
{
    return std::make_unique<OpenGL::GLShader>(name, vert_src, frag_src);
}

std::unique_ptr<Shader>
Shader::create(const std::string &filepath)
{
    return std::make_unique<OpenGL::GLShader>(filepath);
}

std::shared_ptr<Shader>
ShaderLibrary::get(const std::string &name)
{
    auto it = m_shaders.find(name);
    if (it == m_shaders.end())
        return nullptr;

    return it->second;
}

bool
ShaderLibrary::add(std::shared_ptr<Shader> shader)
{
    const std::string &name = shader->name();
    if (auto it = m_shaders.find(name); it != m_shaders.end())
        return false;

    m_shaders[name] = std::move(shader);
    return true;
}

std::shared_ptr<Shader>
ShaderLibrary::load(const std::string &filepath)
{
    std::shared_ptr<Shader> shader = Shader::create(filepath);
    if (!shader)
        return nullptr;
    m_shaders[shader->name()] = shader;
    return shader;
}
std::shared_ptr<Shader>
ShaderLibrary::load(const std::string &name, const std::string &filepath)
{
    std::shared_ptr<Shader> shader = Shader::create(filepath);
    if (!shader)
        return nullptr;

    shader->m_name = name;
    m_shaders[name] = shader;
    return shader;
}
} // namespace dogb::GR