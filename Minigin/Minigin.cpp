#include "MiniginPCH.h"
#include "Minigin.h"

#include <future>

#include "AudioService.h"
#include "EventQueue.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "SdlMixerAudioService.h"
#include "ServiceLocator.h"

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_WindowWidth,
		m_WindowHeight,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);

	auto* pAudioService = new SdlMixerAudioService();
	ServiceLocator::GetInstance().RegisterAudioService(pAudioService);

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	// input
	if (m_pGame)
	{
		m_pGame->LoadGame();
	}
}

void dae::Minigin::Cleanup()
{
	//Delete Game
	delete m_pGame;
	m_pGame = nullptr;

	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();
	LoadGame();

	{
		const auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& eventQueue = EventQueue::GetInstance();


		bool doContinue = true;
		float lag = 0.0f;
		auto lastTime = std::chrono::high_resolution_clock::now();

		auto audioFuture = std::async(launch::async, [&doContinue]()
			{
				const auto& audio = ServiceLocator::GetInstance().GetAudioService();
				while (doContinue)
				{
					audio->ProcessSoundRequests();
				}
			});

		while (doContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;

			doContinue = input.ProcessInput();

			while (lag >= FixedTimeStep)
			{
				sceneManager.Update(FixedTimeStep);
				lag -= FixedTimeStep;
			}
			sceneManager.Update(deltaTime);

			eventQueue.HandleEventQueue();
			if (sceneManager.GetActiveScene())
			{
				sceneManager.GetActiveScene()->DeleteRemovedObjects();
				sceneManager.GetActiveScene()->AddObjects();
			}
			renderer.Render();
		}
	}

	Cleanup();
}
