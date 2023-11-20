/*
Author: Seulgi Kim
Class: ECE6122 A
Last Date Modified: 10/28/2023

Description:

This code loads the obj file and implements a Dynamic 3D graphics application,
with lighting, shading, model transformations, and keyboard inputs.
*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

/**
 * @brief Render the object with MVP
 * @param vertexPosition_modelspaceID monkey's modelspace ID
 * @param vertexUVID monkey's uv ID
 * @param vertexNormal_modelspaceID monkey's normal modelspace ID
 * @param MatrixID monkey's matrix ID
 * @param ModelMatrixID model matrix ID
 * @param ModelMatrix model matrix
 * @param vertexbuffer model vertex buffer
 * @param uvbuffer model UV buffer
 * @param normalbuffer model normal buffer
 * @param elementbuffer model element buffer
 * @param indices monkey model indices
 * @param ProjectionMatrix camera projection matrix
 * @param ViewMatrix camera view matrix
*/
void ObjectRenderingWithMVP(GLuint& vertexPosition_modelspaceID, GLuint& vertexUVID, GLuint& vertexNormal_modelspaceID,
							GLuint& MatrixID, GLuint& ModelMatrixID,
							glm::mat4& ModelMatrix, GLuint& vertexbuffer, 
							GLuint& uvbuffer, GLuint& normalbuffer, GLuint& elementbuffer, std::vector<unsigned short>& indices,
							glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

	// 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 3rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(vertexNormal_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}

int main( void )
{
	// Initialise GLFW
	if ( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 09 - Loading with AssImp", NULL, NULL);
	if ( window == NULL )
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	
	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
	GLuint vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

	// Load the texture for monkeys
	GLuint Texture = loadDDS("uvmap.DDS");
	// Load the texture (rectangle ground)
	GLuint TextureforRectangle = loadBMP_custom("texture.bmp");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	GLuint TextureIDforRectangle = glGetUniformLocation(programID, "TextureSamplerforRectangle");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("suzanne.obj", vertices, uvs, normals);

	//////////////////////////////// Get the min.z of suzanne's chick ////////////////////////////////////////
	//////// Cannot use this code because the CMakeLists.txt does not include assimp in this file. ///////////
	// Assimp::Importer importer;
	// const aiScene* scene = importer.ReadFile("suzanne.obj", aiProcess_Triangulate);

	// aiVector3D min, max;

	// for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	// {
	// 	const aiMesh* mesh = scene->mMeshes[i];
		
	// 	for (unsigned int j = 0; j < mesh->mNumVertices; j++)
	// 	{
	// 		if (i == 0 && j == 0)
	// 		{
	// 			min = mesh->mVertices[j];
	// 			max = mesh->mVertices[j];
	// 		}
	// 		else
	// 		{
	// 			min.x = std::min(min.x, mesh->mVertices[j].x);
	// 			min.y = std::min(min.y, mesh->mVertices[j].y);
	// 			min.z = std::min(min.z, mesh->mVertices[j].z);
				
	// 			max.x = std::max(max.x, mesh->mVertices[j].x);
	// 			max.y = std::max(max.y, mesh->mVertices[j].y);
	// 			max.z = std::max(max.z, mesh->mVertices[j].z);
	// 		}
	// 	}
	// }
	// this is the mininum z coordinate for suzanne.
	float minZ = -0.95; //min.z;
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// // Read our .obj file
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	// Load it into a VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	/////////////////////////////// Binding the green rectangle & its texture //////////////////////////////////////////
	GLuint RectangleMatrixID = glGetUniformLocation(programID, "MVP"); // green rectangle
	GLuint TextureMatrixID = glGetUniformLocation(programID, "MVP");   // texture
	// Get a handle for our buffers
	// 1. rectangle
	GLuint rectangleVertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	// 2. texture
	GLuint textureVertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint textureVertexUVID = glGetAttribLocation(programID, "rectanglevertexUV");

	float rectangleWidth =  10.0f;
	float rectangleHeight = 10.0f;

	// retangle shall be located in z = 0
	static const GLfloat g_rectangleVertices[] = 
	{
		-rectangleWidth/2, -rectangleHeight/2, 0.0f,
		rectangleWidth/2, -rectangleHeight/2,  0.0f,
		rectangleWidth/2, rectangleHeight/2,   0.0f,
		-rectangleWidth/2, rectangleHeight/2,  0.0f
	};

	// texture size shall be 1/2 than rectangle.
	float textureWidth =  5.0f;
	float textureHeight = 5.0f;
	// texture shall be located in z = 0
	static const GLfloat g_textureVertices[] = 
	{
		-textureWidth/2, -textureHeight/2, 0.0f,
		textureWidth/2, -textureHeight/2,  0.0f,
		textureWidth/2, textureHeight/2,   0.0f,
		-textureWidth/2, textureHeight/2,  0.0f
	};

	// two uv coordinates for each vertex
	static const GLfloat g_uv_buffer_data[] = 
	{
		0.01f, 0.01f,
		0.99f, 0.01f,
		0.99f, 0.99f,
		0.01f, 0.99f
	};

	GLuint rectangleVBO;
	glGenBuffers(1, &rectangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_rectangleVertices), g_rectangleVertices, GL_STATIC_DRAW);

	GLuint textureVBO;
	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_textureVertices), g_textureVertices, GL_STATIC_DRAW);

	GLuint textureuvbuffer;
	glGenBuffers(1, &textureuvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureuvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	float translateDist = 1.85f;
	
	do{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{   // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();

		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glEnableVertexAttribArray(vertexUVID);
		glEnableVertexAttribArray(vertexNormal_modelspaceID);

		//////////////////////////////// Light Toggle /////////////////////////////////////
		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		glUniform1i(glGetUniformLocation(programID, "toggleLight"), getLightStatus());
		////////////////////////////////////////////////////////////////////////////////////

		// declare MVP for first object
		glm::mat4 ModelMatrix1 = glm::mat4(1.0);
		ModelMatrix1 = glm::translate(ModelMatrix1, glm::vec3(0.0f, -translateDist, -minZ));
		ModelMatrix1 = glm::rotate(ModelMatrix1, 3.14f/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		ObjectRenderingWithMVP(vertexPosition_modelspaceID, vertexUVID, vertexNormal_modelspaceID,
							 MatrixID, ModelMatrixID, ModelMatrix1, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, indices, ProjectionMatrix, ViewMatrix);

		// declare MVP for 2nd object
		glm::mat4 ModelMatrix2 = glm::mat4(1.0);
		ModelMatrix2 = glm::translate(ModelMatrix2, glm::vec3(0.0f, translateDist, -minZ));
		ModelMatrix2 = glm::rotate(ModelMatrix2, 3.14f/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix2 = glm::rotate(ModelMatrix2, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
		ObjectRenderingWithMVP(vertexPosition_modelspaceID, vertexUVID, vertexNormal_modelspaceID,
							 MatrixID, ModelMatrixID, ModelMatrix2, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, indices, ProjectionMatrix, ViewMatrix);

		// declare MVP for 3rd object
		glm::mat4 ModelMatrix3 = glm::mat4(1.0);
		ModelMatrix3 = glm::translate(ModelMatrix3, glm::vec3(translateDist, 0.0f, -minZ));
		ModelMatrix3 = glm::rotate(ModelMatrix3, 3.14f/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix3 = glm::rotate(ModelMatrix3, 3.14f/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		ObjectRenderingWithMVP(vertexPosition_modelspaceID, vertexUVID, vertexNormal_modelspaceID,
							 MatrixID, ModelMatrixID, ModelMatrix3, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, indices, ProjectionMatrix, ViewMatrix);

		// declare MVP for 4th object
		glm::mat4 ModelMatrix4 = glm::mat4(1.0);
		ModelMatrix4 = glm::translate(ModelMatrix4, glm::vec3(-translateDist, 0.0f, -minZ));
		ModelMatrix4 = glm::rotate(ModelMatrix4, 3.14f/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix4 = glm::rotate(ModelMatrix4, 3.14f/2.0f, glm::vec3(0.0f, -1.0f, 0.0f));
		ObjectRenderingWithMVP(vertexPosition_modelspaceID, vertexUVID, vertexNormal_modelspaceID,
							 MatrixID, ModelMatrixID, ModelMatrix4, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, indices, ProjectionMatrix, ViewMatrix);
		////////////////////////////////////////////////////////////// Finish Draw All objects ///////////////////////////////////////////////////

		///////////////////////////////////////////////////////////// Rendering the green rectangle //////////////////////////////////////
		// Set the flag for green rectangle rendering in the fragment shader.
		glUniform1i(glGetUniformLocation(programID, "isGreenRectangle"), 1);

		// Set the MVP for green rectangle.
		glm::mat4 ModelMatrixRectangle = glm::mat4(1.0);
		glm::mat4 rectangleMVP = getProjectionMatrix() * getViewMatrix() * ModelMatrixRectangle;
		glUniformMatrix4fv(RectangleMatrixID, 1, GL_FALSE, &rectangleMVP[0][0]);

		glEnableVertexAttribArray(rectangleVertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
		glVertexAttribPointer
		(
			rectangleVertexPosition_modelspaceID, // The attribute we want to configure
			3,                  				  // size
			GL_FLOAT,           				  // type
			GL_FALSE,           				  // normalized?
			0,                  				  // stride
			(void*)0            				  // array buffer offset
		);

		// Draw the rectangle
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(rectangleVertexPosition_modelspaceID);

		// Reset the flag after rendering
		glUniform1i(glGetUniformLocation(programID, "isGreenRectangle"), 0);
		/////////////////////////////////////////////////////////// Finish draw green rectangle /////////////////////////////////////

		///////////////////////////////////////////////////////////// Rendering the green texture //////////////////////////////////////
		// Set the flag for texture rendering in the fragment shader
		glUniform1i(glGetUniformLocation(programID, "isTexture"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureforRectangle);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureIDforRectangle, 0);

		glm::mat4 ModelMatrixTexture = glm::mat4(1.0);
		glm::mat4 textureMVP = getProjectionMatrix() * getViewMatrix() * ModelMatrixTexture;
		glUniformMatrix4fv(TextureMatrixID, 1, GL_FALSE, &textureMVP[0][0]);

		glEnableVertexAttribArray(textureVertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glVertexAttribPointer
		(
			textureVertexPosition_modelspaceID, // The attribute we want to configure
			3,                  				  // size
			GL_FLOAT,           				  // type
			GL_FALSE,           				  // normalized?
			0,                  				  // stride
			(void*)0            				  // array buffer offset
		);

		// uv positions
		glEnableVertexAttribArray(textureVertexUVID);
		glBindBuffer(GL_ARRAY_BUFFER, textureuvbuffer);
		glVertexAttribPointer(textureVertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Draw the rectangle
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(textureVertexPosition_modelspaceID);
		glDisableVertexAttribArray(textureVertexUVID);

		// Reset the flag after rendering
		glUniform1i(glGetUniformLocation(programID, "isTexture"), 0);
		///////////////////////////////////////////// Finish draw texture ////////////////////////////////////////////////////

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);
		glDisableVertexAttribArray(vertexNormal_modelspaceID);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteBuffers(1, &rectangleVBO);
	glDeleteBuffers(1, &textureuvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteTextures(1, &TextureforRectangle);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

