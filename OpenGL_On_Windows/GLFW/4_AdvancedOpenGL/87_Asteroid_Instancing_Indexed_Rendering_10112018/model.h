#ifndef MODEL_H
#define MODEL_H

#include "Include\glad\glad.h" //holds all OpenGL type declarations

//OpenGL Mathematics (GLM) library
#include "Include\glm\glm.hpp"
#include "Include\glm\gtc\matrix_transform.hpp"

//by defining this, the preprocessor modifies the header file such that it contains only the relevant definition source code
#define STB_IMAGE_IMPLEMENTATION
#include "Include\stb_image.h"

//assimp importer - to load model
#include "Include\assimp\Importer.hpp"
#include "Include\assimp\scene.h"
#include "Include\assimp\postprocess.h"
#include"mesh.h" //all mesh data
#include "shader.h" //to load and configure shaders and create program object


#include<string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include<vector>


using namespace std;

//global function declarations
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
	//model data
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
	string directory;

	//functions
	Model(string const &path)
	{
		loadModel(path);
	}

	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);
		}
	}

private:
	
	//functions
	void loadModel(string const &path)
	{
		//using Assimp to load the model into data structure of Assimp called Scene which is root node
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, //file path
									//post processing options
									aiProcess_Triangulate //if model does not(entirely) consist of triangles, it should transform all the models primitive shapes to triangles
									| aiProcess_FlipUVs //flip texture coordinates on y-axis where necessary during processing
									| aiProcess_CalcTangentSpace
								);
		cout << "inside loadModel::File Read " << path << endl;
		//check if model loaded or not
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}
	
		directory = path.substr(0, path.find_last_of('/'));
		cout << "inside loadModel::File Directory " << directory << endl;
		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode *node, const aiScene *scene)
	{
		//process the meshesh first
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			//first check each nodes mesh indices and retrieve the corresponding mesh by indexing the scene's nMeshes array
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

			//convert the above mesh into our data structure
			meshes.push_back(processMesh(mesh, scene));
		}

		//then process all the childrens of the node
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		//process vertex attributes
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//process vertex position, normal, texcoords attributes
			glm::vec3 vector;

			//position
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			//normal
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			//tex coords
			if (mesh->mTextureCoords[0]) //does the mesh contains the texcoordinates
			{
				glm::vec2 vec;
				//a vertex can contain upto 8 different texture coordinates. We thus make the assumption that we won't 
				//use models where a vertex can have multiple texture coordinates so we always take the first set 0
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;

			vertices.push_back(vertex);
		}

		//process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//process material
		if (mesh->mMaterialIndex >= 0)
		{
			//get aiMaterial first
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// diffuse: texture_diffuseN
			// specular: texture_specularN
			// normal: texture_normalN


			//diffuse material
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), //where
							diffuseMaps.begin(), //first
							diffuseMaps.end() //last
						);

			//specular material
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), //where
				specularMaps.begin(), //first
				specularMaps.end() //last
			);

			// 3. normal maps
			std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			// 4. height maps
			std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
		return Mesh(vertices, indices, textures);
	}

	/*loadMaterialTextures: 
	Iterates over all the texture locations of a given texture type, retrieves the texture file location
	and then loads and generates the texture and stores the information in vertex struct
	*/
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;

		//iterate over texture types
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			//get texture file locations
			mat->GetTexture(type, i, &str);

			bool skip = false;

			//check if texture is already loaded
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				//compare the paths
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				//if textures has not been loaded already, load it
				Texture texture;

				//loads the texture with SOIL and returns the texture ID
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture); //add to loaded textures
			}
		}
		return textures;
	}
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	//texture ID
	unsigned int textureID;
	//generate texture
	glGenTextures(
		1, //how many texture objects you want to generate
		&textureID //it will contain id of the texture generated over GPU
	);

	//LOAD IMAGE, CREATE TEXTURES AND GENERATE MIPMAPS
	int width, height, nrChannels;

	//load image data
	unsigned char* data = stbi_load(
		filename.c_str(), //file path
		&width, //this will contain width of the image
		&height, //this will contain height of the image
		&nrChannels, //this will contain number of colored channels
		0 //req_comp
	);
	//check if image data successfully loaded
	if (data) //if yes
	{
		//format of the image
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		//bind the above texture to the Texture type
		//all the upcoming GL_TEXTURE_2D operations will now affect this texture object
		glBindTexture(GL_TEXTURE_2D, //binding to this texture type
			textureID //texture to bind
		);

		//generate texture from this texture data
		glTexImage2D(
			GL_TEXTURE_2D, //target; this operation will generate texture on currently bound texture object
			0, //base mipmap level
			format, //internal format; in what kind of format you want to store the texture
			width, //width of resulting texture
			height, //height of resulting texture
			0, //this should be always 0 for legacy purpose
			format, //format of source image; png has Alpha value also
			GL_UNSIGNED_BYTE, //data is in GL_UNSIGNED_BYTE format
			data //actual data
		);

		//now the currently bound texture object has the texture image attached to it
		//but it only has the base-level of the texture image loaded
		//below will automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);

		//set texture wrapping parameters
		glTexParameteri(
			GL_TEXTURE_2D, //target type
			GL_TEXTURE_WRAP_S, //wrap for s texture axis
			GL_REPEAT //this is the default value
		);

		glTexParameteri(
			GL_TEXTURE_2D, //target type
			GL_TEXTURE_WRAP_T, //wrap for t texture axis
			GL_REPEAT //this is the default value
		);

		//set texture filtering parameters
		glTexParameteri(
			GL_TEXTURE_2D, //target type
			GL_TEXTURE_MIN_FILTER, //when scaling down
			GL_LINEAR_MIPMAP_LINEAR
		);

		glTexParameteri(
			GL_TEXTURE_2D, //target type
			GL_TEXTURE_MAG_FILTER, //when scaling up
			GL_LINEAR //takens interpolated value from the tex coordinates neighbouring texels
		);
	}
	else //if no
	{
		std::cout << "ERROR::TEXTURE::LOADING_IMAGE_FAILED" << std::endl;
	}

	//free image data now
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

	return textureID;
}
#endif // !MODEL_H
