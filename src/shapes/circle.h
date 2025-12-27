#pragma once

#include "square.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Circle {

public:
  glm::vec2 position { 0.0f };
  glm::vec2 velocity { 1.0f, 0.5f };
  float radius = 1.0f;

  void Draw(unsigned int shader_program) const;

  static void InitMesh(int segments = 64);
  static void DestroyMesh();
  
  void Move(float delta_time);
  float Distance(Square paddle);

private:
  static unsigned int VAO;
  static unsigned int VBO;
  static int vertex_count;

  void DetectWallCollision();
};
