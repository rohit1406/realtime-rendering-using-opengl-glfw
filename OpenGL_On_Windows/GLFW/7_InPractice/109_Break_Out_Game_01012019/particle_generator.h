/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 15 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H

#include<iostream>
#include"include\GL\glew.h"
#include"include\glm\glm.hpp"

#include"shader.h"
#include"texture.h"
#include"game_object.h"
#include<vector>
struct Particle
{
	glm::vec2 position, velocity;
	glm::vec4 color;
	GLfloat life;

	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f)
	{

	}
};

class ParticleGenerator
{
public:
	//constructor
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	//update all particles
	void update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	//render all particles
	void draw();

private:
	//state
	std::vector<Particle> particles;
	GLuint amount;
	//render state
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	//initialize vertex and buffer attributes
	void init();
	//returns the first unused particle index
	GLuint firstUnusedParticle();

	//respawns particles
	void respawnsParticle(Particle &particle, GameObject &object, glm::vec2 offset=glm::vec2(0.0f,0.0f));
};
#endif // !PARTICLEGENERATOR_H
