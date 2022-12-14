#pragma once
#include <string>
#include <vector>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include <memory>

#include "Component.h"
#include "EventListener.h"

enum class Mode
{
	Singleplayer,
	Coop,
	Versus
};

class LevelsComponent : public dae::Component, public dae::EventListener
{
public:
	LevelsComponent(Mode mode, unsigned int width, unsigned int height, unsigned int level = 1);
	~LevelsComponent() override;
	LevelsComponent(const LevelsComponent& other) = delete;
	LevelsComponent(LevelsComponent&& other) = delete;
	LevelsComponent& operator=(const LevelsComponent& other) = delete;
	LevelsComponent& operator=(LevelsComponent&& other) = delete;

	bool OnEvent(const dae::Event* event) override;

	void NextLevel();

private:
	void Startup() override;
	void Update(float deltaSec) override;
	void AddInput();

	void CreateLevel(unsigned int levelIndex);
	glm::vec2 ToPoint2f(const std::string& pos1);
	glm::vec2 GetRandomPos();
	void CreatePlayers(unsigned int amount);
	void CreateEnemy();
	void PickMode();
	void GameOver();
	void LoadNewLevel();

	void RemoveEverythingOnScene();
	//Dimensions
	unsigned int m_LevelWidth{ 672 };
	unsigned int m_LevelHeight{ 654 };

	unsigned int m_PlayerDims{ 40 };
	
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight{};

	unsigned int m_level{};

	bool m_NeedsRestart{};
	bool m_NeedsToLoadNewLevel{};

	float m_ElapsedSecLevelRestart{};
	float m_ElapsedSecLoadNewLevel{};

	std::string m_SourcePath{};

	std::vector<std::vector<glm::vec2>> m_LevelVertices{};
	std::vector<std::vector<glm::vec2>> m_pLevelIndicesWalls{};

	glm::vec2 m_SourceToDestRatio{};

	std::vector<std::shared_ptr<dae::GameObject>> m_pPlayers{};
	std::vector<std::shared_ptr<dae::GameObject>> m_pEnemies{};

	std::vector<std::shared_ptr<dae::GameObject>> m_pTanks{};

	Mode m_Mode{};

	int PlayerAmount{};
	int m_AmountOfRestarts{ 3 };
	int m_EnemiesKilled{};

	int m_Score{};

	std::string m_SpawnPoints{};

	bool m_BroadcastedRestartLevel{};
	bool m_NeedsInput{};

	int m_AmountOfAddInput{};
};

