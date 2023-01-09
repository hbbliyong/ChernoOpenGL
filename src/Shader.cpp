#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath) :
  m_FilePath(filepath), m_RendererId(0)
{
  ShaderProgramSource source = ParseShader(filepath);
  m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
}
void Shader::Bind()const
{
  glUseProgram(m_RendererId);
}
void Shader::UnBind()const
{
  glUseProgram(0);
}
unsigned int Shader::GetUniformLocation(const std::string& name) 
{
  if (m_UniformlocationCache.find(name) != m_UniformlocationCache.end())
    return m_UniformlocationCache[name];

  int location = glGetUniformLocation(m_RendererId, name.c_str());
  if (location == -1) {
    std::cout << "Warning :uniform '" << name << "' doesn't exist!" << std::endl;
  }
  m_UniformlocationCache[name] = location;
  return location;
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) 
{
  int location = GetUniformLocation(name);
  glUniform4f(location, v0, v1, v2, v3);
}
void Shader::SetUniform1i(const std::string& name, unsigned int value)
{
  int location = GetUniformLocation(name);
  glUniform1i(location,value);
}
ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
  std::ifstream stream(filePath);
  enum class ShaderType
  {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  std::string line;
  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;

      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    }
    else
    {
      ss[(int)type] << line << "\n";
    }
  }
  return { ss[0].str(),ss[1].str() };
}
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);
  //todo error hading
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));//C函数，在栈上分配

    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
      << " shader" << std::endl;

    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;

  }
  return id;
}
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

Shader::~Shader()
{
  glDeleteProgram(m_RendererId);
}


