#pragma once
#include "Enemy.h"

class Hedgehog : public Enemy
{
private:
	Animation walkAnimation;
	sf::Texture textureToDraw;
	float movement = 5.0f;
	bool dirLeft = false;

	float destroyTimer = 0.0f;
	bool isDead = false;

	FixtureData fixtureData;
	b2Body* body;

	sf::Clock sleepTimer;
	bool isSleeping = false;
	float sleepDuration = 2.0f;
	sf::Clock walkTimer;
	float walkDuration = 4.0f;

public:
	virtual void Begin() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Renderer& ren) override;

	virtual void Die() override;
	virtual bool IsDead() override;

	bool IsSleeping();
	void Sleep();
	void WakeUp();
};

