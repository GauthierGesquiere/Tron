#include "LevelsComponent.h"
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

	dae::EventQueue::GetInstance().Subscribe("RestartLevel", this);
	dae::EventQueue::GetInstance().Subscribe("KilledPlayer0", this);
	dae::EventQueue::GetInstance().Subscribe("KilledPlayer1", this);
	dae::EventQueue::GetInstance().Subscribe("KilledEnemy", this);

	PickMode();

	LoadData();
}

LevelsComponent::~LevelsComponent()
{
	/*dae::EventQueue::GetInstance().Unsubscribe("RestartLevel", this);
	dae::EventQueue::GetInstance().Unsubscribe("KilledPlayer0", this);
	dae::EventQueue::GetInstance().Unsubscribe("KilledPlayer1", this);
	dae::EventQueue::GetInstance().Unsubscribe("KilledEnemy", this);*/
}

bool LevelsComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "RestartLevel")
	{
		m_NeedsRestart = true;
	}
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

		if (m_ElapsedSecLevelRestart >= 3)
		{
			dae::EventQueue::GetInstance().Broadcast(new dae::Event("ClearAllBullets"));
			m_AmountOfRestarts--;

			m_ElapsedSecLevelRestart = 0;
			m_NeedsRestart = false;

			if (m_AmountOfRestarts <= 0)
			{
				GameOver();
				return;
			}

			RemoveEverythingOnScene();

			PickMode();
		}
	}
}

void LevelsComponent::CreateLevel(unsigned levelIndex)
{
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

void LevelsComponent::CreatePlayers(unsigned amount)
{
	for (unsigned int i = 0; i < amount; ++i)
	{
		const auto gObject = std::make_shared<dae::GameObject>();
		gObject->AddComponent(new RenderSpriteComponent());
		gObject->AddComponent(new PlayerStateComponent(m_WindowWidth, m_WindowHeight, m_PlayerDims, m_SourceToDestRatio, i));
		gObject->AddComponent(new PlayerControllerComponent(&m_LevelVertices, &m_pLevelIndicesWalls, m_PlayerDims, m_SourceToDestRatio, i));
		dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
		m_pTanks.push_back(gObject);
		gObject->GetComponentOfType<PlayerControllerComponent>()->SetAllEnemies(&m_pTanks);
		m_pPlayers.push_back(gObject);
	}
}

void LevelsComponent::CreateEnemy()
{
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent());
	int randInt = rand() % 10;

	EnemyType type;
	/*if (randInt > 8)
	{
		type = EnemyType::Tank;
	}
	else*/
	{
		type = EnemyType::Recognizer;
	}

	gObject->AddComponent(new EnemyStateComponent(m_WindowWidth, m_WindowHeight, m_PlayerDims, m_SourceToDestRatio, type));

	glm::vec2 spawnPoint{};
	spawnPoint = { 170,  356 };


	//spawnPoint = { 0, -10.f };
	gObject->AddComponent(new EnemyControllerComponent(&m_LevelVertices, &m_pLevelIndicesWalls, m_PlayerDims, m_SourceToDestRatio, spawnPoint, type));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<EnemyControllerComponent>()->SetPlayerTransform(m_pPlayers);
	gObject->GetComponentOfType<EnemyControllerComponent>()->SetAllEnemies(&m_pTanks);
	m_pEnemies.push_back(gObject);
	m_pTanks.push_back(gObject);
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
	PickMode();
}

void LevelsComponent::RemoveEverythingOnScene()
{
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

