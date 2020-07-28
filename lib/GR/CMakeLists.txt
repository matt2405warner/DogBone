project(GR)

set(sources
        GR_Renderer.cpp
        GR_RenderBuffer.cpp
        GR_Shader.cpp
        GR_VertexArray.cpp
        GR_DesktopWindow.cpp
        GR_Texture.cpp
        GR_Renderer2D.cpp
        GR_SubSystem.cpp
        GR_SubTexture.cpp
        GR_Framebuffer.cpp
        GR_Camera.cpp

        GR_OpenGLShader.cpp
        GR_OpenGLVertexArray.cpp
        GR_OpenGLRenderer.cpp
        GR_OpenGLRenderBuffer.cpp
        GR_OpenGLTexture.cpp
        GR_OpenGLFramebuffer.cpp
        )

set(headers
        GR_Renderer.h
        GR_RenderBuffer.h
        GR_Shader.h
        GR_VertexArray.h
        GR_DesktopWindow.h
        GR_Texture.h
        GR_Renderer2D.h
        GR_SubSystem.h
        GR_SubTexture.h
        GR_Framebuffer.h
        GR_Camera.h

        GR_OpenGLShader.h
        GR_OpenGLVertexArray.h
        GR_OpenGLRenderer.h
        GR_OpenGLRenderBuffer.h
        GR_OpenGLTexture.h
        GR_OpenGLFramebuffer.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC UT CE CONAN_PKG::glm CONAN_PKG::stb)