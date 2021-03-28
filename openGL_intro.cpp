#include "ShaderUtils.h"

#include <string>
#include <iostream>
// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Librairie mathématique pour les transformations
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
	glViewport(0, 0, 1024, 768);

	// Préparation de la forme à afficher
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

	//VBO : L'espace mémoire de la carte pour nos données de formes
	GLuint VertexBufferObjectID;
	glGenBuffers(1, &VertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // la positioin de la variable en entrée dans le programme de shader
		3,                  // taille de l'attribut
		GL_FLOAT,           // type
		GL_FALSE,           // normalisé ou pas
		3 * sizeof(GLfloat),  // taille du nb de données à lire pour chaque vertex                
		(void*)0            // espace à sauter entre les vertex
	);

	// identificateur du programme des shaders compilé
	GLuint shaderProgramID = loadShaders("simple.vertexshader", "simple.fragmentshader");

	


	// Enfin le boucle de rendu!
	while (!glfwWindowShouldClose(window))
	{
		// nettoie le dernier écran affiché
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		// Utiliser le programme de shader
		glUseProgram(shaderProgramID);

		// Transformation du modèle
		// 1. Générer la matrice identité
		// 2. Multiplier la matrice identité par la matrice de translation
		// 2. Multiplier par la matrice de rotation
		// 3. Multiplier par la matrice de mise à l'échelle
		// 4. On obtien la matrice de transformation complète pour notre objet
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
		// Matrice de la caméra (vue)
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// Matrice du modèle (notre triangle)
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

	// Ne JAMAIS oublier de libérer la mémoire utilisée par GLFW
	glfwTerminate();

	return 0;
}


