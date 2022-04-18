#pragma once
#include "FE/Core/Base.hpp"

namespace FE
{
	namespace RENDERER
	{
		class Texture
		{
		public:			
			virtual void Bind(uint32_t slot = 0) =0;
			virtual void Unbind() = 0;

		private:
		};

		class Texture2D:public Texture
		{
		public:
			explicit Texture2D(const std::string& filePath);
			explicit Texture2D(uint32_t width, uint32_t height);

			~Texture2D();
			void Bind(uint32_t slot = 0);
			void Unbind() ;

			uint32_t GetRenderID() const { return RenderID; }
			static Ref<Texture2D> Create(const std::string& filePath);
			static Ref<Texture2D> Create(uint32_t width, uint32_t height) ;

		private:
			bool Bound{ false };
			uint32_t RenderID,Slot;
			uint32_t InternalFormat{ 0 };
			uint32_t Width{0}, Height{ 0 };
		};

	}
}
