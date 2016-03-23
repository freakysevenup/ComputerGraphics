#include "ViewPort.h"
#include "Camera3D.h"
#include "InputManager.h"
#include "ShaderNova.h"
#include "Shape.h"
#include "AssImpModelLoader.h"

class MainGame
{

public:

	MainGame() 
	{
	}

	~MainGame() {}

	void run();

private:

	void init();
	void draw();
	void gameLoop();
	void processInput();

	void setViewPort(ViewPort & view) { m_view = view; }
	void setTexture(Texture & tex) { m_shapeTexture = tex; }
	void setShaderProgram(ShaderNova & shader) { m_shader = shader; }
	void setSkyBoxShaderProgram(ShaderNova & shader) { m_skyBoxShader = shader; }

	inline glm::vec2 getGLWindowCoords(float x, float y)
	{
		float X_Coord = x;
		float Y_Coord = y;

		X_Coord -= m_width / 2.0f;
		Y_Coord -= m_height / 2.0f;

		Y_Coord *= -1;

		X_Coord /= 1000.0f;
		Y_Coord /= 1000.0f;

		return glm::vec2(X_Coord, Y_Coord);
	}

	// VARIABLES:
	int m_width = 500, m_height = 500;

	Camera3D m_cam;
	ViewPort m_view;
	ShaderNova m_shader, m_skyBoxShader;
	Shape m_shape, m_shape2, m_lightBulb, m_skyBox;
	Texture m_shapeTexture, m_bulbTex, m_skyBoxTex;
	InputManager m_input;

	glm::vec2 m_offset;

	float m_counter = 0;
};

