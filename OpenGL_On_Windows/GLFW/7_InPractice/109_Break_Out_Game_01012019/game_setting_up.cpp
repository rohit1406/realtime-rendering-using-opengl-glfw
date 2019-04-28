/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 1 Jan 2011

Program Name - Breakout game
*************************************************************/
#include "game_setting_up.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include"ball_object.h"
#include "particle_generator.h"
//method signatures
Direction viewerDirection(glm::vec2 target);

SpriteRenderer *renderer;
ParticleGenerator *particles;

const glm::vec2 PLAYER_SIZE(100, 20); //player paddle
const GLfloat PLAYER_VELOCITY(500.0f); //velocity of paddle
GameObject *player;

//ball states
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
//radius of ball
const GLfloat BALL_RADIUS = 12.5f;
BallObject *ball;

Game::Game(GLuint width, GLuint height) : state(GAME_ACTIVE), keys(), consoleWidth(width), consoleHeight(height)
{

}

Game::~Game()
{
	delete renderer;
	delete ball;
	delete player;
	delete particles;
}

void Game::init()
{
	//load shaders
	ResourceManager::loadShader("shaders/sprites.vs", "shaders/sprites.fs", nullptr, "sprites");
	ResourceManager::loadShader("shaders/particles.vs", "shaders/particles.fs", nullptr, "particles");
	//configure shaders
	glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<GLfloat>(this->consoleWidth),
		static_cast<GLfloat>(this->consoleHeight), 0.0f,-1.0f, 1.0f);
	
	ResourceManager::getShader("sprites").use().setInteger("u_sampler_image", 0, true);
	ResourceManager::getShader("sprites").use().setMatrix4("u_projection", projectionMatrix, true);
	ResourceManager::getShader("particles").use().setInteger("u_sampler_image", 0, true);
	ResourceManager::getShader("particles").use().setMatrix4("u_projection", projectionMatrix, true);
	//load textures
	ResourceManager::loadTexture("resources/textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::loadTexture("resources/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::loadTexture("resources/textures/block.png", GL_FALSE, "block");
	ResourceManager::loadTexture("resources/textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::loadTexture("resources/textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::loadTexture("resources/textures/particle.png", GL_TRUE, "particle");

	renderer = new SpriteRenderer(ResourceManager::getShader("sprites"));

	particles = new ParticleGenerator(ResourceManager::getShader("particles"),
		ResourceManager::getTexture("particle"), 500);
	//load levels
	GameLevel one, two, three, four;
	one.load("resources/levels/one.lvl", this->consoleWidth, this->consoleHeight * 0.5);
	two.load("resources/levels/two.lvl", this->consoleWidth, this->consoleHeight * 0.5);
	three.load("resources/levels/three.lvl", this->consoleWidth, this->consoleHeight * 0.5);
	four.load("resources/levels/four.lvl", this->consoleWidth, this->consoleHeight * 0.5);
	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	this->level = 0;

	//player details
	glm::vec2 playerPos = glm::vec2(
	this->consoleWidth/2 - PLAYER_SIZE.x/2,
		this->consoleHeight - PLAYER_SIZE.y
	);
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

	//ball details
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x/2 - BALL_RADIUS, -BALL_RADIUS * 2);
	ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));
	
}

void Game::update(GLfloat dt)
{
	ball->move(dt, this->consoleWidth);

	this->doCollisions();

	//update particles
	particles->update(dt, *ball, 2, glm::vec2(ball->radius / 2));

	if (ball->position.y >= this->consoleHeight) //did ball reach bottom edge?
	{
		this->resetLevel();
		this->resetPlayer();
	}


}

void Game::processInput(GLfloat dt)
{
	if (this->state == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		//move playerboard
		if (this->keys[GLFW_KEY_A])
		{
			if (player->position.x >= 0)
			{
				player->position.x = player->position.x - velocity;
				if (ball->stuck)
				{
					ball->position.x -= velocity;
				}
			}
		}else if (this->keys[GLFW_KEY_D])
		{
			if (player->position.x <= this->consoleWidth - player->size.x)
			{
				player->position.x = player->position.x + velocity;
				if (ball->stuck)
				{
					ball->position.x += velocity;
				}
			}

		}

		if (this->keys[GLFW_KEY_SPACE])
		{
			ball->stuck = false;
		}
	}
}

void Game::render()
{
	//draw background
	if(this->state == GAME_ACTIVE)
	{
	renderer->drawSprite(ResourceManager::getTexture("background"), 
		glm::vec2(0, 0), 
		glm::vec2(this->consoleWidth,this->consoleHeight), 0.0f);
	}
	//draw levels
	this->levels[this->level].draw(*renderer);

	player->draw(*renderer);
	particles->draw();
	ball->draw(*renderer);
}

