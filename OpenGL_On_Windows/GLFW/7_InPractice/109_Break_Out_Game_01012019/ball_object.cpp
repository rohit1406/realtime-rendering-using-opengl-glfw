/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 14 Jan 2011

Program Name - Breakout game
*************************************************************/
#include"ball_object.h"

BallObject::BallObject() : GameObject(), radius(12.5f), stuck(true)
{

}

BallObject::BallObject(glm::vec2 pos, GLfloat rad, glm::vec2 vel, Texture2D sp)
	: GameObject(pos, glm::vec2(rad * 2, rad * 2), sp, glm::vec3(1.0f), vel), radius(rad), stuck(true)
{

}
glm::vec2 BallObject::move(GLfloat dt, GLuint win_width)
{
	//if not stuck to player board
	if (!this->stuck)
	{
		//move the ball
		this->position = this->position + this->velocity * dt;

		//check if outside window bounds
		//no code to bounce the ball from bottom edge of window
		if (this->position.x <= 0.0f)
		{
			this->velocity.x = -this->velocity.x;
			this->position.x = 0.0f;
		}
		else if (this->position.x + this->size.x >= win_width)
		{
			this->velocity.x = -this->velocity.x;
			this->position.x = win_width - this->size.x;
		}
		if (this->position.y <= 0)
		{
			this->velocity.y = -this->velocity.y;
			this->position.y = 0.0f;
		}

	}
	return this->position;
}

void BallObject::reset(glm::vec2 pos, glm::vec2 vel)
{
	this->position = pos;
	this->velocity = vel;
	this->stuck = true;
}