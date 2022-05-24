#include "../../include/Ressources/Application.h"
#include "Mat4/Mat4.h"

#pragma region USING NAMESPACE
using namespace Core::Debug;
using namespace Ressource;
using namespace LowRenderer;
#pragma endregion

int application::init_window(const char* window_name, const int window_width, const int window_height)
{
	// Create logFile
	// --------------
	Log log;
	log.OpenFile("LogFile.txt");
	log.print("Application | Opening LogFile.txt\n");

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_name, nullptr, nullptr);
	if (window == nullptr)
	{
		log.print("Application | Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	window_ = window;
	
	return 1;
}

int application::init_glad()
{
	// Open logFile
	// ------------
	Log log;
	log.OpenFile("LogFile.txt");

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		log.print("Application | Failed to initialize GLAD");
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//glEnable(GL_DEPTH_TEST);


	GLint flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(application::gl_debug_output, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	return 1;
}

application::application(char const* window_name, const int window_width, const int window_height)
: window_(nullptr), scr_width_(window_width), scr_height_(window_height)
{
	this->init_window(window_name, window_width, window_height);
	this->init_glad();

	delta_time_ = last_time_ = current_time_ = 0;
}

void application::delta_time()
{
	// DeltaTime
	// ---------
	current_time_ = static_cast<float>(glfwGetTime());
	delta_time_ = current_time_ - last_time_;
	last_time_ = current_time_;
}

void application::run()
{
	// Open logFile
	// ------------
	Log log;
	log.OpenFile("LogFile.txt");

	// Camera
	// ------
	Camera camera(70.0f, scr_width_, scr_height_, 0.1f, 1000.0f);
	
	// Ressource Manager
	// -----------------
	ressource_manager ressource_manager;
	glfwSetTime(0);
	Model* model1 = ressource_manager.create_ressource<Model>("assets/meshes/diablo3_pose/diablo3_pose.obj");
	Model* model2 = ressource_manager.create_ressource<Model>("assets/meshes/armadillo.obj");
	Model* model3 = ressource_manager.create_ressource<Model>("assets/meshes/floor.obj");
	//Model* model3 = ressource_manager.create_ressource<Model>("assets/meshes/nol_gnom2.obj");
	std::cout << "Model loaded in : " << glfwGetTime() << "\n";
	Shader* shader = ressource_manager.create_ressource<Shader>("source/shaders/vertexShaderSource.glsl", "source/shaders/fragmentShaderSource.glsl");

	// Mesh
	// ----
	Mesh mesh1(model1, vec3(90.f,0.f,0.f), vec3(-2.f,2.f,0.f), vec3(2.f,2.f,2.f));
	meshes_.emplace_back(mesh1);
	Mesh mesh2(model2, vec3(0.f,0.f,0.f), vec3(2.f,-0.15f,0.f), vec3(.7f,.7f,.7f));
	meshes_.emplace_back(mesh2);
	Mesh mesh3(model3, vec3(0.f,0.f,0.f), vec3(0.f,0.f,0.f), vec3(10.f,1.f,10.f));
	meshes_.emplace_back(mesh3);

	// configure global opengl state
	// ------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

	Mat4<float> temp1;
	temp1.identity;
	Mat4<float> temp2;
	temp2.identity;
	// MAIN LOOP
	while (!glfwWindowShouldClose(window_))
	{
		// time logic
		// ----------
		delta_time();

		// Update Camera
		mat4 temp_view;
		camera.view_matrix = temp_view.look_at(camera.position,camera.position + camera.front, camera.up);
		
		// input
		// -----
		glfwPollEvents();
		process_input(window_, camera, delta_time_);

		// rendering
		// ---------
		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		// update mvp
		// ----------
		camera.update_view_matrix();

		// meshes gestion
		// --------------
		meshes_[0].model_matrix = meshes_[0].model_matrix * temp1.z_rotation(1);
		meshes_[1].model_matrix = meshes_[1].model_matrix * temp2.y_rotation(1);


		for (auto i : meshes_)
		{

			// update mvp
			// ----------
			mvp_ = camera.projection_matrix * camera.view_matrix * i.model_matrix;

			// update uniform
			// --------------
			shader->set_uniform_m4f("mvp", mvp_);

			// drawing
			// -------
			i.model->binding_vao();
	        i.model->draw(shader);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(this->window_);
	}
	//end of function
	glfwTerminate();
}

void application::framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}

void application::process_input(GLFWwindow* window, Camera& camera, const float delta_time)
{
	mouse_callback(window, camera);
	keyboard_callback(window, camera, delta_time);
}

void application::keyboard_callback(GLFWwindow* window, Camera& camera, const float delta_time)
{
	// Escape key = Exit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// WASD movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.position += camera.speed * camera.front * delta_time;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.position -= camera.speed * camera.front * delta_time;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.position += camera.right * camera.speed * delta_time;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.position -= camera.right * camera.speed * delta_time;
	}

	// Up & Down movement
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.position += camera.speed * camera.up * delta_time;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.position -= camera.speed * camera.up * delta_time;
	}

	// Sprint key
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.speed = camera.speed_sprint;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		camera.speed = camera.speed_walk;
	}
}

void application::mouse_callback(GLFWwindow* window, Camera& camera)
{
	double xpos;
    double ypos;

	glfwGetCursorPos(window, &xpos, &ypos);

    if (camera.first_mouse)
    {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.first_mouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos;
    camera.lastX = xpos;
    camera.lastY = ypos;

    constexpr float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;


    if (camera.pitch > 89.f)
    {
        camera.pitch = 89.f;
    }
    if (camera.pitch < -89.f)
    {
        camera.pitch = -89.f;
	}
    if (camera.yaw < -360.f || camera.yaw > 360.f)
    {
        camera.yaw = 0.f;
    }

	camera.front[0] = cos(degreesToRadians(camera.yaw)) * cos(degreesToRadians(camera.pitch));
    camera.front[1] = sin(degreesToRadians(camera.pitch));
    camera.front[2] = sin(degreesToRadians(camera.yaw)) * cos(degreesToRadians(camera.pitch));
    camera.front = camera.front.normalized();
}

void APIENTRY application::gl_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_param)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	default: ;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	default: ;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	default: ;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}