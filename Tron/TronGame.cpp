#include "TronGame.h"

#include "GameObject.h"
#include <string>
#include "LevelsComponent.h"
#include "PlayerSelectComponent.h"
#include "Scene.h"

TronGame::TronGame(unsigned int width, unsigned int height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;
}

void TronGame::LoadGame()
{
	//Make Scene
	std::string sceneName = "SelectMode";
	dae::Scene& gameScene = dae::SceneManager::GetInstance().CreateScene(sceneName);
	dae::SceneManager::GetInstance().SetSceneAsActive(sceneName);


	const auto gObject = std::make_shared<dae::GameObject>();
	const auto playerSelect = new PlayerSelectComponent(m_WindowWidth, m_WindowHeight);
	//const auto levelsComponent = new LevelsComponent(m_WindowWidth, m_WindowHeight);
	gObject->AddComponent(playerSelect);
	gameScene.Add(gObject);
	
}