#pragma once
#include <glad/glad.h>
#include <fstream>
#include "RessourcesManager.h"
#include "../../include/Core/Debug/Assertion.h"
#include "../../include/Core/Debug/Log.h"
#include "../../include/LowRenderer/Camera.h"
#include "Mat4/Mat4.h"

#pragma region USING NAMESPACE
using namespace LowRenderer;
#pragma endregion

namespace Ressource
{
    class Shader : public IRessource
    {
    private:
        int id_;

        // Shader reading/loading
        std::string read_shader(const char* filename) const;
        unsigned int compile_shader(GLenum type, const char* filename) const;

        // Shader linking
        void link_shader(unsigned int vertex_shader, unsigned int fragment_shader);


    public:

        Shader(const char* vertex_file, const  char* fragment_file);
        ~Shader();

        void use();

        // Set
        void set_bool(const std::string& name, bool value) const;
        void set_int(const std::string& name, int value) const;
        void set_float(const std::string& name, float value) const;
        void set_uniform_m4f(const char* name, lm::Mat4<float>matrix);
        void set_uniform_m4v(const char* name, lm::Mat4<float>matrix);
    };
}
