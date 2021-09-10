#pragma once

#include "Renderer/Core/Buffer.h"

namespace HEngine {

class GLVertexBuffer : public VertexBuffer {
public:
    GLVertexBuffer(float* vertices, uint32_t size);
    GLVertexBuffer(const std::vector<FVertex>& vertices, uint32_t size);
    virtual ~GLVertexBuffer();

    void Bind() const override;
    void Unbind() const override;

    const BufferLayout& GetLayout() const override { return m_Layout; }
    void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

private:
    uint32_t m_ID;
    BufferLayout m_Layout;
};

class GLIndexBuffer : public IndexBuffer {
public:
    GLIndexBuffer(uint32_t* indices, uint32_t count);
    virtual ~GLIndexBuffer();

    void Bind() const;
    void Unbind() const;

    uint32_t GetCount() const { return m_Count; }

private:
    uint32_t m_ID;
    uint32_t m_Count;
};

}
