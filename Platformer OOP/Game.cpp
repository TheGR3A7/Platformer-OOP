#include "Game.h"
#include "Resources.h"
#include "Map.h"
#include "Character.h"
#include "Physics.h"
#include "Object.h"
#include <SFML/Audio.hpp>
#include <filesystem>

using namespace std;

Map gameMap(1.0f);
Camera camera(20.0f);
Character player;
vector<Object*> objects;

sf::Music music;
sf::Font font;
sf::Text coinsText("Coins", font);

void Begin(const sf::Window& win)
{
	for (auto& file : filesystem::directory_iterator("./Images/"))
	{
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg"))
		{
			Resources::textures[file.path().filename().string()].loadFromFile(file.path().string());
		}
	}

	for (auto& file : filesystem::directory_iterator("./Sounds/"))
	{
		if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
		{
			Resources::sounds[file.path().filename().string()].loadFromFile(file.path().string());
		}
	}

	music.openFromFile("Sounds/mainMusic.wav");
	music.setLoop(true);
	music.setVolume(25);

	font.loadFromFile("Fonts/NicoPaint-Regular.ttf");
	coinsText.setFillColor(sf::Color::Yellow);
	coinsText.setOutlineColor(sf::Color::Black);
	coinsText.setOutlineThickness(1.0f);
	coinsText.setScale(0.1f, 0.1f);

	Physics::Init();

	sf::Image image;
	image.loadFromFile("Images/map.png");
	player.position = gameMap.CreateFromImage(image, objects);
	player.Begin();

	for (auto& object : objects)
	{
		object->Begin();
	}

	music.play();
}

void Update(float deltaTime)
{
	Physics::Update(deltaTime);
	player.Update(deltaTime);
	camera.position = player.position;

	for (auto& object : objects)
	{
		object->Update(deltaTime);
	}
}

void Render(Renderer& ren)
{
 	ren.Draw(Resources::textures["background.png"], camera.position, camera.GetViewSize());

	gameMap.Draw(ren);
	player.Draw(ren);

	for (auto& object : objects)
	{
		object->Render(ren);
	}

	Physics::DebugDraw(ren);
}

void RenderUI(Renderer& ren)
{
	coinsText.setPosition(-camera.GetViewSize() / 2.0f + sf::Vector2f(2.0f, 1.0f));
	coinsText.setString("Coins: " + to_string(player.GetCoins()));
	ren.target.draw(coinsText);
}

void DeleteObject(Object* object)
{
	const auto& iter = find(objects.begin(), objects.end(), object);
	if (iter != objects.end())
	{
		delete* iter;
		objects.erase(iter);
	}
}
