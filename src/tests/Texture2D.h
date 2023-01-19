#pragma once
#include "Test.h"
#include <memory>
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

namespace test{
class Texture2D :
    public Test
{
public:
  Texture2D();
  ~Texture2D();
public:
  void OnUpdate(float deltaTime);
  void OnRender();
  void OnImGuiRender();

private:
  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<IndexBuffer> m_IndexBuffer;
  std::unique_ptr<VertexBuffer> m_VertexBuffer;
 std::unique_ptr< Shader> m_Shader;
 std::unique_ptr< Texture> m_Texture;

 glm::mat4 m_Proj, m_View;
  glm::vec3 m_TranslationA, m_TranslationB;
};
}
