#include "../../include/engine/engine_core.h"
#include <utils/file-decoders/image_decoder.h>


namespace TheEngine
{

	EngineCore::EngineCore(Platform& platform) :
		m_platform(platform),
		m_ecsEngine(m_nullFatalErrorHandler),
		m_gpuBufferManager(),
		m_gpuTextureManager(),

		m_gpuMaterialSystem(m_gpuBufferManager),
		m_worldVertexBufferManagementSystem(m_gpuBufferManager),
		m_animationSystem(m_ecsEngine, m_gpuBufferManager),

		m_engineLoader(m_ecsEngine, m_gpuTextureManager,
		m_gpuMaterialSystem, m_worldVertexBufferManagementSystem, m_animationSystem),

		m_transformationSystem(m_ecsEngine),

		
		m_uiCoreSystem(m_ecsEngine, m_gpuBufferManager, m_gpuTextureManager),
		m_uiSystem(m_uiCoreSystem),
		m_renderSystem(m_ecsEngine, m_worldVertexBufferManagementSystem, m_gpuMaterialSystem, m_uiCoreSystem, m_uiSystem, m_animationSystem, m_gpuBufferManager),
		m_uiBuilder(m_uiCoreSystem)
	


	{

		m_ecsEngine.registerComponent<EngineRenderComponent>();
		m_ecsEngine.registerComponent<EngineFatRenderComponent>();
		m_ecsEngine.registerComponent<EngineChildrenComponent>();
		m_ecsEngine.registerComponent<EngineTransformationComponent>();

		m_ecsEngine.registerComponent<AnimationComponent>();
		m_ecsEngine.registerComponent<BoneAnimationComponent>();
		m_ecsEngine.registerComponent<BoneJointMatrixIdComponent>();
		m_ecsEngine.registerComponent<AnimationStateComponent>();

		m_ecsEngine.registerComponent<RootEntityComponent>();






	}


	EngineCore::~EngineCore()
	{


	}

	Platform& EngineCore::getPlatform()
	{
		return m_platform;
	}

	void EngineCore::setViewportDimension(const int physicalWidth,const int physicalHeight,const int logicalWidth,const int logicalHeight)
	{
		m_renderSystem.setViewportDimension(physicalWidth, physicalHeight);
		m_renderSystem.setViewportDimensionForUI(logicalWidth, logicalHeight);

	}



	ECS::ECSEngine& EngineCore::getECSEngine()
	{

		return m_ecsEngine;
	}

	ECS::EntityId EngineCore::loadModel(std::string pathToFile)
	{

		ECS::EntityId rootEntityId = m_engineLoader.createRootEntity(pathToFile);


		return rootEntityId;
	}




	void EngineCore::update(const float deltaTime)
	{
		//need to look into how to order properly though
		m_transformationSystem.updateTransformationsForAllEntities();
		m_uiSystem.update();
		m_animationSystem.animateAll(deltaTime);

	}

	void EngineCore::render(TheEngine::Camera camera)
	{

		m_renderSystem.render(camera);

	}


	void EngineCore::renderUI()
	{
		m_renderSystem.renderUI();

	}

	/***********UI SYSTEM********************/

		/***** UI SYSTEM *****/
	UI::UIBuilder& EngineCore::getUIBuilder()
	{
		return m_uiBuilder;
	}



	UI::UICoreSystem& EngineCore::getUICoreSystem()
	{

		return m_uiCoreSystem;
	}


	void EngineCore::loadFont(const std::string& fontName, const std::string& fontJsonFilePath, const std::string& fontAtlasPath)
	{

		TheEngine::Utils::ImageDecoder imageDecoder(m_platform);
		Utils::ImageData fontAtlasImageData = imageDecoder.decodeImageFromFile(fontAtlasPath);
		TextureCreateInfo textureCreateInfo;
		textureCreateInfo.type = TextureType::TEXTURE_2D;
		textureCreateInfo.internalFormat = TextureInternalFormat::R8;
		textureCreateInfo.textureSourcePixelFormat = TextureSourcePixelFormat::R;
		textureCreateInfo.textureSourceComponentType = TextureSourceComponentType::UNSIGNED_BYTE;
		textureCreateInfo.width = static_cast<uint32_t>(fontAtlasImageData.width);
		textureCreateInfo.height = static_cast<uint32_t>(fontAtlasImageData.height);

		//confirm the destructor of fontAtlasImageData will free the data after this function
		textureCreateInfo.data = fontAtlasImageData.data.get();


		
		TextureInfo fontAtlasTextureInfo = m_gpuTextureManager.createNewTexture(textureCreateInfo);



		FileData jsonData = m_platform.readFile(fontJsonFilePath);
		
		m_uiSystem.getUITextSystem().getUIFontManager().loadFont(fontName, jsonData, fontAtlasTextureInfo.resisdentHandle);



	}



	GPUMemoryUsageStats EngineCore::getGPUMemoryUsageStats() const
	{
		GPUMemoryUsageStats gpuMemoryUsageStats;
		gpuMemoryUsageStats.gpuBufferMemoryUsage = m_gpuBufferManager.getTotalAllocatedGPUMemory();
		gpuMemoryUsageStats.gpuTextureMemoryUsage = m_gpuTextureManager.getTotalAllocatedTextureMemory();
		
		return gpuMemoryUsageStats;
	}


}