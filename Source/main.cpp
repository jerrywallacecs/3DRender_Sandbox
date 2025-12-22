#include "main.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec4 backgroundColor(0.025f, 0.025f, 0.025f, 1.0f);

float vertices[] = {
	// Position                // UV
	-0.5f, -0.5f, -0.5f,        0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,        1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,        0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,        0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,        0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,        1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,        1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,        1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,        0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,        0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,        1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,        0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,        1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,        1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,        0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,        1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,        1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,        1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,        1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,        0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,        0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,        0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,        1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,        1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,        0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,        0.0f, 1.0f
};

glm::vec3 cubePosition[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

// camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void cameraInput(GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main()
{
	glfwInit(); // GLFW init

	// versions and profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// windowed
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

	// fullscreen
	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "LearnOpenGL", monitor, nullptr);

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

	glViewport(0, 0, WIDTH, HEIGHT);

	// enables depth buffer
	glEnable(GL_DEPTH_TEST);

	/* 
		----------------- BUFFERS -----------------
	*/

	// vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO); // generates unique ID for the buffer

	// vertex array
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // generate unique ID for the buffer

	glBindVertexArray(VAO);

	// copies vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// set vertex attributes pointers
	
	// Position (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// UVs (location = 1)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	stbi_set_flip_vertically_on_load(true);
	// load and generate texture
	int width;
	int height;
	int numberChannels;
	// using relative paths, fix this
	unsigned char* data = stbi_load("../../../Source/Textures/T_Container.jpg", &width, &height, &numberChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture!" << std::endl;
	}
	stbi_image_free(data);

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
		std::cout << "Failed to load texture!" << std::endl;
	}
	stbi_image_free(data);

	// tell OpenGL which texture unit each shader sampler belongs to
	ourShader.Activate();
	ourShader.setInt("texture1", 0);
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
		// Handling delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*
			----------------- INPUT HANDLING -----------------
		*/

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// view in wireframe
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
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

		cameraInput(window);

		/*
			----------------- END OF INPUT HANDLING -----------------
		*/

		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		// clean the two buffers and assign the new color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, moss);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, rocky);

		ourShader.Activate();

		// create transformations
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		// camera look at
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// retrieve the matrix uniform locations
		unsigned int modelLocation = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLocation = glGetUniformLocation(ourShader.ID, "view");


		// pass the locations on to the shaders
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		ourShader.setMat4("projection", projection);
		
		/*
			----------------- RENDERING -----------------
		*/
		
		glBindVertexArray(VAO);

		// render 10 boxes and rotate the first and any that are multiple of 3
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePosition[i]);
			float angle = 20.0f * i;

			if (i == 1 || i % 3 == 0)
			{
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// unbind
		glBindVertexArray(0);

		/*
			----------------- END OF RENDERING -----------------
		*/

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}