#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// fonction appelée lors d'une erreur
static void error_callback(int error, const char* description)
{
	std::cout << "Error " << description << "\n";
}

// fonction appelée lors d'une entrée au clavier
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
	// Un pointeur pour une fenêtre windows créé par GLFW
	GLFWwindow* window;

	// Dire à GLFW quoi faire quand il y a une erreur
	glfwSetErrorCallback(error_callback);

	// Si on n'est pas capable d'initialiser GLFW on arrête tout!
	if (!glfwInit())
	{
		return -1;
	}

	// Définition des propriété de la fenêtre de rendu
	// Les deux prochaines commandes spécifient que l'on veut utilise OpenGL 4.2 minimalement
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	// La fenêtre ne pourra pas changer de taille, ça évite la gestion de cet événement
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// On génère la fenêtre avec un titre
	// S'il y une erreur on arrête tout, mais surtout on libère d'abord la mémoire utilisée par GLFW
	window = glfwCreateWindow(1024, 768, "Hello OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Avant d'utiliser la carte graphique il faut initialiser GLEW
	// Encore une fois, s'il y a une erreur on arrête tout et on libère la mémoire
	// glewExperimental spécifie que l'on veut utiliser les fonctions OPENGL modernes (v3 +)
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		glfwTerminate();
		return -1;
	}

	// On spécifie à GLFW quelle fonction utiliser pour les entrées du clavier
	glfwSetKeyCallback(window, key_callback);

	//
	//glViewport(0, 0, 1024, 768);

	// Préparation de la forme à afficher
	GLfloat vertices[]
	{
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f,-0.5f, 0.0f,
	};

	// VAO : on va voir bientôt à quoi ça sert!
	GLuint VertexArrayObjectID;
	glGenVertexArrays(1, &VertexArrayObjectID);
	glBindVertexArray(VertexArrayObjectID);

	//VBO : L'espace mémoire de la carte pour nos données de formes
	GLuint VertexBufferObjectID;
	glGenBuffers(1, &VertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Vertex shader : prog de la carte graphique pour la position à l'écran
	std::string vertexShaderStr{ loadShader("simple.vertexshader") };


	// Fragment shader : porg de la carte graphique pour la couleur finale des pixels
	std::string fragmentShaderStr{ loadShader("simple.fragmentshader") };

	// Enfin le boucle de rendu!
	while (!glfwWindowShouldClose(window))
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectID);
		glVertexAttribPointer(
			0,                  // la positioin de la variable en entrée dans le programme de shader
			3,                  // taille de l'attribut
			GL_FLOAT,           // type
			GL_FALSE,           // normalisé ou pas
			3*sizeof(GLfloat),  // taille du nb de données à lire pour chaque vertex                
			(void*)0            // espace à sauter entre les vertex
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Ne JAMAIS oublier de libérer la mémoire utilisée par GLFW
	glfwTerminate();

	return 0;
}


