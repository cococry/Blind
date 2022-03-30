#include "blindpch.h"
#include "MeshLoader.h"
#include <glad/glad.h>

namespace Blind
{
    Ref<Mesh> MeshLoader::LoadToVAO(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, float dimensions)
    {
        int32_t vaoID = CreateVAO();
        SetEBO(indices);
        SetLayoutAttribute(0, vertices);
        UnbindVAO();
        return CreateRef<Mesh>(vaoID, indices.size());
    }

    void MeshLoader::DeleteData()
    {
        for (auto& vao : m_VAOs)
        {
            glDeleteVertexArrays(1, &vao);
        }
        for (auto& vbo : m_VBOs)
        {
            glDeleteBuffers(1, &vbo);
        }
    }

    void MeshLoader::SetLayoutAttribute(int32_t attribute, const std::vector<float>& data, float dimensions)
    {
        uint32_t vbo;
        glGenBuffers(1, &vbo);
        m_VBOs.push_back(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data) * sizeof(float), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(attribute, dimensions, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void MeshLoader::UnbindVAO()
    {
        glBindVertexArray(0);
    }

    RendererID MeshLoader::CreateVAO()
    {
        RendererID vao;
        glGenVertexArrays(1, &vao);
        m_VAOs.push_back(vao);
        glBindVertexArray(vao);
        return vao;
    }

    void MeshLoader::SetEBO(const std::vector<uint32_t>& indices)
    {
        uint32_t ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    }

}
