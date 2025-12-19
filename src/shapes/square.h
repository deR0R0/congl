#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Square {
public:
  glm::vec2 position{0.0f, 0.0f};
  glm::vec2 scale{1.0f, 1.0f};

  static void InitMesh();
  static void DestroyMesh();

  void Draw(unsigned int shader_program) const;

  //static unsigned int VAO;
private:
  static unsigned int VAO;
  static unsigned int VBO;
  static unsigned int EBO;
};
