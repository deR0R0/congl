#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>
#include "utils/shader_loader.h"
#include "shapes/square.h"

using namespace std;




//
// WINDOW SETUP FUNCTIONS
//
void set_window_configuration() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

GLFWwindow* create_window(int width, int height, string title) {
  GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if(!window) {
    cerr << "Failed to create a window" << endl;
    glfwTerminate();
    return nullptr;
  }

  return window;
}

void handle_paddle_movement(GLFWwindow* window, float delta_time, Square* paddle, int up_key, int down_key) {
  if(glfwGetKey(window, up_key) == GLFW_PRESS) {
    paddle -> position.y += 2 * delta_time;
  }

  if(glfwGetKey(window, down_key) == GLFW_PRESS) {
    paddle -> position.y -= 2 * delta_time;
  }
}




//
// MAIN LOOP
//

int main()
{
  // load our shaders
  load_shaders();

  // get our shaders
  string vertex_shader_string = get_vertex_shader();
  string fragment_shader_string = get_fragment_shader();
  
  // check for glfw
  if (!glfwInit()) {
      std::cerr << "Failed to initialize GLFW\n";
      return -1;
  }

  // do a bunch of stupid config i dunno make window and stuff
  set_window_configuration();
    
  GLFWwindow* window = create_window(800, 600, "CongL++");
  
  glfwMakeContextCurrent(window);

  // check for glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD\n";
      return -1;
  }

  // compile the vertex shader
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* vertex_shader_source = vertex_shader_string.c_str();
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    cerr << "Failed to compile vertex shader. Log: " << infoLog << endl;
  }

  // compile fragmentation shader
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fragment_shader_source = fragment_shader_string.c_str();
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
    cerr << "Failed to compile fragment shader. Log: " << infoLog << endl;
  }

  // link shaders
  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    cerr << "Failed to link shader program. Log: " << infoLog << endl;
  }

  // delete shaders after linking
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);


  // colorerer
  int colorer = glGetUniformLocation(shader_program, "uColor");
  glUniform3f(colorer, 1.0f, 0.0f, 0.0f);

  // init the mesh stuff
  Square::InitMesh();

  // create our paddles
  Square paddle1;
  paddle1.position = { -0.9f, 0.0f };
  paddle1.scale = { 0.1f, 0.5f };

  Square paddle2;
  paddle2.position = { 0.9f, 0.0f };
  paddle2.scale = { 0.1f, 0.5f };

  float last = glfwGetTime();

  // loop that continously updates our content
  while (!glfwWindowShouldClose(window)) {
      // calculate our delta time
      float current = glfwGetTime();
      float delta_time = current - last;
      last = current;

      // clear the screen
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shader_program);

      // projection location shit
      int project_loc = glGetUniformLocation(shader_program, "projection");
      glm::mat4 projection = glm::mat4(1.0f);
      glUniformMatrix4fv(project_loc, 1, GL_FALSE, &projection[0][0]);

      // handle inputs
      handle_paddle_movement(window, delta_time, &paddle1, GLFW_KEY_W, GLFW_KEY_S);
      handle_paddle_movement(window, delta_time, &paddle2, GLFW_KEY_UP, GLFW_KEY_DOWN);

      // draw our stuff
      glUniform3f(colorer, 1.0f, 0.0f, 0.0f);
      paddle1.Draw(shader_program);

      glUniform3f(colorer, 0.0f, 1.0f, 0.0f);
      paddle2.Draw(shader_program);

      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  Square::DestroyMesh();

  glfwTerminate();
  return 0;
}
