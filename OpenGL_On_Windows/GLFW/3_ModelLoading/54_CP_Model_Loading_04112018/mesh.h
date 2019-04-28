#ifndef MESH_H
#define MESH_H

#include "Include\glad\glad.h" //holds all OpenGL type declarations

//OpenGL Mathematics (GLM) library
#include "Include\glm\glm.hpp"
#include "Include\glm\gtc\matrix_transform.hpp"
#include "Include\glm\gtc\type_ptr.hpp"

#include "shader.h" //load shaders and create program object

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include<vector>

using namespace std;

struct Vertex
{
	//position
	glm::vec3 Position;
	//normal
	glm::vec3 Normal;
	//tex coords
	glm::vec2 TexCoords;
	//tangent
	glm::vec3 Tangent;
	//bitangent
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	string type;
	string path; //we store path of the texture to compare with other textures
};


class Mesh
{
public:
	//mesh data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	//functions
	//constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		//now that we have all data set up, now set vertex buffers and it's attributes
		setupMesh();
	}

	//Render the mesh
	void Draw(Shader shader)
	{
		//bind the appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (int i = 0; i < textures.size(); i++)
		{
			//active appropriate texture unit before binding
			glActiveTexture(GL_TEXTURE0 + i);

			//retrieve texture number (N in diffuseTextureN)
			string number;
			string name = textures[i].type;

			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++); //transfer unsigned int to stream
			}else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++); //transfer unsigned int to stream
			}else if (name == "texture_normal")
			{
				number = std::to_string(normalNr++); //transfer unsigned int to stream
			}else if (name == "texture_height")
			{
				number = std::to_string(heightNr++); //transfer unsigned int to stream
			}

			//set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);

			//finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		//draw mesh
		//bind the VAO you want to use
		glBindVertexArray(VAO);

		//indexed drawing
		glDrawElements(GL_TRIANGLES,  //I want to draw triangle
				indices.size(), //total number of vertices
				GL_UNSIGNED_INT, //faces are in UNSIGNED_INT format
				0 //offset
			);

		//unbind VAO
		glBindVertexArray(0);

		//good practice to set everything back to default
		glActiveTexture(GL_TEXTURE0);
	}

private:
	//render data
	unsigned int VBO, EBO;

	//functions
	void setupMesh()
	{
		/*******************VAO starts**********************/
		//create Vertex Array Object on GPU
		glGenVertexArrays(
			1, //how many objects you want to create
			&VAO //stores the id of created VAO
		);

		//Bind VAO
		glBindVertexArray(VAO);

		/*******************VBO starts**********************/
		//create Vertex Buffer Object on GPU
		glGenBuffers(
			1, //how many objects you want to create
			&VBO //stores the id of the created VBO
		);

		//tell what type of VBO you just created
		//bind the VBO to the target so that any buffer call (on GL_ARRAY_BUFFER target) will be used to configure currently bound buffer
		glBindBuffer(
			GL_ARRAY_BUFFER, //assign type to the create buffer
			VBO //buffer
		);

		//pass data the currently bound VBO
		glBufferData(
			GL_ARRAY_BUFFER, //target
			vertices.size() * sizeof(Vertex), //size of data
			&vertices[0], //address of zeroth element; data in struct is sequential
			GL_STATIC_DRAW //data is not changing so frequently so STATIC
		);

		//tell OpenGL that how to use the data which we have just send
		//vertex position data
		glVertexAttribPointer(
			0, //layout location
			3, //each layout attribute will contain 3 floats
			GL_FLOAT, //type of data
			GL_FALSE, //I don't want to normalize the data as it is already normalized
			sizeof(Vertex), //stride - distance between each consecutive attributes
			(void *) 0 //starting offset is 0
		);

		//Now enable the vertex attribute
		glEnableVertexAttribArray(0);


		//vertex normal data
		glVertexAttribPointer(
			1, //layout location
			3, //each layout attribute will contain 3 floats
			GL_FLOAT, //type of data
			GL_FALSE, //I don't want to normalize the data as it is already normalized
			sizeof(Vertex), //stride - distance between each consecutive attributes
			(void *) offsetof(Vertex, //from this struct
								Normal //what is offset of this
							) //offset
		);

		//Now enable the vertex attribute
		glEnableVertexAttribArray(1);


		//vertex texcoords data
		glVertexAttribPointer(
			2, //layout location
			2, //each layout attribute will contain 2 floats
			GL_FLOAT, //type of data
			GL_FALSE, //I don't want to normalize the data as it is already normalized
			sizeof(Vertex), //stride - distance between each consecutive attributes
			(void *)offsetof(Vertex, //from this struct
				TexCoords //what is offset of this
			) //offset
		);

		//Now enable the vertex attribute
		glEnableVertexAttribArray(2);


		//vertex tangent data
		glVertexAttribPointer(
			3, //layout location
			3, //each layout attribute will contain 3 floats
			GL_FLOAT, //type of data
			GL_FALSE, //I don't want to normalize the data as it is already normalized
			sizeof(Vertex), //stride - distance between each consecutive attributes
			(void *)offsetof(Vertex, //from this struct
				Tangent //what is offset of this
			) //offset
		);

		//Now enable the vertex attribute
		glEnableVertexAttribArray(3);


		//vertex Bitangent data
		glVertexAttribPointer(
			4, //layout location
			3, //each layout attribute will contain 3 floats
			GL_FLOAT, //type of data
			GL_FALSE, //I don't want to normalize the data as it is already normalized
			sizeof(Vertex), //stride - distance between each consecutive attributes
			(void *)offsetof(Vertex, //from this struct
				Bitangent //what is offset of this
			) //offset
		);

		//Now enable the vertex attribute
		glEnableVertexAttribArray(4);

		//unbind the VBO
		//this is allowed - call to glVertexAttribPointer registers the VBO as vertex attributes bound to Vertex Array Object
		glBindBuffer(
			GL_ARRAY_BUFFER, //target
			0 //unbind currently bound VBO
		);
		
		/*******************VBO ends**********************/

		/*******************EBO**********************/
		//create Element Buffer Object on GPU
		glGenBuffers(
			1, //how many objects you want to create
			&EBO //stores the id of the created VBO
		);

		//bind the EBO to the target so that any buffer call (on GL_ARRAY_BUFFER target) will be used to configure currently bound buffer
		glBindBuffer(
			GL_ELEMENT_ARRAY_BUFFER, //assign type to the create buffer
			EBO //buffer
		);

		//pass data to the currently bound EBO
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, //target
			indices.size() * sizeof(unsigned int), //size of data
			&indices[0], //address of zeroth element; data in struct is sequential
			GL_STATIC_DRAW //data is not changing so frequently so STATIC
		);

		//unbind VAO: 0 means we are unbinding the currently bind VAO
		glBindVertexArray(0);

		/*******************VAO ends**********************/
		
		//unbind EBO
		//REMEMBER: do Not unbind the EBO as VAO is active as bound EBO is stored in VAO; keeps EBO bound
		//this information is stored in VAO whether it is bound or unbound
		glBindBuffer(
			GL_ELEMENT_ARRAY_BUFFER, //target
			0 //unbind the currently bound EBO
		);
	}
};
#endif // !MESH_H
