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
glm::vec3 lightPosition(0.0f, 1.0f, 2.0f);

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
	Shader lightShader("../../../Source/Shaders/light.vert", "../../../Source/Shaders/light.frag");

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
		// Position             // Normal        // UV
		-15.0f, 0.0f, -15.0f,   0,1,0,            0.0f, 0.0f,
		 15.0f, 0.0f, -15.0f,   0,1,0,           10.0f, 0.0f,
		 15.0f, 0.0f,  15.0f,   0,1,0,           10.0f,10.0f,

		-15.0f, 0.0f, -15.0f,   0,1,0,            0.0f, 0.0f,
		 15.0f, 0.0f,  15.0f,   0,1,0,           10.0f,10.0f,
		-15.0f, 0.0f,  15.0f,   0,1,0,            0.0f,10.0f
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

	// GROUND
	unsigned int groundVBO, groundVAO;
	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindVertexArray(groundVAO);
	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
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

	unsigned int crateTexture = loadTexture("../../../Source/Textures/T_Crate.jpg");
	unsigned int mossTexture = loadTexture("../../../Source/Textures/T_MossBrick.png");
	
	// some lighting ya know the vibes
	ourShader.Activate();
	ourShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	groundShader.Activate();
	groundShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	groundShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	/*
			----------------- MAIN LOOP -----------------
	*/

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		processInput(window);

		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		// clean the two buffers and assign the new color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//////////////////////////////////////////////////////////

		// GROUND RENDER
		groundShader.Activate();
		glActiveTexture(GL_TEXTURE0); // select texture unit 0
		glBindTexture(GL_TEXTURE_2D, mossTexture); // bind texture
		groundShader.setInt("m_texture", 0); // tell the shader to use texture unit 0

		// pass uniforms that change per frame
		groundShader.setMat4("projection", projection); // projection matrix
		groundShader.setMat4("view", view); // camera/view transformation
		groundShader.setVec3("lightPosition", lightPosition);
		groundShader.setVec3("viewPosition", camera.Position);

		// transformations
		glm::mat4 groundModel = glm::mat4(1.0f);
		groundModel = glm::translate(groundModel, glm::vec3(0.0f, -1.0f, 0.0f));
		groundShader.setMat4("model", groundModel);

		// render
		glBindVertexArray(groundVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//////////////////////////////////////////////////////////

		// CRATE RENDER
		ourShader.Activate();

		// textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, crateTexture);
		ourShader.setInt("m_texture", 0);

		// uniforms
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		ourShader.setVec3("lightPosition", lightPosition);
		ourShader.setVec3("viewPosition", camera.Position);

		// crate1 transformations
		glm::mat4 crate1 = glm::mat4(1.0f);
		crate1 = glm::translate(crate1, glm::vec3(0.0f, -0.5f, 0.0f));
		ourShader.setMat4("model", crate1);

		// crate1 render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// crate2 transformations
		glm::mat4 crate2 = glm::mat4(1.0f);
		crate2 = glm::translate(crate2, glm::vec3(0.0f, 0.375f, 0.0f));
		crate2 = glm::rotate(crate2, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // args: matrix to rotate, amount of rotation in radians, axis of rotation
		crate2 = glm::scale(crate2, glm::vec3(0.75f, 0.75f, 0.75f));
		ourShader.setMat4("model", crate2);

		// crate2 render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//////////////////////////////////////////////////////////

		// LIGHT RENDER
		lightShader.Activate();

		// uniforms
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);

		// transformations
		glm::mat4 light = glm::mat4(1.0f);
		light = glm::rotate(light, glm::radians(static_cast<float>(deltaTime) * 100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 rotated = light * glm::vec4(lightPosition, 1.0f);
		lightPosition = glm::vec3(rotated);
		light = glm::translate(light, lightPosition);
		light = glm::scale(light, glm::vec3(0.25f, 0.25f, 0.25f));
		lightShader.setMat4("model", light);

		// render
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, &groundVBO);
	glDeleteVertexArrays(1, &lightVAO);

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

	// load the texture data
	textureData = stbi_load(filepath, &textureWidth, &textureHeight, &textureChannels, 0);

	// could use a function here
	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
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