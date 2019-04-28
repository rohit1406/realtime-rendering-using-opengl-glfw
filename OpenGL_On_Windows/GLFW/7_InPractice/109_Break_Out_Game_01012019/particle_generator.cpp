/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 15 Jan 2011

Program Name - Breakout game
*************************************************************/
#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader s, Texture2D t, GLuint a)
	: shader(s), texture(t), amount(a)
{
	this->init();
}

void ParticleGenerator::update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset)
{
	//add new particles
	for (GLuint i = 0; i < newParticles; i++)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnsParticle(this->particles[unusedParticle], object, offset);
	}

	//update all particles
	for (GLuint i = 0; i < this->amount; i++)
	{
		Particle &p = this->particles[i];
		p.life -= dt; //reduce life
		if (p.life > 0.0f)
		{
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5;
		}
	}
}


//render all particles
void ParticleGenerator::draw()
{
	//use additive blending to give it a glow effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.use();
	for (Particle particle : this->particles)
	{
		if (particle.life > 0.0f)
		{
			this->shader.setVector2f("u_offset", particle.position);
			this->shader.setVector4f("u_color", particle.color);
			this->texture.bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	//reset default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	// Set up mesh and attribute properties
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

//store the index of last particle used
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	//first search from last used particle
	for (GLuint i = lastUsedParticle; i < this->amount; i++)
	{
		if (this->particles[i].life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	//otherwise do linear search
	for (GLuint i = 0; i < lastUsedParticle; i++)
	{
		if (this->particles[i].life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	lastUsedParticle = 0; //reset
	return 0;
}

void ParticleGenerator::respawnsParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.position = object.position + random + offset;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.life = 1.0f;
	particle.velocity = object.velocity * 0.1f;
}