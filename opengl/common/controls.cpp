/*
Author: Seulgi Kim
Class: ECE6122 A
Last Date Modified: 10/28/2023

Description:

This code controls the camera perspective view by the input of the keyboard.
*/

// Include GLFW
#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include <iostream>

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::vec3 origin = glm::vec3(0,0,0);

glm::mat4 getViewMatrix()
{
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

////////////////////////////// Toggle flag for Light on/off ///////////////////////////
int toggleLight = 1;
int getLightStatus()
{
	return toggleLight;
}
//////////////////////////////////////////////////////////////////////////////////////

// Set Initial Position, angle, FOV
float distFromOrigin = 15;
float horizontalAngle = 0.5f;
float verticalAngle = glm::radians(45.0f);
float initialFoV = 45.0f;

// Set Speed
float linearSpeed = 3.0f;
float angularSpeed = 0.5f;

void computeMatricesFromInputs()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaT = float(currentTime - lastTime);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		-cos(verticalAngle) * sin(horizontalAngle),
		-cos(verticalAngle) * cos(horizontalAngle),
		-sin(verticalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		cos(horizontalAngle - 3.14f/2.0f),
		0
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// 1) 'w' key moves the camera radially closer to the origin.
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS)
	{
		distFromOrigin -= linearSpeed * deltaT;
	}
	// 2) 's' key moves the camera radially farther from the origin.
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS)
	{
		distFromOrigin += linearSpeed * deltaT;
	}
	// 3) 'a' key rotates the camera to the left maintaining the radial distance from the origin.
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS)
	{
		horizontalAngle += angularSpeed * deltaT;
	}
	// 4) 'd' key rotates to camera to the right maintaining the radial distance from the origin.
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS)
	{
		horizontalAngle -= angularSpeed * deltaT;
	}
	// 5) The up arrow key radially rotates the camera up.
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS)
	{
		verticalAngle += angularSpeed * deltaT;
	}
	// 6) The down arrow radially rotates the camera down.
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
	{
		verticalAngle -= angularSpeed * deltaT;
	}
	// 7). Turn on/off the light
	if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS)
	{
		toggleLight = 0;
	} else if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_RELEASE)
	{
		toggleLight = 1;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	ViewMatrix       = glm::lookAt(
								-distFromOrigin * direction,    // Camera is here
								origin,        					// and looks here : at the same position, plus "direction"
								up                  			// Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}