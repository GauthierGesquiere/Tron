#include "EnemyStateComponent.h"

#include "GameObject.h"
#include "RenderSpriteComponent.h"

EnemyStateComponent::EnemyStateComponent(unsigned displayWidth, unsigned displayHeight, unsigned playerDims, glm::vec2 playerSize, EnemyType type)
	: StateComponent(displayWidth, displayHeight, playerDims, playerSize)
	, m_EnemyType{ type }
{
	m_SourcePath = "Tron/Enemies/";
}

void EnemyStateComponent::Startup()
{
	StateComponent::Startup();

	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		renderer->SetTextureToDraw(m_SourcePath + "DrivingHorizontal.png", m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally);
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
	case State::Dying:
		IsDying(deltaSec);
		break;
	case State::Nothing:
		break;
	default:
		break;
	}
}

void EnemyStateComponent::OnNotify(const dae::GameObject& /*actor*/, dae::Event* event)
{
	if (event->Message == "IsWalkingLeft")
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
	else if (event->Message == "IsWalkingRight")
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
	else if (event->Message == "IsClimbingDown")
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
	else if (event->Message == "IsClimbingUp")
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
			const std::string fullPath{ "DrivingVertical.png" };

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
			const std::string fullPath{ "DrivingHorizontal.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally, m_MirrorVertically);
		}
	}
}

void EnemyStateComponent::IsDying(float /*deltaSec*/)
{
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			const std::string fullPath{ "Death.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop);
		}
	}
}

