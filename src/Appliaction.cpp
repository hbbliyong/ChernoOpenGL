#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/matrix.hpp"
#include "Vendor/glm/ext/matrix_clip_space.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int main();
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  /**
  * �������������������֮ǰ�ȴ���֡����ͨ����Ϊv-sync
  * Ĭ������£��������Ϊ0
  * ��������Ϊ1����ÿ֡����һ��
  **/
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }


  // build and compile our shader program
  // ------------------------------------
  // vertex shader
   // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.0f,
       0.5f, -0.5f, 1.0f, 0.0f,
       0.5f,  0.5f, 1.0f,  1.0f,
      -0.5f,  0.5f, 0.0f,1.0f,
  };
  unsigned int indeices[] = { 0,1,2,2,3,0 };

  /**
    * ���:
    * �������ɫ(�ж���ɫ���������ɫ)��Ŀ�껺�������е���ɫ���
    * glEnable/glDisable(����&�ر�) => glBlendFunc(ָ����ɫ����) => glBlendEquation(ָ�����ģʽ)
    * glBlendEquation(mode) mode: src��dest�Ļ�Ϸ�ʽ(Ĭ��GL_FUNC_ADD, ����)
    *
    **/
    /* ���û��(Ĭ�ϲ�������) */
  GLCall(glEnable(GL_BLEND));
  /**
   * glBlendFunc(src, dest) ָ����ɫ����
   * src ָ�������ɫ(RGBA)���ӵļ��㷽ʽ, Ĭ��ΪGL_ONE
   * dest ָ��Ŀ����ɫ���ӵļ��㷽ʽ, Ĭ��ΪGL_ZERO
   * GL_SRC_ALPHA ��Ϊsrc��alphaΪ0, GL_ONE_MINUS_SRC_ALPHA 1-src.alpha
   * RGBA = Srgba * GL_SRC_ALPHA + Drgba * GL_ONE_MINUS_SRC_ALPHA
   **/
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui::StyleColorsDark();

  const char* glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

  VertexArray va;
  VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indeices, 6);
  glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0, 0));
  glm::mat4 result = view * proj;
  Shader shader("res/shaders/Basic.shader");
  shader.Bind();
  shader.SetUniform4f("u_Color", .8f, .3f, .8f, 1.0f);
  shader.SetUniformMat4f("u_MVP", result);
  Texture texture("res/textures/ChernoLogo.png");
  texture.Bind();
  shader.SetUniform1i("u_Texture", 0);

  ib.Bind();
  vb.Bind();
  va.Bind();

  va.Unbind();
  vb.Unbind();
  ib.Unbind();
  shader.UnBind();

  Renderer renderer;
  
  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  float r = 0;
  float increment = 0.05f;
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    // render
    // ------
    renderer.Clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test");
    ImGui::Text("Hello, world %d", 123);
    float f = 0.25;
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::End();
    va.Bind();
    ib.Bind();
    // draw our first triangle
    shader.Bind();
    shader.SetUniform4f("u_Color", r, .3f, .8f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(f, 0, 0));
    result = view * proj;
    shader.SetUniformMat4f("u_MVP", result);
    renderer.Draw(va, ib, shader);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0));
    result = view * proj;
    shader.SetUniformMat4f("u_MVP", result);
    renderer.Draw(va, ib, shader);
    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.0f)
      increment = 0.05f;
    r += increment;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}