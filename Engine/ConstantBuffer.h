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

		enum ShaderBinding : int8
		{
			VERTEX = BIT(0),
			PIXEL = BIT(1),
			GEOMETRY = BIT(2),
			HULL = BIT(3),
			DOMAINS = BIT(4),
			COMPUTE = BIT(5)
		};

		void Bind(int32 index, int32 shader_binding, const RenderContext& rc);
		void Bind(const int32 index[], int32 shader_binding, const RenderContext& rc);
		
	private:
		struct BufferVariable
		{
			BufferVariable() { };
			BufferVariable(void * var, int32 _size) 
				: variable(var)
				, size(_size)
			{
			}
			void* variable = nullptr;
			int32 size = 0;
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