/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 14 Jan 2011

Program Name - Breakout game
*************************************************************/

#include "game_level.h"
#include <fstream>
#include <sstream>

GameLevel::GameLevel()
{

}
void GameLevel::load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
	//clear old data
	this->bricks.clear();
	//load from file
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode) //read each word seperated by spaces
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
		{
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint lvlWidht, GLuint lvlHeight)
{
	//calculate dimensions
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = lvlWidht / static_cast<GLfloat>(width);
	GLfloat unit_height = lvlHeight / height;

	//initialize level tiles based on tiles data
	for (GLuint y = 0; y < height; ++y)
	{
		for(GLuint x = 0; x < width; ++x)
		{
			//check block type from level data
			if (tileData[y][x] == 1) //solid
			{
				glm::vec2 pos(unit_width * x, unit_height *y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, 
					ResourceManager::getTexture("block_solid"),
					glm::vec3(0.8, 0.8, 0.7));
				obj.isSolid = GL_TRUE;
				this->bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f); //original white
				if (tileData[y][x] == 2)
				{
					color = glm::vec3(0.2f,0.6f,1.0f);
				}else if (tileData[y][x] == 3)
				{
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}else if (tileData[y][x] == 4)
				{
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}else if (tileData[y][x] == 5)
				{
					color = glm::vec3(1.0f, 5.0f, 0.0f);
				}
				glm::vec2 pos(unit_width *x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				this->bricks.push_back(GameObject(pos, size, ResourceManager::getTexture("block"), color));
			}
		}
	}
}

void GameLevel::draw(SpriteRenderer &renderer)
{
	for (GameObject &tile : this->bricks)
	{
		if (!tile.destroyed)
		{
			tile.draw(renderer);
		}
	}
}

GLboolean GameLevel::isCompleted()
{
	for (GameObject &tile : this->bricks)
	{
		if (!tile.isSolid && !tile.destroyed)
		{
			return GL_FALSE;
		}
	}
	return GL_TRUE;
}