#include "main.h"

unsigned int loadTexture(const char* filepath);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

// light source
glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
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

	// tell opengl to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program
	// ------------------------------------
	Shader materialShader("../../../Source/Shaders/default.vert", "../../../Source/Shaders/default.frag");
	Shader lightSourceShader("../../../Source/Shaders/light.vert", "../../../Source/Shaders/light.frag");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// Position				// NORMALS				// UV
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	    0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normals (location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// UVs (location = 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// LIGHT - seperate VAO as lights do not need uv's or normals
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	

	/*
		----------------- END OF BUFFERS -----------------
	*/

	// loading texture
	unsigned int crateDiffuse = loadTexture("../../../Source/Textures/container2.png");
	unsigned int crateSpecular = loadTexture("../../../Source/Textures/container2_specular_colored.png");
	unsigned int crateEmission = loadTexture("../../../Source/Textures/matrix.jpg");

	// shader configuration
	materialShader.Activate();
	materialShader.setInt("material.diffuse", 0);
	materialShader.setInt("material.specular", 1);
	materialShader.setInt("material.emission", 2);

	/*
			----------------- MAIN LOOP -----------------
	*/

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		materialShader.Activate();
		materialShader.setVec3("viewPosition", camera.Position);
		materialShader.setVec3("light.position", lightPosition);
		// needed to get normalized direction vector from world position for spotlight
		glm::vec3 target = glm::vec3(3.0f, 2.8f, -8.0f);
		glm::vec3 direction = glm::normalize(target - lightPosition);
		//materialShader.setVec3("light.direction", camera.Front);
		materialShader.setFloat("light.innerCutoff", glm::cos(glm::radians(12.5f)));
		materialShader.setFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));

		// light properties
		materialShader.setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		materialShader.setVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		materialShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		materialShader.setFloat("light.emissiveStrength", 0.0f);

		materialShader.setFloat("light.constant", 1.0f);
		materialShader.setFloat("light.linear", 0.09f);
		materialShader.setFloat("light.quadratic", 0.032f);

		// material properties
		materialShader.setFloat("material.shininess", 32.0f);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		materialShader.setModelMatrix(model);

		// view/projection transformation
		glm::mat4 view = camera.GetViewMatrix();
		materialShader.setViewMatrix(view);
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		materialShader.setProjectionMatrix(projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, crateDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, crateSpecular);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, crateEmission);

		// render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// directional light demo
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model2, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			materialShader.setModelMatrix(model2);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//////////////////////////////////////////////////////////

		// LIGHT RENDER
		lightSourceShader.Activate();
		lightSourceShader.setViewMatrix(view);
		lightSourceShader.setProjectionMatrix(projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f)); // smaller cube
		
		// animating
		float radius = 3.0f;
		float time = glfwGetTime();

		lightPosition.x = cos(time) * radius;
		lightPosition.z = sin(time) * radius;
		lightPosition.y = 1.0f;
		model = glm::translate(model, lightPosition);

		lightSourceShader.setModelMatrix(model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

// takes in filepath and returns a texture that was created from that texture file
unsigned int loadTexture(const char* filepath)
{
	int textureWidth;
	int textureHeight;
	int textureChannels;
	unsigned char* textureData;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	// channels:
	// 1	grayscale	GL_RED
	// 3	RGB			GL_RGB
	// 4	RGBA		GL_RGBA

	// load the texture data
	textureData = stbi_load(filepath, &textureWidth, &textureHeight, &textureChannels, 0);

	// could use a function here
	if (textureData)
	{
		switch (textureChannels)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		}
	}
	else
	{
		std::cout << "Failed to load texture!" << std::endl;
	}
	stbi_image_free(textureData);

	return textureID;
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
		camera.ProcessKeyboard(FORWARD, deltaTime, FREE);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime, FREE);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime, FREE);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime, FREE);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime, FREE);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime, FREE);

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