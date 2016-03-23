#include "MainGame.h"
#include <string>
#include <vector>
#include <iostream>

#define LEFT_ROTATION_PANE m_input.getMouseCoords().x < 700
#define RIGHT_ROTATION_PANE m_input.getMouseCoords().x > m_width - 700

#define TOP_ROTATION_PANE m_input.getMouseCoords().y < 200
#define BOTTOM_ROTATION_PANE m_input.getMouseCoords().y > m_height - 200

void MainGame::run()
{
	std::string title = "Computer Graphics 2";
	ViewPort m(title, m_width, m_height, 0);
	setViewPort(m);

	init();
}

void MainGame::init()
{
	//// MODEL:
	//m_model.LoadMesh("./Assets/Models/House/house.obj",
	//	"./Assets/Models/House/");
	// TEXTURE:
	Texture texture("./Assets/Textures/aluminium-texture-6102637.jpg");
	m_shapeTexture = texture;
	Texture bulbTexture("./Assets/Textures/aluminium-texture-6102637.jpg");
	m_bulbTex = bulbTexture;

	std::vector <std::string> cubeTextures;
	cubeTextures.push_back("./Assets/SkyBoxes/day_sky/front.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/day_sky/back.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/day_sky/top.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/day_sky/bottom.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/day_sky/left.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/day_sky/right.jpg");

	Texture cubeTexture;
	cubeTexture.createCubeMap(
		cubeTextures[0].c_str(),
		cubeTextures[1].c_str(),
		cubeTextures[2].c_str(),
		cubeTextures[3].c_str(),
		cubeTextures[4].c_str(),
		cubeTextures[5].c_str()
		);

	m_skyBoxTex = cubeTexture;

	// SHAPE:
	m_shape.createShape(SPHERE, 1.0f, 2.0f, 512);
	Mesh cubeMesh(m_shape.getVertices(), m_shape.getVertices().size());

	m_shape2.createShape(SPHERE, 1.0f, 2.0f, 512);
	Mesh cube2Mesh(m_shape2.getVertices(), m_shape2.getVertices().size());

	m_lightBulb.createShape(SPHERE, 0.25f, 2.0f, 16);
	Mesh bulbMesh(m_lightBulb.getVertices(), m_lightBulb.getVertices().size());

	m_skyBox.createShape(CUBE, 2000.0f, 0.0f);
	Mesh skyBoxMesh(m_skyBox.getVertices(), m_skyBox.getVertices().size());

	ShaderNova basic;

	std::vector<std::string> shaderFiles;
	shaderFiles.push_back("./Assets/Shaders/blinnPhongVert.glsl");
	shaderFiles.push_back("./Assets/Shaders/blinnPhongFrag.glsl");

	basic.compileShadersFromFile(shaderFiles);

	basic.addAttributes("position");
	basic.addAttributes("textureCoords");
	basic.addAttributes("normal");
	basic.linkShaders();

	setShaderProgram(basic);

	ShaderNova skyBox;

	std::vector<std::string> skyBoxShaderFiles;
	skyBoxShaderFiles.push_back("./Assets/Shaders/cubeShaderVertex.glsl");
	skyBoxShaderFiles.push_back("./Assets/Shaders/cubeShaderFragment.glsl");

	skyBox.compileShadersFromFile(skyBoxShaderFiles);

	skyBox.addAttributes("position");
	skyBox.addAttributes("textureCoords");
	skyBox.addAttributes("normal");
	skyBox.linkShaders();

	setSkyBoxShaderProgram(skyBox);

	// CAMERA INIT:
	m_cam.setPosition(glm::vec3(0.0f, -3.0f, 10.0f));
	m_cam.setFieldOfView(70.0f);
	m_cam.setViewportAspectRatio((float)m_width / (float)m_height);
	m_cam.setNearAndFarPlanes(1.0f, 20000.0f);

	// START GAMELOOP:
	gameLoop();
}

