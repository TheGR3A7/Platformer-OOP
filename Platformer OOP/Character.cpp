#define _USE_MATH_DEFINES
#include "Character.h"
#include "Resources.h"
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <cmath>


const float movementSpeed = 7.0f;
const float jumpVelocity = 10.0f;

void Character::Begin()
{
	runAnimation = Animation(0.72f,
		{
			AnimFrame(0.60f, Resources::textures["run6.png"]),
			AnimFrame(0.48f, Resources::textures["run5.png"]),
			AnimFrame(0.36f, Resources::textures["run4.png"]),
			AnimFrame(0.24f, Resources::textures["run3.png"]),
			AnimFrame(0.12f, Resources::textures["run2.png"]),
			AnimFrame(0.00f, Resources::textures["run1.png"]),
		});

	jumpSound.setBuffer(Resources::sounds["jump.wav"]);
	jumpSound.setVolume(50);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true; // ������� �������� ���������
	body = Physics::world.CreateBody(&bodyDef);


	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f; // ���������
	fixtureDef.friction = 0.0f; // ������

	b2CircleShape circleShape;
	circleShape.m_radius = 0.5f;
	circleShape.m_p.Set(0.0f, -0.5f);
	fixtureDef.shape = &circleShape;
	body->CreateFixture(&fixtureDef);

	circleShape.m_p.Set(0.0f, 0.5f);
	body->CreateFixture(&fixtureDef);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(0.5f, 0.5f);
	fixtureDef.shape = &polygonShape;// ������
	body->CreateFixture(&fixtureDef);

	polygonShape.SetAsBox(0.4f, 0.2f, b2Vec2(0.0f, 1.0f), 0.0f);
	fixtureDef.userData.pointer = (uintptr_t)this; //���������� ��������� � ������������ �������������� ���� ��� ���������� ��������� �� ������ 
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
}

void Character::Update(float deltaTime)
{
	float move = movementSpeed;

	runAnimation.Update(deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		move *= 2;

	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.x = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		velocity.x += move;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		velocity.x -= move;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isGrounded)
	{
		velocity.y = -jumpVelocity;
		jumpSound.play();
	}

	textureToDraw = runAnimation.GetTexture();

	if (velocity.x < 0.0f)
		dirLeft = true;
	else if (velocity.x > 0.0f) // else if ������ ��� ���� == 0, �� �� �� ������ ������ �������, ��� ������ else ��� �� �������
		dirLeft = false;
	else
		textureToDraw = Resources::textures["idle1.png"];


	if (!isGrounded)
		textureToDraw = Resources::textures["jump1.png"];
			

	body->SetLinearVelocity(velocity);

	position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
	angle = body->GetAngle() * (180.0f / M_PI);

}

void Character::Draw(Renderer& ren)
{
	ren.Draw(textureToDraw, position, sf::Vector2f(dirLeft ? -1.0f : 1.0f, 2.0f), angle); // ���� ���� 1 ���� ������ � 1 ���� � ������(�� ������ ����� �������� ������ �� 16)
}

void Character::OnBeginContact()
{
	isGrounded++;
}

void Character::OnEndContact()
{
	if (isGrounded > 0)
		isGrounded--;
}
