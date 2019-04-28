/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 14 Jan 2011

Program Name - Breakout game
*************************************************************/
#include "game_object.h"

GameObject::GameObject() : position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f),
sprite(), isSolid(false), destroyed(false)
{

}

GameObject::GameObject(glm::vec2 pos, glm::vec2 s, Texture2D sp, glm::vec3 c, glm::vec2 v)
	:position(pos), size(s), velocity(v), color(c), rotation(0.0f), sprite(sp), isSolid(false), destroyed(false)
{

}

void GameObject::draw(SpriteRenderer &renderer)
{
	renderer.drawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}