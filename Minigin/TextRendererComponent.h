#pragma once
#include "Component.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextRendererComponent final : public dae::Component
	{
	public:
		explicit TextRendererComponent(const std::string& text, const std::shared_ptr<Font>& font);
		virtual ~TextRendererComponent() = default;
		TextRendererComponent(const TextRendererComponent& other) = delete;
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(const TextRendererComponent& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& other) = delete;

		void SetPosition(float x, float y);
	private:
		void Startup() override;
		void Update(float deltaSec) override;
		void Render() const override;

		void SetText(const std::string& text);

		bool m_NeedsUpdate;
		std::string m_Text;
		Transform m_Transform;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
	};
}
