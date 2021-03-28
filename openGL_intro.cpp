#include "ShaderUtils.h"

#include <string>
#include <iostream>
// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Librairie math�matique pour les transformations
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
	glViewport(0, 0, 1024, 768);

	// Pr�paration de la forme � afficher
	GLfloat vertices[]
	{
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f,-0.5f, 0.0f,
	};

	// VAO
	GLuint VertexArrayObjectID;
	glGenVertexArrays(1, &VertexArrayObjectID);
	glBindVertexArray(VertexArrayObjectID);

	//VBO : L'espace m�moire de la carte pour nos donn�es de formes
	GLuint VertexBufferObjectID;
	glGenBuffers(1, &VertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // la positioin de la variable en entr�e dans le programme de shader
		3,                  // taille de l'attribut
		GL_FLOAT,           // type
		GL_FALSE,           // normalis� ou pas
		3 * sizeof(GLfloat),  // taille du nb de donn�es � lire pour chaque vertex                
		(void*)0            // espace � sauter entre les vertex
	);

	// identificateur du programme des shaders compil�
	GLuint shaderProgramID = loadShaders("simple.vertexshader", "simple.fragmentshader");

	


	// Enfin le boucle de rendu!
	while (!glfwWindowShouldClose(window))
	{
		// nettoie le dernier �cran affich�
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		// Utiliser le programme de shader
		glUseProgram(shaderProgramID);

		// Transformation du mod�le
		// 1. G�n�rer la matrice identit�
		// 2. Multiplier la matrice identit� par la matrice de translation
		// 2. Multiplier par la matrice de rotation
		// 3. Multiplier par la matrice de mise � l'�chelle
		// 4. On obtien la matrice de transformation compl�te pour notre objet
		glm::mat4 identite = glm::mat4(1.0f);
		glm::mat4 transformMatrix = glm::translate(identite, glm::vec3(0.0f, 0.0f, 0.0f));
		transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5, 0.5, 0.5));
		// 5. passer la matrice au shader avec une variable 'uniform'
		GLuint transformMatrixId = glGetUniformLocation(shaderProgramID, "transform");
		glUniformMatrix4fv(transformMatrixId, 1, GL_FALSE, glm::value_ptr(transformMatrix));
		//

		/* Transformation mvp 
		// Matrice de projection
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Matrice de la cam�ra (vue)
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// Matrice du mod�le (notre triangle)
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		// Matrice mvp
		glm::mat4 mvp = projection * view * model;
		// 5. passer la matrice au shader avec une variable 'uniform'
		GLuint mvpMatrixId = glGetUniformLocation(shaderProgramID, "transform");
		glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, glm::value_ptr(mvp));

		*/

		// Dessiner notre forme
		glDrawArrays(GL_TRIANGLES, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VertexBufferObjectID);
	glDeleteProgram(shaderProgramID);
	glDeleteVertexArrays(1, &VertexArrayObjectID);

	// Ne JAMAIS oublier de lib�rer la m�moire utilis�e par GLFW
	glfwTerminate();

	return 0;
}


