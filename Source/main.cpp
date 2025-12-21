#include "main.h"

[[maybe_unused]] const unsigned int WIDTH = 800;
[[maybe_unused]] const unsigned int HEIGHT = 600;

glm::vec4 backgroundColor(0.025f, 0.025f, 0.025f, 1.0f);

float vertices[] =
{
	 // positions		// colors		// texture coords
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
};

unsigned int indices[] =
{
	0, 1, 2,
	0, 2, 3
};

int main()
{
	glfwInit(); // GLFW init

	// versions and profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "LearnOpenGL", monitor, nullptr);

	if (window == nullptr)
	{
		std::cerr << "failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	glViewport(0, 0, mode->width, mode->height);

	/* 
		----------------- BUFFERS -----------------
	*/

	// vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO); // generates unique ID for the buffer

	// vertex array
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // generate unique ID for the buffer
	
	// index buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO); // generates unique ID for the buffer

	glBindVertexArray(VAO);

	// copies vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// copy our index array in an element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 *sizeof(float)));
	glEnableVertexAttribArray(2);

	/*
		----------------- END OF BUFFERS -----------------
	*/

	/*
		----------------- SHADERS -----------------
	*/

	// currently using relative paths. works but need to fix cmake
	Shader ourShader("../../../Source/Shaders/default.vert", "../../../Source/Shaders/default.frag");
	/*
		----------------- END OF SHADERS -----------------
	*/

	/*
		----------------- TEXTURES -----------------
	*/

	unsigned int moss;
	glGenTextures(1, &moss);
	glBindTexture(GL_TEXTURE_2D, moss);

	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate texture
	int width;
	int height;
	int numberChannels;
	// using relative paths, fix this
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../../../Source/Textures/T_MossBrick.png", &width, &height, &numberChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture!" << std::endl;
	}

	unsigned int rocky;
	glGenTextures(1, &rocky);
	glBindTexture(GL_TEXTURE_2D, rocky);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("../../../Source/Textures/T_Rocky.png", &width, &height, &numberChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture!" << std::endl;
	}

	// tell OpenGL which texture unit each shader sampler belongs to
	ourShader.Activate();
	ourShader.setInt("texture2", 1);
	float textureOpacity = 0.0f;
	ourShader.setFloat("opacity", textureOpacity); // sets default opacity to 0

	/*
		----------------- END OF TEXTURES -----------------
	*/
	
	/*
			----------------- MAIN LOOP -----------------
	*/

	while (!glfwWindowShouldClose(window))
	{
		
		/*
			----------------- INPUT HANDLING -----------------
		*/


		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
			return 0;
		}

		// view in wireframe
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// light mode
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			backgroundColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
		{
			backgroundColor = { 0.025f, 0.025f, 0.025f, 1.0f };
		}

		// change textures
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			if (textureOpacity < 1.0f)
			{
				textureOpacity += .01f;
				ourShader.setFloat("opacity", textureOpacity);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			if (textureOpacity > 0)
			{
				textureOpacity -= .01f;
				ourShader.setFloat("opacity", textureOpacity);
			}
		}

		/*
			----------------- END OF INPUT HANDLING -----------------
		*/

		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		// clean the two buffers and assign the new color
		glClear(GL_COLOR_BUFFER_BIT);

		/*
			----------------- DRAW CAll -----------------
		*/
		ourShader.Activate();

		// render
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, moss);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, rocky);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		/*
			----------------- END OF DRAW CAll -----------------
		*/

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// cleanup
	stbi_image_free(data);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
