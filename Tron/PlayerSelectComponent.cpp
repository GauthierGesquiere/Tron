#include "PlayerSelectComponent.h"

#include "Font.h"
#include "GameObject.h"
#include "InputManager.h"
#include "NextSceneCommand.h"
#include "RenderSpriteComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SelectModeCommand.h"
#include "TextRendererComponent.h"

PlayerSelectComponent::PlayerSelectComponent(unsigned width, unsigned height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent("Tron/PlayerSelect/background.png", 640, 480, m_WindowWidth, m_WindowHeight));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);

	gObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Font> font = dae::ResourceManager::GetInstance().LoadFont("Tron/PlayerSelect/Lingua.otf", 36);
	gObject->AddComponent(new dae::TextRendererComponent("SinglePlayer", font));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<dae::TextRendererComponent>()->SetPosition(200, 200);

	gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new dae::TextRendererComponent("Coop", font));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<dae::TextRendererComponent>()->SetPosition(200, 300);

	gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new dae::TextRendererComponent("Versus", font));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	gObject->GetComponentOfType<dae::TextRendererComponent>()->SetPosition(200, 400);

	gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent("Tron/PlayerSelect/Selection.png", 8, 8, 10, 10));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_Selection = gObject;
	m_Selection->GetComponentOfType<RenderSpriteComponent>()->SetPosition({ 180, 210 });

	AddInput();
}

PlayerSelectComponent::~PlayerSelectComponent()
{

}

bool PlayerSelectComponent::OnEvent(const dae::Event* /*event*/)
{
	return false;
}

void PlayerSelectComponent::ChangeMode(Directions dir)
{
	if (dir == Directions::Down)
	{
		if (m_CurrentMode == Mode::Singleplayer)
		{
			m_CurrentMode = Mode::Coop;
			m_Selection->GetComponentOfType<RenderSpriteComponent>()->SetPosition({ 180, 310 });
		}
		else if (m_CurrentMode == Mode::Coop)
		{
			m_CurrentMode = Mode::Versus;
			m_Selection->GetComponentOfType<RenderSpriteComponent>()->SetPosition({ 180, 410 });
		}
		else if (m_CurrentMode == Mode::Versus)
		{
			m_CurrentMode = Mode::Singleplayer;
			m_Selection->GetComponentOfType<RenderSpriteComponent>()->SetPosition({ 180, 210 });
		}
	}
	else
	{
		if (m_CurrentMode == Mode::Singleplayer)
		{
			m_CurrentMode = Mode::Versus;
			m_Selection->GetComponentOfType<RenderSpriteComponent>()->SetPosition({ 180, 410 });
		}
		else if (m_CurrentMode == Mode::Coop)
		{
			m_CurrentMode = Mode::Singleplayer;
			m_Selection->GetComponentOfType<RenderSpriteComponent>()->SetPosition({ 180, 210 });
		}
		else if (m_CurrentMode == Mode::Versus)
		{
			m_CurrentMode = Mode::Coop;
			m_Selection->GetComponentOfType<RenderSpriteComponent>()->SetPosition({ 180, 310 });
		}
	}
}

void PlayerSelectComponent::Startup()
{
	Component::Startup();
}

void PlayerSelectComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);

	if (m_StartGame)
	{
		m_ElapsedSec += deltaSec;
		if (m_ElapsedSec >= 1)
		{
			m_StartGame = false;
			StartGame();
		}
	}
}

void PlayerSelectComponent::StartGame()
{
	auto& input = dae::InputManager::GetInstance();
	input.RemoveCommands();

	//Make Scene
	std::string sceneName = "Game";
	dae::Scene& gameScene = dae::SceneManager::GetInstance().CreateScene(sceneName);
	dae::SceneManager::GetInstance().SetSceneAsActive(sceneName);


	const auto gObject = std::make_shared<dae::GameObject>();
	//const auto playerSelect = new PlayerSelectComponent(m_WindowWidth, m_WindowHeight);
	const auto levelsComponent = new LevelsComponent(m_CurrentMode, m_WindowWidth, m_WindowHeight);
	gObject->AddComponent(levelsComponent);
	gameScene.Add(gObject);

	sceneName = "SelectMode";
	dae::SceneManager::GetInstance().RemoveScene(sceneName);
}

void PlayerSelectComponent::AddInput()
{
	auto& input = dae::InputManager::GetInstance();

	input.SetCommandToKey(0, SDLK_s, new SelectModeCommand(this, Directions::Down), dae::InputManager::InputState::Pressed);
	input.SetCommandToKey(0, SDLK_w, new SelectModeCommand(this, Directions::Up), dae::InputManager::InputState::Pressed);
	input.SetCommandToKey(0, SDLK_SPACE, new NextSceneCommand(this), dae::InputManager::InputState::Pressed);

	input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_DOWN, new SelectModeCommand(this, Directions::Down), dae::InputManager::InputState::Pressed);
	input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_UP, new SelectModeCommand(this, Directions::Up), dae::InputManager::InputState::Pressed);
	input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_A, new NextSceneCommand(this), dae::InputManager::InputState::Pressed);

}
