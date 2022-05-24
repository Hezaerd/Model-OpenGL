#include "../../include/Ressources/Shader.h"

#pragma region USING NAMESPACE
using namespace Core::Debug;
#pragma endregion

Ressource::Shader::Shader(const char* vertex_file, const char* fragment_file)
{
	unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_file);
	unsigned int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_file);

    link_shader(vertex_shader, fragment_shader);
}

Ressource::Shader::~Shader()
{
	glDeleteProgram(id_);
}

std::string Ressource::Shader::read_shader(const char* filename) const
{
    // Open logFile
	// ------------
	Log log;
	log.OpenFile("LogFile.txt");

	std::string src;

    std::ifstream file_shader(filename);

    if (file_shader.is_open())
    {
	    std::string temp;

	    while (std::getline(file_shader, temp))
        {
            src += temp + "\n";
        }
    }
    else
    {
        log.print("Shader | Could not open shader file");
    }

    file_shader.close();

    return src;
}

unsigned int Ressource::Shader::compile_shader(const GLenum type, const char* filename) const
{
	// Open logFile
	// ------------
	Log log;
	log.OpenFile("LogFile.txt");

	int succes;

	unsigned int shader_program = glCreateShader(type);

	std::string temp_source = read_shader(filename);
    const char* src = temp_source.c_str();

    glShaderSource(shader_program, 1, &src, nullptr);
    glCompileShader(shader_program);

    glGetShaderiv(shader_program, GL_COMPILE_STATUS, &succes);
    if (!succes)
    {
	    char gl_log[1024];

	    glGetShaderInfoLog(shader_program, 1024, nullptr, gl_log);
        log.print("Shader | could not compile shader");
        DEBUG_LOG("Shader | could not compile shader");
        std::cout << gl_log << '\n';
    }

    return shader_program;
}

void Ressource::Shader::link_shader(unsigned int const vertex_shader, unsigned int const fragment_shader)
{
	// Open logFile
	// ------------
	Log log;
	log.OpenFile("LogFile.txt");

	int succes;

    id_ = glCreateProgram();

    glAttachShader(id_, vertex_shader);
    glAttachShader(id_, fragment_shader);

    glLinkProgram(id_);

    glGetProgramiv(id_, GL_LINK_STATUS, &succes);
    if (!succes)
    {
	    char gl_log[1024];

	    log.print("Shader | can't link shaders\n");
	    DEBUG_LOG("Shader | can't link shaders\n");

        std::cout << gl_log << '\n';
    }

    glUseProgram(0);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Ressource::Shader::set_bool(const std::string& name, bool const value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

void Ressource::Shader::set_int(const std::string& name, int const value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Ressource::Shader::set_float(const std::string& name, float const value) const
{
	glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Ressource::Shader::set_uniform_m4f(const char* name, lm::Mat4<float>matrix)
{
    use();
    glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, &matrix[0][0]);
    glUseProgram(0);
}

void Ressource::Shader::use()
{
    glUseProgram(id_);
}
