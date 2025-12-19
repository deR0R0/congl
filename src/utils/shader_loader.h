#pragma once
#include <string>

using namespace std;

string load_shader(std::string f);
void load_shaders();

string get_vertex_shader();
string get_fragment_shader();
