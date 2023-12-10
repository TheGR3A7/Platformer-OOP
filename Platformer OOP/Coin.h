#pragma once
#include <SFML/Audio.hpp>
#include "Object.h"
#include "Animation.h"
#include "Physics.h"
#include <box2d/b2_body.h>

class Coin : public Object
{
private:
	Animation animation;
	b2Body* body;

	float destroyTimer = 0.0f;
	bool isCollected = false;
public:
	~Coin();

	virtual void Begin() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Renderer& ren) override;

	void Collected();
	bool IsCollected();
};

