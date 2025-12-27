#include "circle.h"
#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>

unsigned int Circle::VAO = 0;
unsigned int Circle::VBO = 0;
int Circle::vertex_count = 0;

void Circle::InitMesh(int segments) {
  if(VAO != 0) return;

  std::vector<float> vertices;
  
  vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f });

  for(int i = 0; i <= segments; ++i) {
    float angle = 2.0f * 3.1415926f * i / segments;
    vertices.push_back(std::cos(angle));
    vertices.push_back(std::sin(angle));
    vertices.push_back(0.0f);
  }

  vertex_count = segments + 2;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

void Circle::DestroyMesh() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  VAO = VBO = 0;
}

void Circle::Draw(unsigned int shader_program) const {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));
  model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

  int model_loc = glGetUniformLocation(shader_program, "model");
  glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_count);
  glBindVertexArray(0);
}

void Circle::DetectWallCollision() {
  if(position.x - radius <= -1.0f || position.x + radius >= 1.0f) {
    velocity.x *= -1.0f;
    // snap position to the side
    // because there was a bug where the
    // ball could somehow escape
    if(velocity.x > 0.0f) {
      position.x = radius + -1.0f;
    } else {
      position.x = 1.0f - radius;
    }
  }

  if(position.y - radius <= -1.0f || position.y + radius >= 1.0f) {
    velocity.y *= -1.0f;
    // also snap position
    if(velocity.y > 0.0f) {
      position.y = radius + -1.0f;
    } else {
      position.y = 1.0f - radius;
    }
  }
}

void Circle::Move(float delta_time) {
  position.x += velocity.x * delta_time;
  position.y += velocity.y * delta_time;

  DetectWallCollision();
}

float Circle::Distance(Square paddle) {
  float x_abs_diff = std::abs(position.x - paddle.position.x);
  float y_abs_diff = std::abs(position.y - paddle.position.y);

  return std::sqrt(std::pow(x_abs_diff, 2) + std::pow(y_abs_diff, 2));
}
