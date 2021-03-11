#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// fonction appel�e lors d'une erreur
static void error_callback(int error, const char* description)
{
	std::cout << "Error " << description << "\n";
}

// fonction appel�e lors d'une entr�e au clavier
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

// fonction pour lire les fichiers de shader
std::string loadShader(std::string path)
{
	std::string shaderCode;
	std::ifstream shaderFile{ path };
	if (shaderFile.is_open())
	{
		std::stringstream shaderStream;
		shaderStream << shaderStream.rdbuf();
		shaderCode = shaderStream.str();
		shaderFile.close();
	}

	return shaderCode;
}

int main()
{
	// Un pointeur pour une fen�tre windows cr�� par GLFW
	GLFWwindow* window;

	// Dire � GLFW quoi faire quand il y a une erreur
	glfwSetErrorCallback(error_callback);

	// Si on n'est pas capable d'initialiser GLFW on arr�te tout!
	if (!glfwInit())
	{
		return -1;
	}

	// D�finition des propri�t� de la fen�tre de rendu
	// Les deux prochaines commandes sp�cifient que l'on veut utilise OpenGL 4.2 minimalement
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	// La fen�tre ne pourra pas changer de taille, �a �vite la gestion de cet �v�nement
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// On g�n�re la fen�tre avec un titre
	// S'il y une erreur on arr�te tout, mais surtout on lib�re d'abord la m�moire utilis�e par GLFW
	window = glfwCreateWindow(1024, 768, "Hello OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Avant d'utiliser la carte graphique il faut initialiser GLEW
	// Encore une fois, s'il y a une erreur on arr�te tout et on lib�re la m�moire
	// glewExperimental sp�cifie que l'on veut utiliser les fonctions OPENGL modernes (v3 +)
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		glfwTerminate();
		return -1;
	}

	// On sp�cifie � GLFW quelle fonction utiliser pour les entr�es du clavier
	glfwSetKeyCallback(window, key_callback);

	//
	//glViewport(0, 0, 1024, 768);

	// Pr�paration de la forme � afficher
	GLfloat vertices[]
	{
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f,-0.5f, 0.0f,
	};

	// VAO : on va voir bient�t � quoi �a sert!
	GLuint VertexArrayObjectID;
	glGenVertexArrays(1, &VertexArrayObjectID);
	glBindVertexArray(VertexArrayObjectID);

	//VBO : L'espace m�moire de la carte pour nos donn�es de formes
	GLuint VertexBufferObjectID;
	glGenBuffers(1, &VertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Vertex shader : prog de la carte graphique pour la position � l'�cran
	std::string vertexShaderStr{ loadShader("simple.vertexshader") };


	// Fragment shader : porg de la carte graphique pour la couleur finale des pixels
	std::string fragmentShaderStr{ loadShader("simple.fragmentshader") };

	// Enfin le boucle de rendu!
	while (!glfwWindowShouldClose(window))
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectID);
		glVertexAttribPointer(
			0,                  // la positioin de la variable en entr�e dans le programme de shader
			3,                  // taille de l'attribut
			GL_FLOAT,           // type
			GL_FALSE,           // normalis� ou pas
			3*sizeof(GLfloat),  // taille du nb de donn�es � lire pour chaque vertex                
			(void*)0            // espace � sauter entre les vertex
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Ne JAMAIS oublier de lib�rer la m�moire utilis�e par GLFW
	glfwTerminate();

	return 0;
}


