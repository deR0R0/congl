#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>

using namespace std;

string vertex_shader;
string fragment_shader;

string load_shader(string f) {
  filesystem::path p = "./src/shaders/" + f;
  filesystem::path abs_path = filesystem::absolute(p);
  ifstream file(abs_path);

  if(!file.is_open()) {
    cerr << "Failed to open the shader file: " << abs_path << endl;
    cout << "Current Working Directory: " << filesystem::current_path() << endl;
    return "";
  }

  stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void load_shaders() {
  vertex_shader = load_shader("vertex.glsl");
  fragment_shader = load_shader("fragment.glsl");

  if(vertex_shader == "" || fragment_shader == "") {
    cerr << "Failed to load shader files!";
  } else {
    cout << "Successfully loaded shader files and data";
  }
}

string get_vertex_shader() {
  return vertex_shader;
}

string get_fragment_shader() {
  return fragment_shader;
}
