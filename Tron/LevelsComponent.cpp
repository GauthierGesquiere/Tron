#include "LevelsComponent.h"
#include <memory>

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
	CreatePlayers(1);

	LoadData();
}

LevelsComponent::~LevelsComponent()
{
}

bool LevelsComponent::OnEvent(const dae::Event* event)
{
	return false;
}

void LevelsComponent::Startup()
{

}

void LevelsComponent::Update(float deltaSec)
{
	
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
		gObject->AddComponent(new PlayerControllerComponent(&m_LevelVertices, &m_pLevelIndicesWalls, m_PlayerDims, m_SourceToDestRatio));
		dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
		m_pPlayer = gObject;
	}
}

void LevelsComponent::CreateEnemy()
{
	
}

void LevelsComponent::LoadData()
{
	
}

