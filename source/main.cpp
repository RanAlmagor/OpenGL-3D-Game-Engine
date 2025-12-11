#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

int main()
{
	if (!glfwInit())
	{
		return -1;
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1200, 720, "GameDevelopmentProject", nullptr,nullptr);
	glfwMakeContextCurrent(window);
	if (glewInit()!=GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}
	if (window == nullptr)
	{
		std::cout<<"Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	std::string vertexShaderSource = R"(
     #version 330 core
       layout (location=0) in vec3 position;
       layout (location=1) in vec3 color;
       out vec3 vColor;
       void main()
          { 
             vColor=color;
             gl_Position = vec4(position.x,position.y,position.z,1.0);
          }
     )";
	GLuint vertexShader =glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderCStr = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "[ERROR:VERTEX_SHADER_COMPILATION_FAILED" << std::endl;
	}
	std::string fragmentShaderSource = R"(
     #version 330 core
     out vec4 FragColor;
     in vec3 vColor;

     void main()
{      
       FragColor = vec4(vColor,1.0);
}

)";
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderCStr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader,1,&fragmentShaderCStr,nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infolog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
		std::cerr << "[ERROR:FRAGMENT_SHADER_COMPILATION_FAILED]" << infolog << std::endl;
	}

	GLuint shaderProgram =glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infolog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
	    std::cerr << "[ERROR:SHADER_PROGRAM_LINKING_FAILED]" << infolog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::vector<float> vertices =
	{
		0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,
		-0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,
		-0.5f,-0.5f,0.0f,0.0f,0.0f,1.0f,
		0.5f,-0.5f,0.0f,1.0f,1.0f,0.0f
	};

	std::vector<unsigned int> indices
	{
		0,1,2,
		0,2,3
	};

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0,3,GL_FLOAT,false,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

    }

	glfwTerminate();
	return 0;

}
