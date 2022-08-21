#include "LevelsComponent.h"

#include <fstream>
#include <iostream>
#include <memory>

#include "EnemyControllerComponent.h"
#include "EnemyStateComponent.h"
#include "EventQueue.h"
#include "GameObject.h"
#include "GameOverComponent.h"
#include "PlayerControllerComponent.h"
#include "PlayerStateComponent.h"
#include "RenderSpriteComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"


LevelsComponent::LevelsComponent(Mode mode, unsigned int width, unsigned int height, unsigned int level)
	: m_level{level}
	, m_SourcePath{"Tron/"}
	, m_Mode{ mode }
{
	m_WindowWidth = width; //- 100;
	m_WindowHeight = height; //-100;

	dae::EventQueue::GetInstance().Subscribe("KilledPlayer0", this);
	dae::EventQueue::GetInstance().Subscribe("KilledPlayer1", this);
	dae::EventQueue::GetInstance().Subscribe("KilledEnemy", this);

	PickMode();

	LoadData();
}

LevelsComponent::~LevelsComponent()
{
}

bool LevelsComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "KilledPlayer0")
	{
		PlayerAmount--;
		if (m_Mode == Mode::Versus)
		{
			PlayerAmount--;
		}
	}
	if (event->Message == "KilledPlayer1")
	{
		PlayerAmount--;
		if (m_Mode == Mode::Versus)
		{
			PlayerAmount--;
		}
	}
	if (event->Message == "KilledEnemy")
	{
		m_EnemiesKilled++;

		if (m_EnemiesKilled >= m_pEnemies.size())
		{
			m_NeedsToLoadNewLevel = true;
		}
	}

	if (PlayerAmount <= 0)
	{
		m_NeedsRestart = true;
	}

	return false;
}

void LevelsComponent::Startup()
{
	
}

void LevelsComponent::Update(float deltaSec)
{
	
	for (auto tank : m_pPlayers)
	{
		auto tankPos = tank->GetTransform().GetPosition();
		tankPos.x += tank->GetTransform().GetRect().width / 2;
		tankPos.y += tank->GetTransform().GetRect().height / 2;

		auto pos = glm::vec2{337, 318 };

		auto x = abs(tankPos.x - pos.x);
		auto y = abs(tankPos.y - pos.y);

		if (x <= 20 && y <= 20)
		{
			auto randPos = GetRandomPos();

			tank->GetTransform().SetPosition(randPos.x - tank->GetTransform().GetRect().width / 2, randPos.y - tank->GetTransform().GetRect().height / 2, 0);
		}
	}

	if (m_NeedsToLoadNewLevel)
	{
		m_ElapsedSecLoadNewLevel += deltaSec;

		if (m_ElapsedSecLoadNewLevel >= 3)
		{
			LoadNewLevel();
			m_ElapsedSecLoadNewLevel = 0.0f;
			m_NeedsToLoadNewLevel = false;
		}
	}


	if (m_NeedsRestart)
	{
		m_ElapsedSecLevelRestart += deltaSec;
		if (!m_BroadcastedRestartLevel)
		{
			dae::EventQueue::GetInstance().Broadcast(new dae::Event("RestartLevel"));
			m_BroadcastedRestartLevel = true;
		}

		if (m_ElapsedSecLevelRestart >= 3)
		{
			m_AmountOfRestarts--;

			m_ElapsedSecLevelRestart = 0;
			m_NeedsRestart = false;

			if (m_AmountOfRestarts <= 0)
			{
				GameOver();
				return;
			}
			m_BroadcastedRestartLevel = false;
			RemoveEverythingOnScene();

			PickMode();
		}
	}
}

void LevelsComponent::CreateLevel(unsigned levelIndex)
{
	m_LevelVertices.clear();
	m_pLevelIndicesWalls.clear();

	//Get the image
	std::string fullPath{ "Levels/Level" };
	fullPath += std::to_string(levelIndex) + ".png";

	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent(m_SourcePath + fullPath, m_LevelWidth, m_LevelHeight, m_WindowWidth, m_WindowHeight));
	//gObject->GetComponentOfType<RenderSpriteComponent>()->SetPosition(glm::vec2{ 500.f, 500.f });
	m_SourceToDestRatio = gObject->GetComponentOfType<RenderSpriteComponent>()->GetSourceToDestRatio();
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);

	//Parse the svg file
	fullPath = "Levels/Level" + std::to_string(levelIndex) + ".svg";
	dae::ResourceManager::GetInstance().GetVerticesFromSvgFile(m_SourcePath + fullPath, m_LevelVertices, m_SourceToDestRatio);

	//Parse the svg file
	fullPath = "Levels/Level" + std::to_string(levelIndex) + "_Walls.svg";
	dae::ResourceManager::GetInstance().GetVerticesFromSvgFile(m_SourcePath + fullPath, m_pLevelIndicesWalls, m_SourceToDestRatio);

}

glm::vec2 LevelsComponent::ToPoint2f(const std::string& pos1)
{
	glm::vec2 point;

	std::string x, y;
	int value{ 0 };

	for (unsigned int i = 0; i < pos1.size(); i++)
	{
		if (pos1[i] == ',')
		{
			i++;
			value++;
		}
		if (value < 1)
			x += pos1[i];
		else if (value < 2)
			y += pos1[i];
	}
	point.x = std::stof(x);
	point.y = std::stof(y);

	return point;
}

