//
// Created by matt on 2020-08-06.
//

#include "GS_Material.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Logger.h>

namespace dogb::GS
{
void
Material::setColor(const std::string &name, const GR::Color &color)
{
    m_values[name] = {Value::kCOLOR, color.toVec4()};
}

void
Material::setInt(const std::string &name, int value)
{
    m_values[name] = {Value::kINT, value};
}

void
Material::setFloat(const std::string &name, float value)
{
    m_values[name] = {Value::kFLOAT, value};
}

void
Material::setMatrix(const std::string &name, const glm::mat4 &matrix)
{
    m_values[name] = {Value::kMATRIX, matrix};
}

void
Material::setTexture(const std::string &name, GR::TexturePtr texture)
{
    if (name == theMainTextName)
    {
        m_mainTexture = texture;
        return;
    }

    for (auto &m_texture : m_textures)
    {
        if (m_texture.first == name)
        {
            m_texture.second = texture;
            return;
        }
    }

    m_textures.emplace_back(name, texture);
}

void
Material::setVector(const std::string &name, const glm::vec4 &vector)
{
    m_values[name] = {Value::kVECTOR, vector};
}

#define MATERIAL_GET_VALUE(_type_, _enum_type_)                                \
    auto it = m_values.find(name);                                             \
    if (it == m_values.end())                                                  \
        return false;                                                          \
                                                                               \
    if (it->second.m_type != _enum_type_)                                      \
    {                                                                          \
        UT_ASSERT_MSG(false, "Value found is not correct type!");              \
        return false;                                                          \
    }                                                                          \
                                                                               \
    if (!std::holds_alternative<_type_>(it->second.m_value))                   \
    {                                                                          \
        UT_ASSERT_MSG(false, "No value found");                                \
        return false;                                                          \
    }                                                                          \
                                                                               \
    value = std::get<_type_>(it->second.m_value);                              \
    return true;

bool
Material::getColor(const std::string &name, GR::Color &value)
{
    MATERIAL_GET_VALUE(glm::vec4, Value::kCOLOR);
}
bool
Material::getInt(const std::string &name, int &value)
{
    MATERIAL_GET_VALUE(int, Value::kINT);
}
bool
Material::getFloat(const std::string &name, float &value)
{
    MATERIAL_GET_VALUE(float, Value::kFLOAT);
}
bool
Material::getMatrix(const std::string &name, glm::mat4 &value)
{
    MATERIAL_GET_VALUE(glm::mat4, Value::kMATRIX);
}
bool
Material::getTexture(const std::string &name, GR::TexturePtr &value)
{
    if (name == theMainTextName)
    {
        value = m_mainTexture;
        return true;
    }

    for (auto &&tex : m_textures)
    {
        if (tex.first == name)
        {
            value = tex.second;
            return true;
        }
    }

    return false;
}
bool
Material::getVector(const std::string &name, glm::vec4 &value)
{
    MATERIAL_GET_VALUE(glm::vec4, Value::kVECTOR);
}

void
Material::uploadValues()
{
    UT_ASSERT(m_shader);

    m_shader->setInt(theMainTextName, 0);

    int texture_idx = 1;
    for (auto &&tex : m_textures)
    {
        if (!tex.second)
            continue;

        m_shader->setInt(tex.first, texture_idx);
        texture_idx++;
    }

    try
    {
        for (auto &&value : m_values)
        {
            if (value.second.m_type == Value::kINT)
            {
                if (!std::holds_alternative<int>(value.second.m_value))
                    continue;

                m_shader->setInt(
                        value.first, std::get<int>(value.second.m_value));
            }
            else if (value.second.m_type == Value::kFLOAT)
            {
                if (!std::holds_alternative<float>(value.second.m_value))
                    continue;

                m_shader->setFloat(
                        value.first,
                        std::get<float>(value.second.m_value));
            }
            else if (value.second.m_type == Value::kVECTOR ||
                     value.second.m_type == Value::kCOLOR)
            {
                if (!std::holds_alternative<glm::vec4>(value.second.m_value))
                    continue;

                m_shader->setFloat4(
                        value.first,
                        std::get<glm::vec4>(value.second.m_value));
            }
            else if (value.second.m_type == Value::kMATRIX)
            {
                if (!std::holds_alternative<glm::mat4>(value.second.m_value))
                    continue;

                m_shader->setMat4(
                        value.first,
                        std::get<glm::mat4>(value.second.m_value));
            }
        }
    }
    catch (const std::bad_variant_access& w)
    {
        UT_LOG_ERROR(w.what());
    }
}
void
Material::bindTextures()
{
    if (m_mainTexture)
        m_mainTexture->bind();

    for (auto &&tex : m_textures)
    {
        if (!tex.second)
            continue;

        tex.second->bind();
    }
}
void
Material::unbindTextures()
{
    if (m_mainTexture)
        m_mainTexture->unbind();

    for (auto &&tex : m_textures)
    {
        if (!tex.second)
            continue;

        tex.second->unbind();
    }
}

} // namespace dogb::GS
