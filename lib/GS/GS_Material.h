//
// Created by matt on 2020-08-06.
//

#ifndef DOGBONE_GS_MATERIAL_H
#define DOGBONE_GS_MATERIAL_H

#include "GS_API.h"

#include <GR/GR_Color.h>
#include <GR/GR_Shader.h>
#include <GR/GR_Texture.h>

#include <CE/CE_Asset.h>

#include <variant>
#include <string>
#include <unordered_map>

namespace dogb::GS
{
class DB_GS_API Material : public CE::Asset
{
public:
    static constexpr const char* theMainTextName = "u_MainTex";

    void setColor(const std::string &name, const GR::Color &color);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setMatrix(const std::string &name, const glm::mat4 &matrix);
    void setTexture(const std::string &name, GR::TexturePtr texture);
    void setVector(const std::string &name, const glm::vec4 &vector);

    bool getColor(const std::string &name, GR::Color &color);
    bool getInt(const std::string &name, int &value);
    bool getFloat(const std::string &name, float &value);
    bool getMatrix(const std::string &name, glm::mat4 &matrix);
    bool getTexture(const std::string &name, GR::TexturePtr &texture);
    bool getVector(const std::string &name, glm::vec4 &vector);

    // Send the material values to the shader.
    void uploadValues();
    // Bind all of our textures.
    void bindTextures();
    // Unbind all of our textures.
    void unbindTextures();

    GR::TexturePtr m_mainTexture;
    GR::ShaderPtr m_shader;

private:
    struct Value
    {
        using Holder = std::variant<int, float, glm::mat4, glm::vec4>;

        enum Type
        {
            kCOLOR,
            kINT,
            kFLOAT,
            kMATRIX,
            kVECTOR
        } m_type;

        Holder m_value;
    };
    using TextureValue = std::pair<std::string, GR::TexturePtr >;

    std::unordered_map<std::string, Value> m_values;
    std::vector<TextureValue> m_textures;
};

using MaterialPtr = std::shared_ptr<GS::Material>;

} // namespace dogb::GS

#endif // DOGBONE_GS_MATERIAL_H
