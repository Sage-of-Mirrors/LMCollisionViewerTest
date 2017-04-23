#include "stdafx.h"
#include "src\imgui\imgui.h"
#include "src\imgui\imgui-sfml.h"
#include "src\util\bstream.h"
#include "src\collision\Collision.h"
 
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <cstring>
#include <iostream>

GLuint LoadShader(std::string fileName, GLenum shaderType, GLuint program) {
	GLuint address = glCreateShader(shaderType);

	std::ifstream strm(fileName);
	std::string str((std::istreambuf_iterator<char>(strm)), std::istreambuf_iterator<char>());
	const char * c_str = str.c_str();
	glShaderSource(address, 1, &c_str, NULL);
	strm.close();

	glCompileShader(address);
	glAttachShader(program, address);

	GLint compileSuccess = 0;
	glGetShaderiv(address, GL_COMPILE_STATUS, &compileSuccess);

	if (compileSuccess == 0)
	{

	}

	return address;
}
 
int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "");
    window.setVerticalSyncEnabled(true);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		//printf("Couldn't init glew\n");
		return -1;
	}

    ImGui::SFML::Init(window);
 
	GLuint _program = glCreateProgram();
	GLuint vertShaderId, fragShaderId;
	vertShaderId = LoadShader("vs.glsl", GL_VERTEX_SHADER, _program);
	fragShaderId = LoadShader("fs.glsl", GL_FRAGMENT_SHADER, _program);

	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);
	glBindAttribLocation(_program, 0, "vertexPos");
	glLinkProgram(_program);

	if (glGetError() != GL_NO_ERROR)
	{
		printf("Shader program failed!");
		return -1;
	}

	GLuint uniformMVP = glGetUniformLocation(_program, "modelview");
	GLuint uniformColor = glGetUniformLocation(_program, "col");

	Collision col("D:\\test.json");

    sf::Color bgColor;
 
    float color[3] = { 0.f, 0.f, 0.f };
 
    // let's use char array as buffer, see next part
    // for instructions on using std::string with ImGui
    char windowTitle[255] = "ImGui + SFML = <3";
 
    window.setTitle(windowTitle);
    window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
 
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
 
        ImGui::SFML::Update(window, deltaClock.restart());
 
        ImGui::Begin("Sample window"); // begin window
 
                                       // Background color edit
        if (ImGui::ColorEdit3("Background color", color)) {
            // this code gets called if color value changes, so
            // the background color is upgraded automatically!
            bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }
 
        // Window title text edit
        ImGui::InputText("Window title", windowTitle, 255);
 
        if (ImGui::Button("Update window title")) {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            window.setTitle(windowTitle);
        }
        ImGui::End(); // end window
 
        window.clear(bgColor); // fill background with color

		float ident[] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		glUniform4f(uniformColor, 1, 1, 1, 1);
		glUniformMatrix4fv(uniformMVP, 1, false, &ident[0]);

		col.Render();

		/*
        glPointSize(10.0f);
        glBegin(GL_POINTS); //starts drawing of points
            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(1.0f,1.0f,0.0f);//upper-right corner
            glVertex3f(-1.0f,-1.0f,0.0f);//lower-left corner
        glEnd();//end drawing of points
		*/

        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}