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

private:
	void Startup() override;
	void Update(float deltaSec) override;

	void LoadLevel(unsigned int levelIndex);
	void CreatePlayers(unsigned int amount);
	void CreateEnemy();
	void LoadData();

	//Dimensions
	unsigned int m_LevelWidth{ 672 };
	unsigned int m_LevelHeight{ 654 };

	unsigned int m_PlayerDims{ 40 };
	
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight{};

	unsigned int m_level{};

	bool m_NeedsRestart{};
	bool m_GameOver{};

	float m_ElapsedSec{};

	std::string m_SourcePath{};

	std::vector<std::vector<glm::vec2>> m_LevelVertices{};
	std::vector<std::vector<glm::vec2>> m_pLevelIndicesWalls{};

	glm::vec2 m_SourceToDestRatio{};

	std::shared_ptr<dae::GameObject> m_pPlayer{};
	std::vector<std::shared_ptr<dae::GameObject>> m_pEnemies{};

	std::vector<std::shared_ptr<dae::GameObject>> m_pTanks{};

	Mode m_Mode{};
};

