#ifndef CAMERA_H
#define CAMERA_H
/*
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 26 Oct 2018

Program Name - Setting Up Camera added keys WASD and mouse movements
It doesnot fly but only looks around while staying on xz plane
*/

//header files
#include "Include\glad\glad.h" //glad.h should be before glfw3.h; it contains correct OpenGL headers includes

#include "Include\glm\glm.hpp"
#include "Include\glm\gtc\matrix_transform.hpp"

#include <vector>

//defines possible options for camera movement
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//default camera values
const float YAW = -90.0f; //since yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to left
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f; //fov - field of view


class Camera
{
public:
	//camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//Euler angles
	float Yaw;
	float Pitch;
	//camera positions
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	//constructor with scalar values
	Camera(float posX, float posY, float posZ,
			float upX, float upY, float upZ,
			float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	//GetViewMatrix: Returns the view matrix calculated using Euler Angles and LookAt matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	//ProcessKeyboard: process input received from any keyboard-like input system.
	//Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing system)
	void ProcessKeyboard(Camera_Movement direction, 
						float deltaTime //time between current frame and last frame
							)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			Position = Position + (Front * velocity);
		}
		else if (direction == BACKWARD)
		{
			Position = Position - (Front * velocity);
		}
		else if(direction == LEFT)
		{
			Position = Position - (Right * velocity);
		}
		else if (direction == RIGHT)
		{
			Position = Position + (Right * velocity);
		}

		//makes sure the user stays at the ground level
		Position.y = 0.0f;
	}

	//ProcessMouseMovement: process input received from the mouse input system.
	//expects the offset value in both the x and y direction
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset = xoffset * MouseSensitivity;
		yoffset = yoffset * MouseSensitivity;

		Yaw = Yaw + xoffset;
		Pitch = Pitch + yoffset;

		//make sure that when pitch is out of bounds, screen doesnot gets flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
			{
				Pitch = 89.0f;
			}
			else if (Pitch < -89.0f)
			{
				Pitch = -89.0f;
			}
		}

		//Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	//ProcessMouseScroll: Process input received from mouse-scroll-wheel event
	//only required input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
		{
			Zoom = Zoom - yoffset;
		}
		if (Zoom <= 1.0f)
		{
			Zoom = 1.0f;
		}
		if (Zoom >= 45.0f)
		{
			Zoom = 45.0f;
		}
	}

	private:
		//updateCameraVectors: Calculate the front vector from the camera's (updated) Euler Angles
		void updateCameraVectors()
		{
			//calculate the new front vector
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);

			//also re-calculate the new Right and Up Vectors
			//normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
			Right = glm::normalize(glm::cross(Front, WorldUp));
			Up = glm::normalize(glm::cross(Right, Front));
		}
};
#endif // !CAMERA_H
