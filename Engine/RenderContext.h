#pragma once

class Engine;

namespace graphics
{
	class IGraphicsAPI;
	class IGraphicsContext;
	class IGraphicsDevice;

	class RenderContext
	{
	public:
		RenderContext() = default;
		RenderContext(Engine* engine, IGraphicsDevice& deivce, IGraphicsContext& ctx, IGraphicsAPI* api)
			: m_Engine(engine)
			, m_Device(&deivce)
			, m_Context(&ctx)
			, m_API(api)
		{
		};

		Engine* GetEngine() const { return m_Engine; }
		IGraphicsAPI& GetAPI() const { return *m_API; }
		IGraphicsDevice& GetDevice() const { return *m_Device; }
		IGraphicsContext& GetContext() const { return *m_Context; }

	private:
		Engine* m_Engine = nullptr;
		IGraphicsAPI* m_API = nullptr;
		IGraphicsDevice* m_Device = nullptr;
		IGraphicsContext* m_Context = nullptr;
	};
};