#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/b2_body.h>
#include "Renderer.h"
#include "Physics.h"
#include "Animation.h"


class Character : public ContactListener
{
private:
	Animation runAnimation;
	sf::Texture textureToDraw;
	sf::Sound jumpSound;

	b2Body* body;
	size_t isGrounded = 0; // bool ����� �������
	bool dirLeft = false;
public:
	sf::Vector2f position;
	float angle;

	void Begin();
	void Update(float deltaTime);
	void Draw(Renderer& ren);

	// ������������ ����� ContactListener
	virtual void OnBeginContact() override;
	virtual void OnEndContact() override;
};