#include "Map.h"
#include "Resources.h"
#include "Physics.h"
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>

Map::Map(float cellSize) : cellSize(cellSize), grid()
{
}


void Map::CreateChekerboard(size_t width, size_t height)
{
	grid = vector(width, vector(height, 1));

	bool last = 0;

	for (auto& column : grid)
	{
		for (auto& cell : column)
		{
			last = cell = !last;
		}
		if (width % 2 == 0)
			last = !last;
	}
}

sf::Vector2f Map::CreateFromImage(const sf::Image& image)
{
	grid.clear();
	grid = vector(image.getSize().x, vector(image.getSize().y, 0));

	sf::Vector2f characterPosition;

	for (size_t x = 0; x < grid.size(); x++)
	{
		for (size_t y = 0; y < grid[x].size(); y++) 
		{
			sf::Color color = image.getPixel(x, y);
			if (color == sf::Color::Black)
			{
				grid[x][y] = 1;
				b2BodyDef bodyDef;
				bodyDef.position.Set(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				b2PolygonShape shape;
				shape.SetAsBox(cellSize / 2.0f, cellSize / 2.0f);
				body->CreateFixture(&shape, 0.0f);
			}
			else if (color == sf::Color::Red)
				characterPosition = sf::Vector2f(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f);
		}
	}

	return characterPosition;
}

void Map::Draw(Renderer& ren)
{
	int x = 0;
	for (const auto& column : grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				ren.Draw(Resources::textures["brick2.png"], sf::Vector2f(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f), sf::Vector2f(cellSize, cellSize));
			}
			y++;
		}
		x++;
	}
}