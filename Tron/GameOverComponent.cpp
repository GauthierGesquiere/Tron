#include "GameOverComponent.h"

#include <fstream>
#include <iostream>

#include "GameObject.h"
#include "RenderSpriteComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextRendererComponent.h"

GameOverComponent::GameOverComponent(Mode mode, unsigned width, unsigned height, int score)
{
	auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent("Tron/PlayerSelect/background.png", 640, 480, width, height));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);

	gObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Font> font = dae::ResourceManager::GetInstance().LoadFont("Tron/PlayerSelect/Lingua.otf", 36);
	gObject->AddComponent(new dae::TextRendererComponent("Game Over", font));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<dae::TextRendererComponent>()->SetPosition(200, 200);

	if (mode == Mode::Singleplayer)
	{
		gObject = std::make_shared<dae::GameObject>();
		font = dae::ResourceManager::GetInstance().LoadFont("Tron/PlayerSelect/Lingua.otf", 36);
		gObject->AddComponent(new dae::TextRendererComponent("Singleplayer", font));
	}
	else if (mode == Mode::Coop)
	{
		gObject = std::make_shared<dae::GameObject>();
		font = dae::ResourceManager::GetInstance().LoadFont("Tron/PlayerSelect/Lingua.otf", 36);
		gObject->AddComponent(new dae::TextRendererComponent("Coop", font));

	}
	else if (mode == Mode::Versus)
	{
		gObject = std::make_shared<dae::GameObject>();
		font = dae::ResourceManager::GetInstance().LoadFont("Tron/PlayerSelect/Lingua.otf", 36);
		gObject->AddComponent(new dae::TextRendererComponent("Versus", font));
	}

	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<dae::TextRendererComponent>()->SetPosition(200, 300);


	gObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Tron/PlayerSelect/Lingua.otf", 36);

	std::ifstream myfilein("HighScore.txt");
	std::string highScore = "0";
	if (myfilein)
	{
		std::getline(myfilein, highScore);
	}

	std::ofstream myfileout;
	myfileout.open("HighScore.txt");
	if (std::stoi(highScore) < score)
	{
		myfileout << score;
	}
	myfileout.close();
	
	std::string s = std::to_string(score);

	gObject->AddComponent(new dae::TextRendererComponent(s, font));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<dae::TextRendererComponent>()->SetPosition(200, 400);

	gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new dae::TextRendererComponent(highScore, font));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<dae::TextRendererComponent>()->SetPosition(200, 500);
}
