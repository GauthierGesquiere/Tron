#include "ArmComponent.h"

#include <iostream>
#include <memory>
#include <utility>

#include "GameObject.h"
#include "RenderSpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"

ArmComponent::ArmComponent(unsigned int Dims, glm::vec2 Size, float* degrees, const MoveDirections* moveDirections)
	: m_SourcePath{ "Tron/" }
	, m_Offset{0, 0, 0}
	, m_Dims{Dims}
	, m_Size{Size}
	, m_Degrees{degrees}
	, m_MoveDirection{ moveDirections }
{
}

void ArmComponent::TankIsKilled()
{
	dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_RenderObj);
}

void ArmComponent::Startup()
{
	Component::Startup();

	const std::string fullPath{ m_SourcePath + "Player/Arm0Degrees.png" };

	//Get the image
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new RenderSpriteComponent(fullPath, m_Dims, m_Dims, m_Dims * static_cast<unsigned int>(m_Size.x), m_Dims * static_cast<unsigned int>(m_Size.y)));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_RenderObj = gObject;
}

void ArmComponent::Update(float deltaSec)
{
	Component::Update(deltaSec);


	std::string fullPath;

	int modifier{};

	const int temp = static_cast<int>(floor(*m_Degrees));

	if (temp < 90 || temp == 360)
	{
		modifier = 0;
		m_FlipVertically = false;
		m_FlipHorizontally = false;

		if (*m_MoveDirection == MoveDirections::Down)
		{
			m_Offset.y = -15;
			m_Offset.x = 9;
		}
		if (*m_MoveDirection == MoveDirections::Up)
		{
			m_Offset.y = -5;
			m_Offset.x = 9;
		}
		if (*m_MoveDirection == MoveDirections::Left)
		{
			m_Offset.y = -10;
			m_Offset.x = 14;
		}
		if (*m_MoveDirection == MoveDirections::Right)
		{
			m_Offset.y = -10;
			m_Offset.x = 5;
		}
	}
	else if (temp < 180)
	{
		modifier = 90;
		m_FlipHorizontally = false;
		m_FlipVertically = true;

		if (*m_MoveDirection == MoveDirections::Down)
		{
			m_Offset.y = -15;
			m_Offset.x = -9;
		}
		if (*m_MoveDirection == MoveDirections::Up)
		{
			m_Offset.y = -5;
			m_Offset.x = -9;
		}
		if (*m_MoveDirection == MoveDirections::Left)
		{
			m_Offset.y = -10;
			m_Offset.x = -5;
		}
		if (*m_MoveDirection == MoveDirections::Right)
		{
			m_Offset.y = -10;
			m_Offset.x = -14;
		}
	}
	else if (temp < 270)
	{
		modifier = 0;
		m_FlipHorizontally = true;
		m_FlipVertically = true;

		if (*m_MoveDirection == MoveDirections::Down)
		{
			m_Offset.y = 5;
			m_Offset.x = -9;
		}
		if (*m_MoveDirection == MoveDirections::Up)
		{
			m_Offset.y = 15;
			m_Offset.x = -9;
		}
		if (*m_MoveDirection == MoveDirections::Left)
		{
			m_Offset.y = 10;
			m_Offset.x = -5;
		}
		if (*m_MoveDirection == MoveDirections::Right)
		{
			m_Offset.y = 10;
			m_Offset.x = -14;
		}
	}
	else if (temp < 360)
	{
		modifier = 90;
		m_FlipHorizontally = true;
		m_FlipVertically = false;

		if (*m_MoveDirection == MoveDirections::Down)
		{
			m_Offset.y = 5;
			m_Offset.x = 9;
		}
		if (*m_MoveDirection == MoveDirections::Up)
		{
			m_Offset.y = 15;
			m_Offset.x = 9;
		}
		if (*m_MoveDirection == MoveDirections::Left)
		{
			m_Offset.y = 10;
			m_Offset.x = 14;
		}
		if (*m_MoveDirection == MoveDirections::Right)
		{
			m_Offset.y = 10;
			m_Offset.x = 5;
		}
	}

	switch (abs(modifier - (temp % 90)))
	{
	case 0:
		fullPath = m_SourcePath + "Player/Arm0Degrees.png";
		break;
	case 5:
	case 10:
		fullPath = m_SourcePath + "Player/Arm10Degrees.png";
		break;
	case 15:
	case 20:
		fullPath = m_SourcePath + "Player/Arm20Degrees.png";
		break;
	case 25:
	case 30:
		fullPath = m_SourcePath + "Player/Arm30Degrees.png";
		break;
	case 35:
	case 40:
		fullPath = m_SourcePath + "Player/Arm40Degrees.png";
		break;
	case 45:
	case 50:
		fullPath = m_SourcePath + "Player/Arm50Degrees.png";
		break;
	case 55:
	case 60:
		fullPath = m_SourcePath + "Player/Arm60Degrees.png";
		break;
	case 65:
	case 70:
		fullPath = m_SourcePath + "Player/Arm70Degrees.png";
		break;
	case 75:
	case 80:
		fullPath = m_SourcePath + "Player/Arm80Degrees.png";
		break;
	case 85:
	case 90:
		fullPath = m_SourcePath + "Player/Arm90Degrees.png";
		break;
	default: 
		fullPath = m_SourcePath + "Player/Arm90Degrees.png";
		break;
	}

	m_RenderObj->GetComponentOfType<RenderSpriteComponent>()->SetTextureToDraw(fullPath, m_Dims, m_Dims, 0, 0, 0, LoopType::NoLoop, -1, m_FlipVertically, m_FlipHorizontally);


	m_RenderObj->GetTransform().SetPosition(m_pOwner->GetTransform().GetPosition() + m_Offset);
}