void MainGame::processInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;
		case SDL_MOUSEMOTION:
			m_input.setMouseCoords((float)e.motion.x, (float)e.motion.y);
			m_offset = getGLWindowCoords((float)e.motion.x, (float)e.motion.y);
			break;
		case SDL_MOUSEWHEEL:
			break;
		case SDL_KEYDOWN:
			m_input.keyPressed(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_input.keyReleased(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_input.keyPressed(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_input.keyReleased(e.button.button);
			break;
		default:
			break;
		}

		if (e.type == SDL_WINDOWEVENT) {
			switch (e.window.event) {
			case SDL_WINDOWEVENT_SHOWN:
				SDL_Log("Window %d shown", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				SDL_Log("Window %d hidden", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_Log("Window %d exposed", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_MOVED:
				SDL_Log("Window %d moved to %d,%d",
					e.window.windowID, e.window.data1,
					e.window.data2);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				SDL_Log("Window %d resized to %dx%d",
					e.window.windowID, e.window.data1,
					e.window.data2);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				SDL_Log("Window %d size changed to %dx%d",
					e.window.windowID, e.window.data1,
					e.window.data2);
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				SDL_Log("Window %d minimized", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				SDL_Log("Window %d maximized", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				SDL_Log("Window %d restored", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_ENTER:
				SDL_Log("Mouse entered window %d",
					e.window.windowID);
				break;
			case SDL_WINDOWEVENT_LEAVE:
				SDL_Log("Mouse left window %d", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				SDL_Log("Window %d gained keyboard focus",
					e.window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				SDL_Log("Window %d lost keyboard focus",
					e.window.windowID);
				break;
			case SDL_WINDOWEVENT_CLOSE:
				SDL_Log("Window %d closed", e.window.windowID);
				break;
			default:
				SDL_Log("Window %d got unknown event %d",
					e.window.windowID, e.window.event);
				break;
			}
		}
	}

	if (m_input.isKeyDown(SDLK_w))
	{
		m_cam.setPosition((m_cam.position() + m_cam.forward()));
	}
	if (m_input.isKeyDown(SDLK_s))
	{
		m_cam.setPosition((m_cam.position() - m_cam.forward()));
	}
	if (m_input.isKeyDown(SDLK_a))
	{
		m_cam.setPosition((m_cam.position() - m_cam.right()));
	}
	if (m_input.isKeyDown(SDLK_d))
	{
		m_cam.setPosition((m_cam.position() + m_cam.right()));
	}
	if (m_input.isKeyDown(SDLK_r))
	{
		m_cam.setPosition((m_cam.position() + m_cam.up()));
	}
	if (m_input.isKeyDown(SDLK_f))
	{
		m_cam.setPosition((m_cam.position() - m_cam.up()));
	}
	if (LEFT_ROTATION_PANE || RIGHT_ROTATION_PANE)
	{
		m_cam.offsetOrientation(0.0f, m_offset.x);
	}

	if (TOP_ROTATION_PANE || BOTTOM_ROTATION_PANE)
	{
		m_cam.offsetOrientation(-m_offset.y, 0.0f);
	}
}

void MainGame::draw()
{
	//////////////////////////////////////////////////////////////////////

	m_shader.startUse();

	m_shape2.setPosition(glm::vec3(30.0f, 10.0f, -40.0f));
	m_shape2.setRotation(glm::vec3(sinf(m_counter)));
	m_shape2.setScale(glm::vec3(20.0f));

	GLint camMUniform = m_shader.getUniformLocation("camera");
	glm::mat4 cameraMMatrix = m_cam.matrix();
	glUniformMatrix4fv(camMUniform, 1, GL_FALSE, &cameraMMatrix[0][0]);

	GLint modelMUniform = m_shader.getUniformLocation("model");
	glm::mat4 modelMMatrix = m_shape2.getModel();
	glUniformMatrix4fv(modelMUniform, 1, GL_FALSE, &modelMMatrix[0][0]);

	GLint normalMUniform = m_shader.getUniformLocation("normalMat");
	glm::mat4 normalMMatrix = m_shape2.getNormalMatrix();
	glUniformMatrix4fv(normalMUniform, 1, GL_FALSE, &normalMMatrix[0][0]);

	GLint lightMUniform = m_shader.getUniformLocation("lightPos");
	//glm::vec3 lightMPosition = glm::vec3(0.0f, 3.0f, -2.0f);
	glm::vec3 lightPosition = glm::vec3(
		cosf(m_counter * 1000),
		sinf(m_counter * 400),
		cosf(m_counter * 1000) + 5.0f);

	glUniform3fv(lightMUniform, 1, &lightPosition[0]);

	m_shapeTexture.bind2D(0);

	m_shape2.draw();

	m_shader.stopUse();


	///////////////////////////////////

	m_skyBoxShader.startUse();

	m_skyBoxTex.bindCube(0);

	//m_skyBox.setPosition(m_cam.position());
	//m_skyBox.setRotation(glm::vec3(0.0f));
	//m_skyBox.setScale(glm::vec3(1.0f));

	GLint camSkyBoxUniform = m_skyBoxShader.getUniformLocation("camera");
	glm::mat4 cameraSkyBoxMatrix = m_cam.matrix();
	glUniformMatrix4fv(camSkyBoxUniform, 1, GL_FALSE, &cameraSkyBoxMatrix[0][0]);

	//GLint modelSkyBoxUniform = m_shader.getUniformLocation("model");
	//glm::mat4 modelSkyBoxMatrix = m_skyBox.getModel();
	//glUniformMatrix4fv(modelSkyBoxUniform, 1, GL_FALSE, &modelSkyBoxMatrix[0][0]);

	m_skyBox.draw();

	m_skyBoxShader.stopUse();

	///////////////////////////////////


	m_shader.startUse();

	GLint camUniform = m_shader.getUniformLocation("camera");
	glm::mat4 cameraMatrix = m_cam.matrix();
	glUniformMatrix4fv(camUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	GLint modelUniform = m_shader.getUniformLocation("model");
	glm::mat4 modelMatrix = m_shape.getModel();
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &modelMatrix[0][0]);

	GLint normalUniform = m_shader.getUniformLocation("normalMat");
	glm::mat4 normalMatrix = m_shape.getNormalMatrix();
	glUniformMatrix4fv(normalUniform, 1, GL_FALSE, &normalMatrix[0][0]);

	GLint lightUniform = m_shader.getUniformLocation("lightPos");
	glUniform3fv(lightUniform, 1, &lightPosition[0]);

	m_shapeTexture.bind2D(0);

	m_shape.setPosition(glm::vec3(-30.0f, 0.0f, 20.0f));
	m_shape.setRotation(glm::vec3(cosf(m_counter)));
	m_shape.setScale(glm::vec3(10.0f));

	m_shape.draw();

	m_shader.stopUse();

	////////////////////////////////////////////////////////////////////

	m_shader.startUse();

	GLint camLUniform = m_shader.getUniformLocation("camera");
	glm::mat4 cameraLMatrix = m_cam.matrix();
	glUniformMatrix4fv(camLUniform, 1, GL_FALSE, &cameraLMatrix[0][0]);

	GLint modelLUniform = m_shader.getUniformLocation("model");
	glm::mat4 modelLMatrix = m_lightBulb.getModel();
	glUniformMatrix4fv(modelLUniform, 1, GL_FALSE, &modelLMatrix[0][0]);

	GLint normalLUniform = m_shader.getUniformLocation("normalMat");
	glm::mat4 normalLMatrix = m_lightBulb.getNormalMatrix();
	glUniformMatrix4fv(normalLUniform, 1, GL_FALSE, &normalLMatrix[0][0]);

	GLint lightLUniform = m_shader.getUniformLocation("lightPos");
	glUniform3fv(lightLUniform, 1, &lightPosition[0]);

	m_bulbTex.bind2D(0);

	m_lightBulb.setPosition(lightPosition);
	m_lightBulb.setRotation(glm::vec3(0.0f));
	m_lightBulb.setScale(glm::vec3(1.0f));

	m_lightBulb.draw();

	m_shader.stopUse();

	/////////////////////////////////////////////////////////////////////

	m_counter += 0.0001f;
}

void MainGame::gameLoop()
{
	while (true)
	{
		m_view.Update();
		m_view.swapBuffer(0.0f, 0.0f, 0.0f, 1.0f);
		m_input.update();

		draw();

		processInput();
	}

	SDL_Quit();
	exit(0);
}

