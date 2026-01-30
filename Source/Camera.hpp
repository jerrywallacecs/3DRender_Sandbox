// NOTE: may change this later to use polymorphism instead, as i find it cleaner
// than this mumbo jumbo

#ifndef CAMERA_H_CLASS
#define CAMERA_H_CLASS

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// defines several possible options for camera movement
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// defines camera types
enum Camera_Type
{
	FREE,
	FPS
};

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 60.0f;

class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	Camera_Type camType = FREE;

	// Camera Options
	float MovementSpeed;
	float MouseSensitivity;
	float fov;

	// Euler Angles
	float Yaw;
	float Pitch;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), fov(FOV)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), fov(FOV)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		// return glm::lookAt(Position, Position + Front, Up);
		return Look(Position, Position + Front, Up);
	}

	// keyboard input handler
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;

		// free cam
		if (camType == FREE)
		{
			if (direction == FORWARD)
				Position += Front * velocity;
			if (direction == BACKWARD)
				Position -= Front * velocity;
			if (direction == LEFT)
				Position -= Right * velocity;
			if (direction == RIGHT)
				Position += Right * velocity;
			if (direction == UP)
				Position += WorldUp * velocity;
			if (direction == DOWN)
				Position -= WorldUp * velocity;
		}

		// classic fps camera, locked to x/z plane
		if (camType == FPS)
		{
			glm::vec3 moveDirection = Front;
			moveDirection.y = 0.0f;
			moveDirection = glm::normalize(moveDirection);

			if (direction == FORWARD)
				Position += moveDirection * velocity;
			if (direction == BACKWARD)
				Position -= moveDirection * velocity;
			if (direction == LEFT)
				Position -= Right * velocity;
			if (direction == RIGHT)
				Position += Right * velocity;
		}
	}

	// mouse input handler
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// scroll input handler
	void ProcessMouseScroll(float yoffset)
	{
		fov -= (float)yoffset;
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > FOV)
			fov = FOV;
	}

	// custom implementation of the glm::LookAt function
	glm::mat4 Look(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
	{
		// calculate camera direction
		glm::vec3 zaxis = glm::normalize(position - target);

		// get positive right axis vector
		glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));

		// calculate camera up vector
		glm::vec3 yaxis = glm::cross(zaxis, xaxis);

		// create translation and rotation matrix
		// note that glm uses a column-major layout, therefore we access elements as mat[col][row]
		glm::mat4 translation = glm::mat4(1.0f);
		translation[3][0] = -position.x; // Fourth column, first row
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = xaxis.x; // First column, first row
		rotation[1][0] = xaxis.y;
		rotation[2][0] = xaxis.z;
		rotation[0][1] = yaxis.x; // First column, second row
		rotation[1][1] = yaxis.y;
		rotation[2][1] = yaxis.z;
		rotation[0][2] = zaxis.x; // First column, third row
		rotation[1][2] = zaxis.y;
		rotation[2][2] = zaxis.z;

		// return lookAt matrix as combination of translation and rotation matrix
		return rotation * translation;
	}

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		// recalculate the Right and Up vectors
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif