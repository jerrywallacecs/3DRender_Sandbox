#include "main.h"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

std::vector<UnitCube> bullets = {};

// settings
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;
bool fullscreen = false;

glm::vec4 backgroundColor(0.0f, 0.0f, 0.0f, 1.0f);

// camera
Camera camera(glm::vec3(0.0f, 0.5f, 2.0f));
bool firstMouse = true;
float lastX = (float)SCREEN_WIDTH / 2.0;
float lastY = (float)SCREEN_HEIGHT / 2.0;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// model outline
bool viewOutline = false;

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

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);

	// fullscreen
	/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "LearnOpenGL", monitor, nullptr);*/

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

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND); // render images with different levels of transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// build and compile our shader program
	// ------------------------------------
	Shader mainShader("../../../Source/Shaders/default.vert", "../../../Source/Shaders/default.frag");
	Shader basicShader("../../../Source/Shaders/basic.vert", "../../../Source/Shaders/basic.frag");
	Shader outlineShader("../../../Source/Shaders/basic.vert", "../../../Source/Shaders/outline.frag");

	// vertex data and buffers
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// load textures
	unsigned int cubeTexture = TextureFromFile("Source/Resources/Textures/pixelBase.jpg", "../../../");
	//unsigned int planeTexture = TextureFromFile("Source/Resources/Textures/metalDiffuse.jpg", "../../../");
	unsigned int planeTexture = TextureFromFile("Source/Resources/Textures/ground.png", "../../../");
	unsigned int windowTexture = TextureFromFile("Source/Resources/Textures/window.png", "../../../");

	// load model
	Model mainModel("../../../Source/Resources/Models/backpack/backpack.obj");
	
	// load unit cube (1x1 textured cube)
	UnitCube thisCube(cubeTexture);
	UnitCube windowCube(windowTexture);

	// VEGETATION
	std::vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	unsigned int grassTexture = TextureFromFile("Source/Resources/Textures/grass.png", "../../../");

	// note that we are repeating vertices due to not using a EBO
	float vegetationVertices[] =
	{
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top left
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left

		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
	};

	unsigned int grassVAO, grassVBO;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vegetationVertices), &vegetationVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);



	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//std::cout << "current framerate: " << (1 / deltaTime) << std::endl;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// set uniforms
		outlineShader.Activate();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		outlineShader.setViewMatrix(view);
		outlineShader.setProjectionMatrix(projection);

		basicShader.Activate();
		basicShader.setViewMatrix(view);
		basicShader.setProjectionMatrix(projection);

		// draw floor as normal, but don't write the floor to the stencil buffer.
		// we only care about the containers.
		glStencilMask(0x00);

		// floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, planeTexture);
		model = glm::mat4(1.0f);
		basicShader.setModelMatrix(model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// 1st render pass, draw objects as normal. writing to stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		basicShader.setModelMatrix(model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		basicShader.setModelMatrix(model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 2nd render pass. draw slightly scaled versions of the objects
		// without stencil writing
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		outlineShader.Activate();
		float scale = 1.05f;

		// cubes
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		outlineShader.setModelMatrix(model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		outlineShader.setModelMatrix(model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// 3d model
		mainShader.Activate();
		mainShader.setViewMatrix(view);
		mainShader.setProjectionMatrix(projection);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		mainShader.setModelMatrix(model);
		
		if (viewOutline)
		{
			mainModel.Draw(mainShader, outlineShader, model);
		}
		else
		{
			mainModel.Draw(mainShader);
		}

		// to make blending work, we draw the most distant object
		// first and the closest object last
		// ---------------------------------
		// when drawing a scene with non-transparent and transparent
		// objects the general outline is as follows:
		// 1. Draw all opaque objects first
		// 2. Sort transparent objects
		// 3. Draw transparent objects in order

		// basic rotating window cube
		basicShader.Activate();
		basicShader.setViewMatrix(view);
		basicShader.setProjectionMatrix(projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
		model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime() * 20.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		basicShader.setModelMatrix(model);
		windowCube.Draw(basicShader);

		// unit cube
		basicShader.Activate();
		basicShader.setViewMatrix(view);
		basicShader.setProjectionMatrix(projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 5.0f));
		model = glm::scale(model, glm::vec3(10.0f, 2.0f, 0.25f));
		basicShader.setModelMatrix(model);
		thisCube.Draw(basicShader);

		// walls
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -5.0f));
		model = glm::scale(model, glm::vec3(10.0f, 2.0f, 0.25));
		basicShader.setModelMatrix(model);
		thisCube.Draw(basicShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 2.0f, 0.25f));
		basicShader.setModelMatrix(model);
		thisCube.Draw(basicShader);


		// OUTLINE TEST
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 2.0f, 0.25f));
		basicShader.setModelMatrix(model);
		thisCube.Draw(basicShader);

		// VEGETATION
		glBindVertexArray(grassVAO);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		// sort grass textures
		std::map<float, glm::vec3> sortedGrass;
		for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			float distance = glm::length(camera.Position - vegetation[i]);
			sortedGrass[distance] = vegetation[i];
		}

		// render grass textures in order
		for (std::map<float, glm::vec3>::reverse_iterator it = sortedGrass.rbegin(); it != sortedGrass.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);

			// make the grass face the camera at all times
			glm::vec3 direction = camera.Position - it->second;
			float angle = glm::atan(direction.x, direction.z); // note this returns angle in radians, not degrees so we dont have to use glm::radians(angle)
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

			basicShader.setModelMatrix(model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// SHOOTING
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		{
			UnitCube Bullet(cubeTexture);
			bullets.push_back(Bullet);
		}

		std::cout << "number of bullets: " << bullets.size() << std::endl;

		for (unsigned int i = 0; i < bullets.size(); i++)
		{
			basicShader.Activate();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, static_cast<float>(i), 2.0f));
			basicShader.setModelMatrix(model);
			bullets[i].Draw(basicShader);
		}

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// cleanup
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

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

	// camera controls
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

	// sprinting
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed = 5.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		camera.MovementSpeed = 2.5f;

	// view in wireframe
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// view model outline
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		viewOutline = true;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
	{
		viewOutline = false;
	}
}