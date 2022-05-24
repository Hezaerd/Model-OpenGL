#include "../../include/Ressources/Model.h"
#include "../../include/Core/Debug/Log.h"

#define _CRT_SECURE_NO_WARNINGS

#pragma region USING NAMESPACE
using namespace Core::Debug;
#pragma endregion

Ressource::Model::Model(const char* filename)
{
	only_vertices = false;
    load_obj(filename);

    vbo.init_vbo(this);
    vao.init_vao();
}

bool Ressource::Model::load_obj(const char* filename)
{
	Log log;
	log.OpenFile("LogFile.txt");

	FILE* file = fopen(filename, "r");
	if (!file)
	{
		log.print("Model.cpp | Impossible to open the file !\n");
		DEBUG_LOG("Model.cpp | Impossible to open the file !\n");
		return false;
	}

	std::vector<lm::vec3> temp_positition;
	std::vector<lm::vec3> temp_normal;
	std::vector <lm::vec2> temp_uv;

	while (true)
	{
		char line_header[128];

		if (int const res = fscanf(file, "%s", line_header); res == EOF)
		{
			log.print("Model.cpp | LoadOBJ : Reach end of OBJ file\n");
			break;
		}

		if (strcmp(line_header, "v") == 0)
		{
			lm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			temp_positition.push_back(vertex);
		}
		else if (strcmp(line_header, "#") == 0) 
		{
			
		}
		else if (strcmp(line_header, "vt") == 0) 
		{
			lm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv[0], &uv[1]);
			temp_uv.push_back(uv);
		}
		else if (strcmp(line_header, "vn") == 0)
		{
			lm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normal.push_back(normal);
		}
		else if (strcmp(line_header, "f") == 0)
		{
			long long temp_position_buffer[9]{0};
			

			if(temp_uv.empty() && temp_normal.empty())
			{
				only_vertices = true;
				fscanf_s(file, "%lld %lld %lld\n", 
																&temp_position_buffer[0], 
																&temp_position_buffer[3], 
																&temp_position_buffer[6]);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[0] - 1], 0 , 0);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[3] - 1], 0 , 0);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[6] - 1], 0 , 0);
			}

			if(temp_uv.empty() && only_vertices == false)
			{
				long long temp_normal_buffer[9]{0};
				fscanf_s(file, "%lld//%lld %lld//%lld %lld//%lld\n", 
																&temp_position_buffer[0], &temp_normal_buffer[2], 
																&temp_position_buffer[3], &temp_normal_buffer[5], 
																&temp_position_buffer[6], &temp_normal_buffer[8]);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[0] - 1], 0 , temp_normal[temp_normal_buffer[2] - 1]);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[3] - 1], 0 , temp_normal[temp_normal_buffer[5] - 1]);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[6] - 1], 0 , temp_normal[temp_normal_buffer[8] - 1]);
			}
			if(temp_normal.empty() && only_vertices == false)
			{
				long long temp_uv_buffer[9]{0};
				fscanf_s(file, "%lld/%lld/ %lld/%lld/ %lld/%lld/\n", 
																&temp_position_buffer[0], &temp_uv_buffer[1],
																&temp_position_buffer[3], &temp_uv_buffer[4], 
																&temp_position_buffer[6], &temp_uv_buffer[7]);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[0] - 1], temp_uv[temp_uv_buffer[1] - 1], 0);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[3] - 1], temp_uv[temp_uv_buffer[4] - 1], 0);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[6] - 1], temp_uv[temp_uv_buffer[7] - 1], 0);
			}
			if(!temp_uv.empty() && !temp_normal.empty() && only_vertices == false)
			{
				long long temp_uv_buffer[9]{0};
			long long temp_normal_buffer[9]{0};
				fscanf_s(file, "%lld/%lld/%lld %lld/%lld/%lld %lld/%lld/%lld\n", 
																	&temp_position_buffer[0], &temp_uv_buffer[1], &temp_normal_buffer[2], 
																	&temp_position_buffer[3], &temp_uv_buffer[4], &temp_normal_buffer[5], 
																	&temp_position_buffer[6], &temp_uv_buffer[7], &temp_normal_buffer[8]);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[0] - 1], temp_uv[temp_uv_buffer[1] - 1], temp_normal[temp_normal_buffer[2] - 1]);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[3] - 1], temp_uv[temp_uv_buffer[4] - 1], temp_normal[temp_normal_buffer[5] - 1]);
				vertex_buffer_.emplace_back(temp_positition[temp_position_buffer[6] - 1], temp_uv[temp_uv_buffer[7] - 1], temp_normal[temp_normal_buffer[8] - 1]);
			}

			//std::cout << "Model.cpp | LoadOBJ : vertex_buffer size = " << vertex_buffer_.size() << "\n";
		}
	}
	return true;
}

void Ressource::Model::binding_vao()
{
	vao.bind();
}

void Ressource::Model::draw(Shader* shaders)
{
    shaders->use();
    vao.bind();

    glBindBuffer(GL_ARRAY_BUFFER, vbo.id_vbo);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_buffer_.size()));
}

void Ressource::Model::buffer::init_vbo(Model* model)
{
    glGenBuffers(1, &id_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo);
    glBufferData(GL_ARRAY_BUFFER, model->vertex_buffer_.size() * sizeof(vertex), model->vertex_buffer_.data(), GL_STATIC_DRAW);
}

void Ressource::Model::vertex_attributes::init_vao()
{
    glGenVertexArrays(1, &id_vao_);
    glBindVertexArray(id_vao_);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(offsetof(vertex, position)));
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, texture_uv));
    glEnableVertexAttribArray(1);

    // normal  attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));
    glEnableVertexAttribArray(2);

    //glVertexAttribPointer();

    glBindVertexArray(0);
}

void Ressource::Model::vertex_attributes::bind()
{
    if (id_vao_ == GL_FALSE)
    {
        glGenVertexArrays(1, &id_vao_);
    }
    glBindVertexArray(id_vao_);
}


Ressource::Model::buffer::~buffer()
{
    glDeleteBuffers(1, &id_vbo);
}

Ressource::Model::vertex_attributes::~vertex_attributes()
{
    glDeleteVertexArrays(1, &id_vao_);
}