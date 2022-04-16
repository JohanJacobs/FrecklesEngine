#pragma once
#include "FE/CORE/Base.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace FE
{
	namespace RENDERER
	{		
		class OrthographicCamera
		{
		public:
			OrthographicCamera()=default;
			~OrthographicCamera()=default;
			
			void SetAspectRatio(float aspectRatio = 16/9.0f) 
			{ 
				AspectRatio = aspectRatio; 
				CalcualteProjection(); 
			}

			void SetClipSpace(float size = 1.0f, float nearClip = 0.01f, float farClip=100.0f)
			{
				Size = size;
				NearClip = nearClip;
				FarClip = farClip;
				CalcualteProjection();
			}
			
			void SetSize(float size) { Size = size; }

			const glm::mat4& GetProjection() const 
			{ 
				return Projection; 
			}

		private:
			void CalcualteProjection()
			{
				auto left = -Size * AspectRatio * 0.5f;
				auto right = Size * AspectRatio * 0.5f;
				auto bottom = -Size * 0.5f;
				auto top = Size * 0.5f;

				Projection = glm::ortho(left, right, bottom, top, NearClip, FarClip);
			}
			
		private:
			glm::mat4 Projection;
			float NearClip, FarClip;
			float Size;
			float AspectRatio{16/9}; //default
		};
	}
}
