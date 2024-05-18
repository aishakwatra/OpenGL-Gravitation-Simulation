const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 a_pos;\n"
"void main() {\n"
"  gl_Position = vec4(a_pos, 1.0);\n"
"}\n";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 frag_color;\n"
"uniform vec4 c_pos;\n"
"void main() {\n"
"  frag_color = c_pos;\n"
"}\n";