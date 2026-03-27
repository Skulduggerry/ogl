#ifndef OGL_VERTEXARRAY_HPP
#define OGL_VERTEXARRAY_HPP
#include "buffer/IndexBuffer.hpp"
#include "buffer/VertexBuffer.hpp"
#include "buffer/VertexBufferLayout.hpp"

#include <glad/glad.h>

class VertexArray
{
  GLuint m_id;

public:
  VertexArray();
  explicit VertexArray(NoCreate_t);
  ~VertexArray();
  VertexArray(const VertexArray &other) = delete;
  VertexArray(VertexArray &&other) noexcept;

  VertexArray &operator=(const VertexArray &other) = delete;
  VertexArray &operator=(VertexArray &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }

  [[nodiscard]] bool isValid() const noexcept { return m_id != 0; }

  void bind() const;
  static void unbind();

  template<typename T>
  void vertexBuffer(const GLuint bindingIndex,
    const VertexBuffer<T> &buffer,
    const VertexBufferLayout &layout,
    const GLintptr elementOffset) const
  {
    const GLintptr byteOffset = elementOffset * static_cast<GLintptr>(sizeof(T));

    GLCall(glVertexArrayVertexBuffer(m_id, bindingIndex, buffer.getId(), byteOffset, layout.stride()));

    for (const auto &[location, components, type, kind, normalized, relativeOffset] : layout.attribs()) {
      GLCall(glEnableVertexArrayAttrib(m_id, location));

      if (kind == AttributeKind::FLOAT) {
        GLCall(
          glVertexArrayAttribFormat(m_id, location, components, static_cast<GLenum>(type), normalized, relativeOffset));
      } else if (kind == AttributeKind::INT) {
        GLCall(glVertexArrayAttribIFormat(m_id, location, components, static_cast<GLenum>(type), relativeOffset));
      } else {
        GLCall(glVertexArrayAttribLFormat(m_id, location, components, static_cast<GLenum>(type), relativeOffset));
      }

      GLCall(glVertexArrayAttribBinding(m_id, location, bindingIndex));
    }
  }

  template<typename T> void indexBuffer(const IndexBuffer<T> &buffer)
  { GLCall(glVertexArrayElementBuffer(m_id, buffer.getId())); }
};

#endif// OGL_VERTEXARRAY_HPP
