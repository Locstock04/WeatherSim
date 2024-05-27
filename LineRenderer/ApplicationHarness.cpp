#include "ApplicationHarness.h"
#include "GLFWCallbacks.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Maths.h"
#include "Utilities.h"



ApplicationHarness::ApplicationHarness(Application* appInit) : app(appInit)
{
	if (!glfwInit())
	{
		return;
	}
	if (app == nullptr)
	{
		return;
	}


	//Can choose resolution here.
	window = glfwCreateWindow(1280, 720, app->GetWindowTitle(), nullptr, nullptr);

	//This is the somewhat hacky oldschool way of making callbacks work without everything having to be global. Look
	//at the way the function callbacks work to get an idea of what's going on.
	glfwSetWindowUserPointer(window, (void*)this);	

	//These functions will be able to reference the testbed object via the user pointer.
	glfwSetWindowSizeCallback(window, WindowResizeCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, MouseWheelCallback);

	if (!window)
	{
		//If the window failed to create for some reason, abandon ship.
		glfwTerminate();
		return;
	}

	//We have to tell glfw to use the OpenGL context from the window.
	glfwMakeContextCurrent(window);

	//This is where GLAD gets set up. After this point we can use openGL functions.
	if (!gladLoadGL())
	{
		return;
	}
	glfwSwapInterval(1);


	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	simpleShader = ShaderProgram("Simple.vsd", "Simple.fsd");
	simpleShader.UseShader();

	lines.Initialise();
	app->lines = &lines;

	glClearColor(0, 0, 0, 1);

	glLineWidth(3.0f);

	grid.Initialise();
	for (float i = (float)-gridLimits; i <= (float)gridLimits; i++)
	{
		Colour colour = (i == 0) ? Colour(0.8f, 0.8f, 0.8f) : Colour(0.3f, 0.3f, 0.3f);
		grid.DrawLineSegment({ i, (float) - gridLimits}, {i, (float)gridLimits}, colour);
		grid.DrawLineSegment({ (float)-gridLimits, i }, { (float)gridLimits, i }, colour);
	}
	grid.DrawLineSegment({ 0,0 }, { 1, 0 }, { 1, 0, 0 });
	grid.DrawLineSegment({ 0,0 }, { 0, 1 }, { 0, 1, 0 });
	grid.Compile();
}

ApplicationHarness::~ApplicationHarness()
{
	delete app;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void ApplicationHarness::Run()
{
	double fixedDelta = 1.0 / fixedFramerate;
	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	while (IsRunning())
	{

		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= fixedDelta)
		{
			Update((float)fixedDelta);
			accumulator -= fixedDelta;
		}

		if (IsRunning())
		{
			Render();
		}
	}
}

void ApplicationHarness::Update(float delta)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	aspectRatio = width / (float)height;

	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	
	//Because we know the camera is always orthographic (and axis-aligned) we can
	//do what would normally be a matrix inverse operation more easily with some remaps.
	float cameraLeft = cameraCentre.x - (cameraHeight * aspectRatio) / 2.0f;
	float cameraRight = cameraCentre.x + (cameraHeight * aspectRatio) / 2.0f;
	app->cursorPos.x = Remap((float)cursorX, 0, (float)width, cameraLeft, cameraRight);
	float cameraTop = cameraCentre.y + cameraHeight / 2.0f;
	float cameraBottom = cameraCentre.y - cameraHeight / 2.0f;
	app->cursorPos.y = Remap((float)cursorY, 0, (float)height, cameraTop, cameraBottom);

	
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		Vec2 cameraMovement;
		
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			cameraMovement.x--;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			cameraMovement.x++;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			cameraMovement.y++;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			cameraMovement.y--;
		}
		cameraMovement.SetMagnitude(cameraSpeed * delta * cameraHeight);
		cameraCentre += cameraMovement;
	}
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		app->leftMouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		app->rightMouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	}

	lines.Clear();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	app->Update(delta);

	lines.Compile();

	ImGui::Render();
}

void ApplicationHarness::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	float orthoMat[16];
	PopulateCameraTransform(orthoMat);
	simpleShader.SetMat4Uniform("vpMatrix", orthoMat);
	grid.Draw();
	lines.Draw();


	//Render tends to get called once or twice before Update
	//gets called, so we need to make sure this info exists.
	ImDrawData* drawData = ImGui::GetDrawData();
	if (drawData) ImGui_ImplOpenGL3_RenderDrawData(drawData);
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool ApplicationHarness::IsRunning() const
{
	return !glfwWindowShouldClose(window);
}

void ApplicationHarness::OnMouseClick(int mouseButton)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		switch (mouseButton)
		{
		case 0:
			app->OnLeftClick();
			break;
		case 1:
			app->OnRightClick();
			break;
		case 2:
			app->OnMiddleClick();
			break;
		}
	}
}

void ApplicationHarness::OnMouseRelease(int mouseButton)
{
	switch (mouseButton)
	{
	case 0:
		app->OnLeftRelease();
		break;
	case 1:
		app->OnRightRelease();
		break;
	case 2:
		app->OnMiddleRelease();
	}
}


void ApplicationHarness::Zoom(float zoomFactor)
{
	cameraHeight /= zoomFactor;
}

void ApplicationHarness::PopulateCameraTransform(float* matrix) const
{
	//Mildly hacky creation of an orthographic matrix (near/far clip at -1 to 1).
	//Not really worth setting up a Mat4 class just for this.
	memset(matrix, 0, sizeof(float) * 16);

	float cameraWidth = aspectRatio * cameraHeight;
	matrix[0] = 2.0f / cameraWidth;
	matrix[5] = 2.0f / cameraHeight;
	matrix[10] = -1.0f;
	matrix[12] = -2.0f * cameraCentre.x / cameraWidth;
	matrix[13] = -2.0f * cameraCentre.y / cameraHeight;
	matrix[15] = 1.0f;
}