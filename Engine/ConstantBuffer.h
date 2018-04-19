#pragma once
#include <Engine/engine_shared.h>
#include <vector>
#include <string>
namespace graphics
{
	class RenderContext;
	
	class ConstantBuffer
	{
	public:
		~ConstantBuffer() 
		{
			Engine::GetAPI()->ReleasePtr(m_Buffer);
		};


		template<typename T>
		void RegisterVariable(T* variable);

		/*
			Call this function after all the variables has been registered.
		*/
		void Initiate(const char* debug_name = "unnamed");

		enum ShaderBinding : s8
		{
			VERTEX = BITFLAG(0),
			PIXEL = BITFLAG(1),
			GEOMETRY = BITFLAG(2),
			HULL = BITFLAG(3),
			DOMAINS = BITFLAG(4),
			COMPUTE = BITFLAG(5)
		};

		void Bind(s32 index, s32 shader_binding, const RenderContext& rc);
		void Bind(const s32 index[], s32 shader_binding, const RenderContext& rc);
		
	private:
		struct BufferVariable
		{
			BufferVariable() { };
			BufferVariable(void * var, s32 _size) 
				: variable(var)
				, size(_size)
			{
			}
			void* variable = nullptr;
			s32 size = 0;
		};

		IBuffer* m_Buffer = nullptr;
		std::vector<BufferVariable> m_Variables;
	};

	template<typename T>
	void graphics::ConstantBuffer::RegisterVariable(T* variable)
	{
		m_Variables.push_back(BufferVariable(variable, sizeof(T)));
	}

};