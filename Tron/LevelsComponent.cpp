#include "LevelsComponent.h"
#include <memory>

#include "EnemyControllerComponent.h"
#include "EnemyStateComponent.h"
#include "EventQueue.h"
#include "GameObject.h"
#include "PlayerControllerComponent.h"
#include "PlayerStateComponent.h"
#include "RenderSpriteComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"


LevelsComponent::LevelsComponent(unsigned int width, unsigned int height, unsigned int level)
	: m_level{level}
	, m_SourcePath{"Tron/"}
{
	m_WindowWidth = width; //- 100;
	m_WindowHeight = height; //-100;
	
	LoadLevel(m_level);
	CreatePlayers(2);
	//CreateEnemy();

	LoadData();
}

LevelsComponent::~LevelsComponent()
{
}

bool LevelsComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "RestartLevel")
	{
		m_NeedsRestart = true;
	}
	if (event->Message == "GameOver")
	{
		m_GameOver = true;
	}
	return false;
}

void LevelsComponent::Startup()
{

	dae::EventQueue::GetInstance().Subscribe("RestartLevel", this);
	dae::EventQueue::GetInstance().Subscribe("GameOver", this);
}

void LevelsComponent::Update(float deltaSec)
{
	if (m_NeedsRestart)
	{
		m_ElapsedSec += deltaSec;
		if (m_ElapsedSec >= 2.8)
		{
			dae::EventQueue::GetInstance().Broadcast(new dae::Event("ClearAllBullets"));

		}
		if (m_ElapsedSec >= 3)
		{
			m_ElapsedSec = 0;
			m_NeedsRestart = false;



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

			dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_pPlayer);
			//m_pPlayer.reset();


			CreatePlayers(1);
			//CreateEnemy();
			//CreateEnemy();
		}
	}
}

void LevelsComponent::LoadLevel(unsigned levelIndex)
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
		gObject->AddComponent(new PlayerStateComponent(m_WindowWidth, m_WindowHeight, m_PlayerDims, m_SourceToDestRatio));
		gObject->AddComponent(new PlayerControllerComponent(&m_LevelVertices, &m_pLevelIndicesWalls, m_PlayerDims, m_SourceToDestRatio, i));
		dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
		m_pTanks.push_back(gObject);
		gObject->GetComponentOfType<PlayerControllerComponent>()->SetAllEnemies(&m_pTanks);
		m_pPlayer = gObject;
	}
}

void LevelsComponent::CreateEnemy()
{
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent());
	int randInt = rand() % 10;

	EnemyType type;
	if (randInt > 8)
	{
		type = EnemyType::Tank;
	}
	else
	{
		type = EnemyType::Recognizer;
	}

	gObject->AddComponent(new EnemyStateComponent(m_WindowWidth, m_WindowHeight, m_PlayerDims, m_SourceToDestRatio, type));

	glm::vec2 spawnPoint{};
	spawnPoint = { 161,  356 };


	//spawnPoint = { 0, -10.f };
	gObject->AddComponent(new EnemyControllerComponent(&m_LevelVertices, &m_pLevelIndicesWalls, m_PlayerDims, m_SourceToDestRatio, spawnPoint));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<EnemyControllerComponent>()->SetPlayerTransform(&m_pPlayer->GetTransform());
	gObject->GetComponentOfType<EnemyControllerComponent>()->SetAllEnemies(&m_pTanks);
	m_pEnemies.push_back(gObject);
	m_pTanks.push_back(gObject);
}

void LevelsComponent::LoadData()
{
	
}

