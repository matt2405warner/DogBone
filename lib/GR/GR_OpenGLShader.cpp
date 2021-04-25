//
// Created by matt on 2020-07-12.
//

#include "GR_OpenGLShader.h"

#include <CE/CE_GLFWHelpers.h>

#include <UT/UT_Logger.h>

#include <UT/UT_Profiler.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <vector>

namespace dogb::GR::OpenGL
{
static GLenum
grShaderTypeFromString(const std::string &type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    else if (type == "fragement")
        return GL_FRAGMENT_SHADER;
    UT_LOG_WARN("Unknown shader type!");
    return 0;
}

GLShader::GLShader(const std::string &filepath)
{
    std::string source = readFile(filepath);
    if (source.length() <= 0)
        return;

    compile(preProcess(source));

    // Extract name from file path
    auto last_slash = filepath.find_last_of("/\\");
    last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
    auto last_dot = filepath.rfind('.');
    auto count = last_dot == std::string::npos ? filepath.size() - last_slash :
                                                 last_dot - last_slash;
    m_name = filepath.substr(last_slash, count);
}

GLShader::GLShader(
        const std::string &name,
        const std::string &vertex_src,
        const std::string &frag_src)
{
    m_name = name;

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertex_src;
    sources[GL_FRAGMENT_SHADER] = frag_src;

    compile(sources);
}

std::string
GLShader::readFile(const std::string &filepath)
{
    std::ifstream in(filepath, std::ios::in | std::ios::binary);

    if (!in)
    {
        UT_LOG_ERROR("Unable to open shader file '{}'", filepath);
        return std::string{};
    }

    in.seekg(0, std::ios::end);
    std::string result;
    result.resize(static_cast<unsigned long>(in.tellg()));
    in.seekg(0, std::ios::beg);

    in.read(&result[0], static_cast<long>(result.size()));

    return result;
}

std::unordered_map<GLenum, std::string>
GLShader::preProcess(const std::string &source)
{
    UT_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> sources;

    const char *type_token = "#type";
    static constexpr size_t type_token_length = 5;

    size_t pos = source.find(type_token, 0);

    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        size_t begin = pos + type_token_length + 1;

        std::string type = source.substr(begin, eol - begin);
        size_t next_line_pos = source.find_first_not_of("\r\n", eol);
        pos = source.find(type_token, next_line_pos);
        sources[grShaderTypeFromString(type)] =
                (pos == std::string::npos) ?
                        source.substr(next_line_pos) :
                        source.substr(next_line_pos, pos - next_line_pos);
    }

    return sources;
}

void
GLShader::compile(const std::unordered_map<GLenum, std::string> &sources)
{
    UT_PROFILE_FUNCTION();

    // Vertex and frag shaders are successfully compiled.
    // Now we link them together into a program.
    m_ID = glCreateProgram();
    std::vector<GLuint> shader_ids{};
    for (auto &&[type, source] : sources)
    {
        // Create an empty vertex shader handle
        GLuint shader = glCreateShader(type);

        // Send the vertex shader source code to GL
        const auto *c_source = source.c_str();
        glShaderSource(shader, 1, &c_source, nullptr);

        // Compile the vertex shader
        glCompileShader(shader);

        GLint is_compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE)
        {
            GLint max_length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

            // max length includes NULL characater
            std::vector<GLchar> info_log(
                    static_cast<std::vector<GLchar>::size_type>(max_length));
            glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

            // we dont need the shader anymore
            glDeleteShader(shader);

            UT_LOG_ERROR("Shader compilation failure!");
            UT_LOG_ERROR("{}", info_log.data());
            return;
        }

        // Attach our shaders to our program
        glAttachShader(m_ID, shader);

        shader_ids.emplace_back(shader);
    }

    // Link our program
    glLinkProgram(m_ID);

    GLint is_linked = 0;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &max_length);

        // max length include NULL character
        std::vector<GLchar> info_log(
                static_cast<std::vector<GLchar>::size_type>(max_length));
        glGetProgramInfoLog(m_ID, max_length, &max_length, &info_log[0]);

        // we don't need the program anymore
        glDeleteProgram(m_ID);
        // Don't leak shaders either
        for (auto &&shader : shader_ids)
            glDetachShader(m_ID, shader);

        UT_LOG_ERROR("Program link failure!");
        UT_LOG_ERROR("{0}", info_log.data());
        return;
    }

    // Always detach shaders after a successful link.
    for (auto &&shader : shader_ids)
        glDetachShader(m_ID, shader);
}

GLShader::~GLShader()
{
    glDeleteProgram(m_ID);
}

void
GLShader::bind() const
{
    glUseProgram(m_ID);
}

void
GLShader::unbind() const
{
    glUseProgram(0);
}

void
GLShader::setMat3(const std::string &name, const glm::mat3 &data)
        const
{
    GLint location = getUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void
GLShader::setMat4(const std::string &name, const glm::mat4 &data)
        const
{
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void
GLShader::setFloatv(const std::string &name, float *data, uint32_t count) const
{
    GLint location = getUniformLocation(name);
    glUniform1fv(location, count, data);
}

void
GLShader::setFloat(const std::string &name, float data) const
{
    GLint location = getUniformLocation(name);
    glUniform1f(location, data);
}

void
GLShader::setFloat2(const std::string &name, const glm::vec2 &data) const
{
    GLint location = getUniformLocation(name);
    glUniform2f(location, data.x, data.y);
}

void
GLShader::setFloat3(const std::string &name, const glm::vec3 &data) const
{
    GLint location = getUniformLocation(name);
    glUniform3f(location, data.x, data.y, data.z);
}

void
GLShader::setFloat4(const std::string &name, const glm::vec4 &data) const
{
    GLint location = getUniformLocation(name);
    glUniform4f(location, data.x, data.y, data.z, data.w);
}

void
GLShader::setIntv(const std::string &name, int *data, uint32_t count) const
{
    GLint location = getUniformLocation(name);
    glUniform1iv(location, count, data);
}

void
GLShader::setInt(const std::string &name, int data) const
{
    GLint location = getUniformLocation(name);
    glUniform1i(location, data);
}
GLint
GLShader::getUniformLocation(const std::string &name) const
{
    if (auto it = m_UniformLocationCache.find(name); it != m_UniformLocationCache.end())
        return it->second;
    GLint location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
    {
        UT_LOG_WARN("Uniform '{0}' not found!", name);
    }
    m_UniformLocationCache[name] = location;
    return location;
}

} // namespace dogb::GR::OpenGL
