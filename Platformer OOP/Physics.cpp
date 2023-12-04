#include "Physics.h"
#include <SFML/Graphics.hpp>
#include <box2d/b2_draw.h>

b2World Physics::world{b2Vec2(0.0f, 9.8f)};
MyDebugDraw* Physics::debugDraw{nullptr};

class MyDebugDraw : public b2Draw
{
private:
	sf::RenderTarget& target;
public:
	MyDebugDraw(sf::RenderTarget& target) : target(target)
	{
	}

	// ������������ ����� b2Draw
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		sf::ConvexShape shape(vertexCount);
		for (size_t i = 0; i < vertexCount; i++)
		{
			shape.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
		}

		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(0.02f);
		shape.setOutlineColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		target.draw(shape);
	}

	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		sf::ConvexShape shape(vertexCount);
		for (size_t i = 0; i < vertexCount; i++)
		{
			shape.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
		}
		shape.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		target.draw(shape);
	}

	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override
	{
		sf::CircleShape circle(radius);
		circle.setPosition(center.x, center.y);
		circle.setOrigin(radius / 2.0f, radius / 2.0f);
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineThickness(0.02f);
		circle.setOutlineColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		target.draw(circle);
	}

	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override
	{
		sf::CircleShape circle(radius);
		circle.setPosition(center.x, center.y);
		circle.setOrigin(radius / 2.0f, radius / 2.0f);
		circle.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 80)); // 80 -����� ��� ����� ����������
		target.draw(circle);

		b2Vec2 p = center + (radius * axis);
		DrawSegment(center, p, color);
	}

	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
	{
		sf::VertexArray va(sf::Lines, 2);
		sf::Color sfColor(color.r * 255, color.g * 255, color.b * 255, color.a * 255);

		va[0].position = sf::Vector2f(p1.x, p1.y);
		va[0].color = sfColor;

		va[1].position = sf::Vector2f(p2.x, p2.y);
		va[1].color = sfColor;

		target.draw(va);
	}

	virtual void DrawTransform(const b2Transform& xf) override
	{
		b2Vec2 p = xf.p; // = ����������� ������� ��������������
		b2Vec2 px = p + (0.5f * xf.q.GetXAxis()); // q - ��������
		b2Vec2 p� = p + (0.5f * xf.q.GetYAxis()); // q - ��������

		DrawSegment(p, px, b2Color(1.0f, 0.0f, 0.0f)); // �������
		DrawSegment(p, p�, b2Color(0.0f, 1.0f, 0.0f)); // �������
	}

	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override
	{
		sf::CircleShape circle(size);
		circle.setPosition(p.x, p.y);
		circle.setOrigin(size / 2.0f, size / 2.0f);
		circle.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		target.draw(circle);
	}

};

void Physics::Init()
{

}

void Physics::Update(float deltaTime)
{
	world.Step(deltaTime, 6, 2);
}

void Physics::DebugDraw(Renderer& ren)
{
	if (!debugDraw) // ����� nullptr(�.� ������� ���������� ���)
	{
		debugDraw = new MyDebugDraw(ren.target);
		//debugDraw->SetFlags(b2Draw::e_aabbBit); // �������� ������� �������
		//debugDraw->SetFlags(b2Draw::e_shapeBit); // �������� ������ �������
		debugDraw->SetFlags(b2Draw::e_centerOfMassBit); // �������� ���� ���� �������
		world.SetDebugDraw(debugDraw);
	}

	world.DebugDraw();
}