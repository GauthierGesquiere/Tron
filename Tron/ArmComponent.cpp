#include "ArmComponent.h"

#include <iostream>
#include <memory>
#include <utility>

#include "GameObject.h"
#include "RenderSpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"

ArmComponent::ArmComponent(unsigned int Dims, glm::vec2 Size, int* degrees, const MoveDirections* moveDirections)
	: m_SourcePath{ "Tron/" }
	, m_Offset{0, 0, 0}
	, m_Dims{Dims}
	, m_Size{Size}
	, m_Degrees{degrees}
	, m_MoveDirection{ moveDirections }
{
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

	/*if (*m_MoveDirection == MoveDirections::Down)
	{
		m_Offset.y = -11;
		m_Offset.x = 10;
	}
	if (*m_MoveDirection == MoveDirections::Up)
	{
		m_Offset.y = -5;
		m_Offset.x = 10;
	}
	if (*m_MoveDirection == MoveDirections::Left)
	{
		m_Offset.y = -8;
		m_Offset.x = 14;
	}
	if (*m_MoveDirection == MoveDirections::Right)
	{
		m_Offset.y = -8;
		m_Offset.x = 8;
	}*/

	std::string fullPath{};

	int modifier{};

	if (*m_Degrees < 90 || *m_Degrees == 360)
	{
		modifier = 0;
		m_FlipVertically = false;
		m_FlipHorizontally = false;
	}
	else if (*m_Degrees < 180)
	{
		modifier = 90;
		m_FlipHorizontally = false;
		m_FlipVertically = true;
	}
	else if (*m_Degrees < 270)
	{
		modifier = 0;
		m_FlipHorizontally = true;
		m_FlipVertically = true;
	}
	else if (*m_Degrees < 360)
	{
		modifier = 90;
		m_FlipHorizontally = true;
		m_FlipVertically = false;
	}

	//std::cout << abs(modifier - (*m_Degrees % 90)) << std::endl;

	switch (abs(modifier - (*m_Degrees % 90)))
	{
	case 0:
		fullPath = m_SourcePath + "Player/Arm0Degrees.png";
		break;
	case 10:
		fullPath = m_SourcePath + "Player/Arm10Degrees.png";
		break;
	case 20:
		fullPath = m_SourcePath + "Player/Arm20Degrees.png";
		break;
	case 30:
		fullPath = m_SourcePath + "Player/Arm30Degrees.png";
		break;
	case 40:
		fullPath = m_SourcePath + "Player/Arm40Degrees.png";
		break;
	case 50:
		fullPath = m_SourcePath + "Player/Arm50Degrees.png";
		break;
	case 60:
		fullPath = m_SourcePath + "Player/Arm60Degrees.png";
		break;
	case 70:
		fullPath = m_SourcePath + "Player/Arm70Degrees.png";
		break;
	case 80:
		fullPath = m_SourcePath + "Player/Arm80Degrees.png";
		break;
	case 90:
		fullPath = m_SourcePath + "Player/Arm90Degrees.png";
		break;
	}

	m_RenderObj->GetComponentOfType<RenderSpriteComponent>()->SetTextureToDraw(fullPath, m_Dims, m_Dims, 0, 0, 0, LoopType::NoLoop, -1, m_FlipVertically, m_FlipHorizontally);


	m_RenderObj->GetTransform().SetPosition(m_pOwner->GetTransform().GetPosition() + m_Offset);
	

	/*if (m_Degrees <= 10)
	{
		if (const auto renderer = m_RenderObj->GetComponentOfType<RenderSpriteComponent>())
		{
			const std::string fullPath{ "LeftRight.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_Dims, m_Dims, 0.5f, m_Dims * static_cast<unsigned int>(m_Size.x), m_Dims * static_cast<unsigned int>(m_Size.y), LoopType::ForwardReverseLoop, -1, m_FlipHorizontally, m_FlipVertically);
		}
	}*/
}
