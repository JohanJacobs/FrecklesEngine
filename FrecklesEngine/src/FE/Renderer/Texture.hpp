#pragma once
#include "FE/Core/Base.hpp"

namespace FE
{
	namespace RENDERER
	{
		class Texture
		{
		public:			
			virtual void Bind(uint32_t slot = 0) const=0;

		private:
		};

		class Texture2D:public Texture
		{
		public:
			Texture2D(const std::string& filePath);
			~Texture2D();
			void Bind(uint32_t slot = 0) const;

			static Ref<Texture2D> Create(const std::string& filePath);
		private:
			uint32_t RenderID;
			uint32_t InternalFormat{ 0 }, ImageFormat{0};
		};

	}
}
