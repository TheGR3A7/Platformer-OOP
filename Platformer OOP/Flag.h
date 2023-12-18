#pragma once
#include "Object.h"
#include "Animation.h"
#include "Physics.h"
#include <box2d/b2_body.h>

class Flag : public Object
{
private:
	Animation activeAnimation;

	FixtureData fixtureData;
	b2Body* body;

public:
	virtual void Begin() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Renderer& ren) override;
};
