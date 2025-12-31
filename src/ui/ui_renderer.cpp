#include "ui/ui_renderer.h"
#include "glad/glad.h"
#include "ui/core/ui_component.h"
#include "components.h"




namespace TheEngine::UI
{


	UIRenderer::UIRenderer(UICoreSystem& uiCoreSystem, UISystem& uiSystem) :
		m_uiCoreSystem(uiCoreSystem),
		m_uiSystem(uiSystem)
	{

		
		glCreateVertexArrays(1, &m_dummyVAO);
		//Need valid vao binded for vertx pulling to work


		m_uiCamera.setOrthographicProjection(0, 1000, 700, 0, 10, -100);

	}

	void UIRenderer::setViewportDimension(int width, int height)
	{
		m_uiCamera.setOrthographicProjection(0, static_cast<float>(width), static_cast<float>(height), 0, 10, -100);
	}


	void UIRenderer::renderUI()
	{


		/****PER FRAME STATE****/
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);

		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		glDisable(GL_CULL_FACE);//WARNING : This is done cause in our UI 
		//system the Y is down and we do this by flipping Y Axis in camera parameters
		//But since we flipped the Y in camera matrix and when we use the projection matrix in
		//shader it causes the winding order to become CW from CCW.
		//So for now disabling culling is a temp fix.

		glEnable(GL_BLEND);
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(m_dummyVAO);//Even though we are using 
		//vertex pulling we need a dummy VAO binded for it to work
		



		/**PER FORMAT STATE***/

		GLuint uiShaderProgram = m_uiSystem.getUIShaderManager().getShaderProgramForVertexFormat(UIVertexFormat::NORMAL_UI_VERTEX_FORMAT);


		glUseProgram(uiShaderProgram);



		GLuint projectionLoc = glGetUniformLocation(uiShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &m_uiCamera.getProjectionMatrix()[0][0]);

	
		ECS::ECSEngine& ecsEngine = m_uiCoreSystem.getECSEngine();

		ECS::Query query = ecsEngine.getQuery<UIRenderMeshComponent,EngineTransformationComponent>();


		UIVertexBufferManagementSystem& uiVertexBufferManagementSystem = m_uiCoreSystem.getUIVertexBufferManagementSystem();



		for (auto& chunk : query.getChunkArrayViews())
		{
			const size_t count = chunk.getCount();
			UIRenderMeshComponent* uiRenderMeshComponentArray = chunk.getComponentArray<UIRenderMeshComponent>();
			EngineTransformationComponent* engineTransformationComponentArray = chunk.getComponentArray<EngineTransformationComponent>();

	
			for (size_t i = 0; i < count; ++i)
			{
			
				


				GLuint modelLoc = glGetUniformLocation(uiShaderProgram, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &engineTransformationComponentArray[i].worldTransformMatrix[0][0]);

				
				

				const UIVertexFormat uiVertexFormat = uiRenderMeshComponentArray[i].vertexFormat;
				const  UIBufferType uiBuffertype = uiRenderMeshComponentArray[i].bufferType;

				GPUBufferInfo vertexBufferInfo = uiVertexBufferManagementSystem.getBufferInfoForVertexFormat(uiVertexFormat, uiBuffertype);

			

				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexBufferInfo.bufferHandle);
	
				

		

			
			
				glDrawArrays(GL_TRIANGLES, uiRenderMeshComponentArray[i].vertexBufferOffset / 32, uiRenderMeshComponentArray[i].vertexCount);

	

			}


		}





		renderText();

	}




	void UIRenderer::renderText()
	{
		glDisable(GL_DEPTH_TEST);
		GLuint textVAO = m_uiSystem.getUIShaderManager().getTextVAO();
		glBindVertexArray(textVAO);
		/**PER FORMAT STATE***/
		
		GLuint textShaderProgram = m_uiSystem.getUIShaderManager().getShaderProgramForVertexFormat(UIVertexFormat::TEXT_UI_VERTEX_FORMAT);


		glUseProgram(textShaderProgram);

		

		GLuint projectionLoc = glGetUniformLocation(textShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &m_uiCamera.getProjectionMatrix()[0][0]);


		GLuint modelLoc = glGetUniformLocation(textShaderProgram, "model");

		GLuint sdfTextureHandleLoc = glGetUniformLocation(textShaderProgram, "sdfTextureHandle");






		ECS::ECSEngine& ecsEngine = m_uiCoreSystem.getECSEngine();

		ECS::Query query = ecsEngine.getQuery<UITextMeshComponent, EngineTransformationComponent>();


		UIVertexBufferManagementSystem& uiVertexBufferManagementSystem = m_uiCoreSystem.getUIVertexBufferManagementSystem();



		for (auto& chunk : query.getChunkArrayViews())
		{
			const size_t count = chunk.getCount();
			UITextMeshComponent* uiTextMeshComponentArray = chunk.getComponentArray<UITextMeshComponent>();
			UITextComponent* uiTextComponentArray = chunk.getComponentArray<UITextComponent>();
			EngineTransformationComponent* engineTransformationComponentArray = chunk.getComponentArray<EngineTransformationComponent>();


			for (size_t i = 0; i < count; ++i)
			{


				
				uint64_t handle = m_uiSystem.getUITextSystem().getUIFontManager().getFontAsset(uiTextComponentArray[i].fontName)->fontAtlasImageID;
				GLuint data[2];
				data[0] = (GLuint)(handle & 0xFFFFFFFF);         // Lower 32 bits
				data[1] = (GLuint)(handle >> 32);                // Upper 32 bits

				glUniform2uiv(sdfTextureHandleLoc, 1, data);

			
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &engineTransformationComponentArray[i].worldTransformMatrix[0][0]);




				const UIVertexFormat uiVertexFormat = uiTextMeshComponentArray[i].vertexFormat;
				const  UIBufferType uiBuffertype = uiTextMeshComponentArray[i].bufferType;

				GPUBufferInfo vertexBufferInfo = uiVertexBufferManagementSystem.getBufferInfoForTextWithVertexFormat(uiVertexFormat, uiBuffertype);



				glBindVertexBuffer(0, vertexBufferInfo.bufferHandle, 0, 36);
			





				glDrawArrays(GL_TRIANGLES, uiTextMeshComponentArray[i].vertexBufferOffset / 36, uiTextMeshComponentArray[i].vertexCount);



			}


		}




	}





}