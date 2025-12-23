#include "main.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

glm::vec4 backgroundColor(0.025f, 0.025f, 0.025f, 1.0f);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = (float)WIDTH / 2.0;
float lastY = (float)HEIGHT / 2.0;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides the cursor and captures it
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	// enables depth buffer
	glEnable(GL_DEPTH_TEST);

	// currently using relative paths. works but need to fix cmake
	Shader ourShader("../../../Source/Shaders/default.vert", "../../../Source/Shaders/default.frag");
	Shader groundShader("../../../Source/Shaders/default.vert", "../../../Source/Shaders/default.frag");

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
		glm::vec3(-5.0f, 2.5f, -10.0f),
		glm::vec3(8.0f, 5.0f, -20.0f),
		glm::vec3(-10.0f, 3.0f, -5.0f),
		glm::vec3(12.0f, 4.2f, -15.0f),
		glm::vec3(3.0f, 2.8f, -8.0f),
		glm::vec3(-7.0f, 3.5f, -12.0f),
		glm::vec3(5.0f, 2.2f, -3.0f),
		glm::vec3(10.0f, 2.7f, -6.0f),
		glm::vec3(-3.0f, 3.0f, -1.0f),
		glm::vec3(0.0f, 2.4f, -18.0f)
	};



	float groundVertices[] =
	{
		// POSITION			  // UV
		-15.0f, 0.0f, -15.0f, 0.0f, 0.0f,
		 15.0f, 0.0f, -15.0f, 6.0f, 0.0f,
		 15.0f, 0.0f,  15.0f, 6.0f, 6.0f,

		-15.0f, 0.0f, -15.0f, 0.0f, 0.0f,
		 15.0f, 0.0f,  15.0f, 6.0f, 6.0f,
		-15.0f, 0.0f,  15.0f, 0.0f, 6.0f
	};

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
	
	// Position (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// UVs (location = 1)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// GROUND




	unsigned int groundVBO, groundVAO;
	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindVertexArray(groundVAO);
	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
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
		----------------- TEXTURES -----------------
	*/

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering options (on currently bound texture)
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

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("../../../Source/Textures/T_MossBrick.png", &width, &height, &numberChannels, 0);

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

	groundShader.Activate();
	groundShader.setInt("texture1", 0);
	groundShader.setInt("texture2", 1);
	groundShader.setFloat("opacity", 1.0f); // sets ground texture to moss

	/*
		----------------- END OF TEXTURES -----------------
	*/

	
	
	/*
			----------------- MAIN LOOP -----------------
	*/

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*
			----------------- INPUT HANDLING -----------------
		*/

		ourShader.Activate();

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

		processInput(window);

		/*
			----------------- END OF INPUT HANDLING -----------------
		*/



		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		// clean the two buffers and assign the new color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on coresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// pass projection matrix to shader
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
		
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

		// GROUND


		groundShader.Activate();

		// pass projection matrix to shader
		groundShader.setMat4("projection", projection);

		// camera/view transformation
		groundShader.setMat4("view", view);

		glm::mat4 groundModel = glm::mat4(1.0f);
		groundModel = glm::translate(groundModel, glm::vec3(0.0f, -1.0f, 0.0f));
		groundShader.setMat4("model", groundModel);
		glBindVertexArray(groundVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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


// GLFW: called when window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// GLFW: called when mouse moves
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos; // reversed since y-coords go bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime, FPS);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime, FPS);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime, FPS);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime, FPS);

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
}