GLboolean Game::checkCollision(GameObject &one, GameObject &two)
{
	//x axis collission?
	bool collisionX = one.position.x + one.size.x >= two.position.x &&
		two.position.x + two.size.x >= one.position.x;
	//y axis collission?
	bool collisionY = one.position.y + one.size.y >= two.position.y &&
		two.position.y + two.size.y >= one.position.x;

	return  collisionX && collisionY;
}

Collission Game::checkCollision(BallObject &one, GameObject &two)
{
	//get center point circle first
	glm::vec2 center(one.position + one.radius);
	//calculate AABB info (center, half-extends)
	glm::vec2 aabb_half_extents(two.size.x / 2, two.size.y / 2);
	glm::vec2 aabb_center(two.position.x + aabb_half_extents.x,
		two.position.y + aabb_half_extents.y);

	//get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	//add clamped value to aabb center and we get the value of box closest to the center
	glm::vec2 closest = aabb_center + clamped;
	//retrieve vector between center circle and closest point aabb and check if length <= radius
	difference = closest - center;

	if (glm::length(difference) <= one.radius)
	{
		return std::make_tuple(GL_TRUE, viewerDirection(difference), difference);
	}
	else
	{
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f,0.0f));
	}
}

void Game::doCollisions()
{
	for (GameObject &box : this->levels[this->level].bricks)
	{
		if (!box.destroyed)
		{
			Collission collission = checkCollision(*ball, box);
			if (std::get<0>(collission))
			{
				//destroy block if not solid
				if (!box.isSolid)
				{
					box.destroyed = GL_TRUE;
				}

				//collission resolution
				Direction dir = std::get<1>(collission);
				glm::vec2 diff_vector = std::get<2>(collission);
				if (dir == LEFT || dir == RIGHT) //horizontal collission
				{
					ball->velocity.x = -ball->velocity.x; //reverse horizontal velocity
					//relocate
					GLfloat penetration = ball->radius - std::abs(diff_vector.x);
					if (dir == LEFT)
					{
						ball->position.x += penetration;
					}
					else
					{
						ball->position.x -= penetration;
					}
				}
				else
				{
					ball->velocity.y = -ball->velocity.y; //reverse verticle velocity
					//relocate
					GLfloat penetration = ball->radius - std::abs(diff_vector.y);
					if (dir == UP)
					{
						ball->position.y -= penetration;
					}
					else
					{
						ball->position.y += penetration;
					}
				}
			}
		}

		//ball and player collission
		Collission result = checkCollision(*ball, *player);
		if (!ball->stuck && std::get<0>(result))
		{
			//check whether it hit the board, and change velocity based on where it hit the board
			GLfloat centerBoard = player->position.x + player->size.x / 2;
			GLfloat distance = (ball->position.x + ball->radius) - centerBoard;
			GLfloat percentage = distance / (player->size.x / 2);

			//then move accordingly
			glm::vec2 oldVelocity = ball->velocity;
			GLfloat strength = 2.0f;
			ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
			//ball->velocity.y = -ball->velocity.y;
			ball->velocity.y = -1 * std::abs(ball->velocity.y);
			ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
		}
	}
}


Direction viewerDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f), //up
		glm::vec2(1.0f, 0.0f), //right
		glm::vec2(0.0f, -1.0f), //down
		glm::vec2(-1.0f, 0.0f) //left
	};

	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat	dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

void Game::resetLevel()
{
	
	if (this->level == 0)
	{
		this->levels[0].load("resources/levels/one.lvl", this->consoleWidth, this->consoleHeight * 0.5);
	}
	else if (this->level == 1)
	{
		this->levels[1].load("resources/levels/two.lvl", this->consoleWidth, this->consoleHeight * 0.5);
	}
	else if (this->level == 2)
	{
		this->levels[2].load("resources/levels/three.lvl", this->consoleWidth, this->consoleHeight * 0.5);
	}
	else if (this->level == 3)
	{
		this->levels[3].load("resources/levels/four.lvl", this->consoleWidth, this->consoleHeight * 0.5);
	}
}

void Game::resetPlayer()
{
	//reset player/ball states
	player->size = PLAYER_SIZE;
	player->position = glm::vec2(this->consoleWidth/2 - PLAYER_SIZE.x/2,
		this->consoleHeight - PLAYER_SIZE.y);
	ball->reset(player->position + glm::vec2(PLAYER_SIZE.x/2 - BALL_RADIUS,
		-(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}