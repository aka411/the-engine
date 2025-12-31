#include "demo_app.h"
#include "camera.h"


void DemoApp::processInput(float deltaTime)
{
	glm::vec3 direction(0.0f);

	//Note: Keycodes are based on SDL Scancode values
	//TODO : Replace magic numbers with Engine enum for better readability

	if (m_platform.isKeyPressed(4))  direction.x -= 1.0f; // A
	if (m_platform.isKeyPressed(7))  direction.x += 1.0f; // D
	if (m_platform.isKeyPressed(26)) direction.z += 1.0f; // W
	if (m_platform.isKeyPressed(22)) direction.z -= 1.0f; // S


	if (glm::length(direction) > 0.0f)
	{
		m_camera.move(direction.x, direction.y, direction.z);
	}
}


DemoApp::DemoApp() 
{
	

	const int WINDOW_WIDTH = 1000;
	const int WINDOW_HEIGHT = 700;



	m_platform.initialize(TheEngine::RenderingAPI::OPENGL_4_6, WINDOW_WIDTH, WINDOW_HEIGHT);
	

	m_engineCore = std::make_unique<TheEngine::EngineCore>(m_platform);

	m_camera.m_viewPort = { WINDOW_WIDTH,WINDOW_HEIGHT };
	m_camera.setPerspectiveProjection(60.0f, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 1000.0f);
	
	TheEngine::EngineCore& engineCore = *m_engineCore;

	//LOAD FONTS
	engineCore.loadFont("DefaultFont", 
		R"(asset\font\inter\inter_sdf_metrics.json)",
		R"(asset\font\inter\inter_sdf_texture_atlas.png)");




	engineCore.loadFont("Bangers",
		R"(asset\font\bangers\bangers_sdf_metrics.json)",
		R"(asset\font\bangers\bangers_sdf_texture_atlas.png)");



	m_performanceMonitorSystem = std::make_unique<PerformanceMonitorSystem>(engineCore.getUICoreSystem());

	TheEngine::UI::UIElement uiWindow = engineCore.getUIBuilder()
		.createUIWindow()
		.withPosition({ 0,0,0 })
		.withRectDimensions(700, 700)
		.build();

	TheEngine::UI::UIElement panel1 = engineCore.getUIBuilder()
		.createUIPanel()
		.withColour({ 23.0 / 255.0, 23.0 / 255.0, 23.0 / 255.0,0.6 })
		.withPosition({ 10,10,0 })
		.withRectDimensions(200, 100)
		.build();

/*
	TheEngine::UI::UIElement panel2 = engineCore.getUIBuilder()
		.createUIPanel()
		.withColour({ 0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0,1.0 })
		.withPosition({ 10,130,0 })
		.withRectDimensions(200, 400)
		.build();
		*/
	TheEngine::UI::UIElement fpsGraph = engineCore.getUIBuilder().createUIGraph()
		.withColour({ 0.0,0.0,1.0,0.8 })
		.withPosition({ 0,0,1 })
		.withRectDimensions(200, 100)
		.build();


	TheEngine::UI::UIElement fpsText = engineCore.getUIBuilder().createUITextLabel()
		.withFont("Bangers")
		.withFontSize(29)
		.withColour({ 1.0,1.0,1.0,1.0 })
		.withRectDimensions(200, 70)
		.withPosition({ 0,0, 1 })
		.withText("FPS : 500")
		.build();



	m_fpsText = std::make_unique<TheEngine::UI::UIElement>(fpsText);

	//  path to the model
	const char* modelPath = R"(PATH TO GLTF FILE)";

	ECS::ECSEngine& ecsEngine = engineCore.getECSEngine();


	ECS::EntityId rootEntity = engineCore.loadModel(modelPath);



	//fpsGraph

	uiWindow.addChild(panel1);
	//uiWindow.addChild(panel2);


	panel1.addChild(fpsGraph);
	panel1.addChild(fpsText);




	PerformanceMonitorSystem& performanceMonitorSystem = *m_performanceMonitorSystem;

	performanceMonitorSystem.setUp(std::move(fpsGraph));




	
}

DemoApp::~DemoApp()
{


}

void DemoApp::onStart()
{


}

void DemoApp::onUpdate(float dt)
{


}

void DemoApp::onRender()
{


}

void DemoApp::onEvent(TheEngine::EngineEvent& event)
{

	switch (event.engineEventType)
	{
	case TheEngine::EngineEventType::WINDOW_CLOSE:
		m_isRunning = false;
		break;

	case TheEngine::EngineEventType::WINDOW_RESIZE:
	{
		auto& resize = event.windowResizeEvent;
		float aspectRatio = static_cast<float>(resize.logicalPixelWidth) /
			static_cast<float>(resize.logicalPixelHeight);

		m_camera.m_viewPort = { resize.logicalPixelWidth, resize.logicalPixelHeight };
		m_camera.setPerspectiveProjection(60.0f, aspectRatio, 0.01f, 2000.0f);
		m_engineCore->setViewportDimension(resize.physicalPixelWidth, resize.physicalPixelHeight);
		break;
	}

	case TheEngine::EngineEventType::MOUSE_MOVE:
		
		m_camera.rotate(event.mouseMoveEvent.xRel, event.mouseMoveEvent.yRel);
		break;

	default:
		break;
	}



}


void DemoApp::run()
{


	TheEngine::EngineCore& engineCore = *m_engineCore;



	m_isRunning = true;

	PerformanceMonitorSystem& performanceMonitorSystem = *m_performanceMonitorSystem;



	float deltaTime = 0.0f;

	float lastFrameTime = engineCore.getPlatform().getTimeInSeconds();;
	float currentFrameTime = lastFrameTime;
	float accumulator = 0;
	float frameNumber = 1;
	performanceMonitorSystem.UpdateFPSMeter(0);





	while (m_isRunning)
	{
		
	
		currentFrameTime = engineCore.getPlatform().getTimeInSeconds(); 
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

			TheEngine::EngineEvent event;

			while (engineCore.getPlatform().pollEvent(event))
			{
				onEvent(event);
			}
			processInput(deltaTime);
	

	

		frameNumber++;

		accumulator += deltaTime;
		if (accumulator > 1.0f / 60.0)
		{
			
			m_fpsText.get()->getComponent<TheEngine::UI::UITextComponent>()->setText("FPS : " + std::to_string(static_cast<int>(frameNumber / accumulator)));

			performanceMonitorSystem.UpdateFPSMeter(accumulator / frameNumber);
			accumulator = 0;
			frameNumber = 1;



		}



		engineCore.update(deltaTime);//involves CPU side processing mostly, animation , text generation etc

		engineCore.render(m_camera);//involves GPU calls 

		engineCore.renderUI();//involves GPU calls


		m_platform.swapBuffers();

	}





}
