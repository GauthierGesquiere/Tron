#include "EnemyStateComponent.h"

#include "GameObject.h"
#include "RenderSpriteComponent.h"

EnemyStateComponent::EnemyStateComponent(unsigned displayWidth, unsigned displayHeight, unsigned playerDims, glm::vec2 playerSize, EnemyType type)
	: StateComponent(displayWidth, displayHeight, playerDims, playerSize)
	, m_EnemyType{ type }
{
	m_SourcePath = "Tron/Enemies/";

	if (m_EnemyType == EnemyType::Tank)
	{
		m_ImageIdx = 0;
	}
	else
	{
		m_ImageIdx = 1;
	}
}

void EnemyStateComponent::Startup()
{
	StateComponent::Startup();

	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		std::string fullPath{ "DrivingHorizontal" };

		fullPath += std::to_string(m_ImageIdx) + ".png";

		renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally);
		renderer->SetFreeze(2);
	}
}

void EnemyStateComponent::Update(float deltaSec)
{
	switch (m_CurrentState)
	{
	case State::DrivingVertical:
		IsGoingUp(deltaSec);
		break;
	case State::DrivingHorizontal:
		IsGoingRight(deltaSec);
		break;
	case State::Nothing:
		break;
	default:
		break;
	}
}

void EnemyStateComponent::OnNotify(const dae::GameObject& /*actor*/, dae::Event* event)
{
	if (event->Message == "IsDrivingRight")
	{
		m_CurrentState = State::DrivingHorizontal;

		//Was walking to the Right?
		if (m_MirrorHorizontally == true)
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_MirrorHorizontally = false;
	}
	else if (event->Message == "IsDrivingLeft")
	{
		m_CurrentState = State::DrivingHorizontal;

		//Was walking to the Left?
		if ((m_MirrorHorizontally == false))
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_MirrorHorizontally = true;
	}
	else if (event->Message == "IsDrivingUp")
	{
		m_CurrentState = State::DrivingVertical;

		//Was ClimbingUp?
		if (m_MirrorVertically == true)
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_MirrorVertically = false;
	}
	else if (event->Message == "IsDrivingDown")
	{
		m_CurrentState = State::DrivingVertical;

		//Was ClimbingDown?
		if ((m_MirrorVertically == false))
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_MirrorVertically = true;
	}

	//Important
	delete event;
}

void EnemyStateComponent::IsGoingUp(float /*deltaSec*/)
{
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;

			std::string fullPath{ "DrivingVertical" };
			fullPath += std::to_string(m_ImageIdx) + ".png";

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally, m_MirrorVertically);
		}
	}
}

void EnemyStateComponent::IsGoingRight(float /*deltaSec*/)
{
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;

			std::string fullPath{ "DrivingHorizontal" };
			fullPath += std::to_string(m_ImageIdx) + ".png";

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally, m_MirrorVertically);
		}
	}
}


