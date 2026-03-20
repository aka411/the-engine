#pragma once
#include "rendering-system/renderer/render-pass/i_render_pass.h"


namespace TheEngine::RenderingSystem
{

	class GBufferRenderPass : public IRenderPass
	{


	private:


		//Inputs : None

		/*emissive?
		//OutPuts :
		// G-Buffer Textures 
		TextureHandle m_albedoBuffer;      // Format: R8G8B8A8_UNORM
		TextureHandle m_normalBuffer;      // Format: R16G16B16A16_FLOAT (Higher precision for normals)
		TextureHandle m_positionBuffer;    ?
		TextureHandle m_pbrDataBuffer;     // Metallic (R), Roughness (G), AO (B)
		TextureHandle m_depthStencilBuffer; // Format: D32_FLOAT_S8X24_UINT
		*/
	

	public:

		GBufferRenderPass(RenderingContext& renderingContext);


		virtual void excecute(const DrawCallBucket& drawCallbucket) override;



	};



}