glm::vec2 LevelsComponent::GetRandomPos()
{
	std::ifstream file{ "SpawnPoints.txt" };

	glm::vec2 randPos;

	while (std::getline(file, m_SpawnPoints, '/'))
	{
		if (m_SpawnPoints.find("Level" + std::to_string(m_level)) != std::string::npos && "Level" + std::to_string(m_level) == ("Level" + std::to_string(m_level)))
		{
			std::string pos1;
			std::string pos2;
			int randNr{ rand() % 8 + 1 };
			int posFound = int(m_SpawnPoints.find(std::to_string(randNr) + "."));
			int value{ 0 };

			for (unsigned int i = posFound; i < m_SpawnPoints.size(); i++)
			{
				if (m_SpawnPoints[i] == '"')
				{
					i++;
					value++;
				}

				if (value == 1)
					pos1 += m_SpawnPoints[i];
				else if (value == 3)
					pos2 += m_SpawnPoints[i];
				else if (value == 4)
					break;
			}

			glm::vec2 firstPoint{ ToPoint2f(pos1) };
			glm::vec2 lastPoint{ ToPoint2f(pos2) };

			float distance{ lastPoint.x - firstPoint.x };

			randPos.x = rand() % int(distance) + firstPoint.x;
			randPos.y = firstPoint.y;
			return randPos;
		}
	}
}

void LevelsComponent::CreatePlayers(unsigned amount)
{
	for (unsigned int i = 0; i < amount; ++i)
	{
		glm::vec2 randPos = GetRandomPos();

		const auto gObject = std::make_shared<dae::GameObject>();
		gObject->AddComponent(new RenderSpriteComponent());
		gObject->AddComponent(new PlayerStateComponent(m_WindowWidth, m_WindowHeight, m_PlayerDims, m_SourceToDestRatio, i, m_Mode));
		gObject->AddComponent(new PlayerControllerComponent(&m_LevelVertices, &m_pLevelIndicesWalls, m_PlayerDims, m_SourceToDestRatio, i, randPos, m_Mode));
		dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
		m_pTanks.push_back(gObject);
		gObject->GetComponentOfType<PlayerControllerComponent>()->SetAllEnemies(&m_pTanks);
		m_pPlayers.push_back(gObject);
	}
}

void LevelsComponent::CreateEnemy()
{
	for (unsigned int i = 0; i < m_level + 2; ++i)
	{
		glm::vec2 randPos = GetRandomPos();

		const auto gObject = std::make_shared<dae::GameObject>();
		gObject->AddComponent(new RenderSpriteComponent());
		int randInt = rand() % 10;

		EnemyType type;
		//if (randInt < 8)
		//{
		//	type = EnemyType::Tank;
		//}
		//else
		{
			type = EnemyType::Recognizer;
		}

		gObject->AddComponent(new EnemyStateComponent(m_WindowWidth, m_WindowHeight, m_PlayerDims, m_SourceToDestRatio, type));

		gObject->AddComponent(new EnemyControllerComponent(&m_LevelVertices, &m_pLevelIndicesWalls, m_PlayerDims, m_SourceToDestRatio, randPos, type));
		dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
		gObject->GetComponentOfType<EnemyControllerComponent>()->SetPlayerTransform(m_pPlayers);
		gObject->GetComponentOfType<EnemyControllerComponent>()->SetAllEnemies(&m_pTanks);
		m_pEnemies.push_back(gObject);
		m_pTanks.push_back(gObject);
	}
}

void LevelsComponent::LoadData()
{
	
}

void LevelsComponent::PickMode()
{
	if (m_Mode == Mode::Singleplayer)
	{
		PlayerAmount = 1;
		CreateLevel(m_level);
		CreatePlayers(PlayerAmount);
		CreateEnemy();
	}
	else if (m_Mode == Mode::Coop)
	{
		PlayerAmount = 2;
		CreateLevel(m_level);
		CreatePlayers(PlayerAmount);
		CreateEnemy();
	}
	else if (m_Mode == Mode::Versus)
	{
		PlayerAmount = 2;
		CreateLevel(m_level);
		CreatePlayers(PlayerAmount);
	}
}

void LevelsComponent::GameOver()
{
	RemoveEverythingOnScene();

	//Make Scene
	std::string sceneName = "GameOverScreen";
	dae::Scene& gameScene = dae::SceneManager::GetInstance().CreateScene(sceneName);
	dae::SceneManager::GetInstance().SetSceneAsActive(sceneName);


	const auto gObject = std::make_shared<dae::GameObject>();
	const auto gameOverComponent = new GameOverComponent(m_Mode, m_WindowWidth, m_WindowHeight, 2);
	gObject->AddComponent(gameOverComponent);
	gameScene.Add(gObject);

	dae::SceneManager::GetInstance().RemoveScene("Game");
}

void LevelsComponent::LoadNewLevel()
{
	RemoveEverythingOnScene();
	m_level++;
	if (m_level > 3)
	{
		m_level = 1;
	}
	PickMode();
}

void LevelsComponent::RemoveEverythingOnScene()
{
	dae::EventQueue::GetInstance().Broadcast(new dae::Event("ClearAllBullets"));

	for (auto enemy : m_pEnemies)
	{
		dae::SceneManager::GetInstance().GetActiveScene()->Remove(enemy);
	}
	m_pEnemies.clear();

	for (auto tank : m_pTanks)
	{
		dae::SceneManager::GetInstance().GetActiveScene()->Remove(tank);
	}
	m_pTanks.clear();

	for (auto& player : m_pPlayers)
	{
		dae::SceneManager::GetInstance().GetActiveScene()->Remove(player);
	}

	m_pPlayers.clear();
}

