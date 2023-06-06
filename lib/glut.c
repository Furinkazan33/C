/*
 * colorswap.c - Options -lglut -lGL -lGLU :
 * cc colorswap.c -lglut -lGL -lGLU && ./a.out
 * 
 * Donnez le focus à la fenêtre, puis pressez :
 * R G B ou Y pour couleur du fond
 * r g b ou y pour celle de l'encre
 * q pour quitter.
 * 
 * Programme écrit sous OpenSuSE avec Geany sous
 * Sony Satellite G50 bootant Linux d'un SSD USB3
 * en inhibant le secure boot et en donnant la 
 * priorité de boot à l'USB.  
 */

#include "GL/glut.h"

int firstPass = 1; // simule le « once » d'Eiffel

// Fonction associée (« callback ») à la fenêtre.
// Elle redessine tout quand on redimensionne.
void Rafraichir(void)	
{
  glClear(GL_COLOR_BUFFER_BIT);	// Effacer la surface graphique

  if (firstPass) {
    glColor3f(0, 0, 1.0);  // Encre bleue au départ
    firstPass = 0;
  }

  // Dessiner un triangle
  glBegin(GL_POLYGON); 	// Commencer un polygone
  glVertex2i(100, 100); // Coordonnées des trois points
  glVertex2i(400, 100);
  glVertex2i(250, 400);
  glEnd(); 				// Fermer le polygone
  glFlush(); 			// Dessiner le polygone
}

// Action sur frappe d'une touche au clavier
void Clavier(unsigned char key, int x, int y)
{
  switch(key) { 
    case 'q' : exit(0); // q => « quitter »
  
    case 'R' : glClearColor(1, 0, 0, 0); break; 	// R G B : couleur fond
    case 'G' : glClearColor(0, 1, 0, 0); break;
    case 'B' : glClearColor(0, 0, 1, 0); break;
    case 'Y' : glClearColor(1, 1, 0, 0); break; 	// Y : jaune
  
    case 'r' : glColor3f(1, 0, 0); break; 	// r g b : couleur encre
    case 'g' : glColor3f(0, 1, 0); break;
    case 'b' : glColor3f(0, 0, 1); break;
    case 'y' : glColor3f(1, 1, 0); break; 	// y ; jaune
  }

  Rafraichir();	// Affichage
}

int main(int argc, char *argv[])
{
  int win; 					// Identificateur de fenêtre (une seule)
  glutInit(&argc, argv); 			// initialise GLUT
  glutInitDisplayMode(GLUT_RGB); 	// On travaille en RGB
  glutInitWindowSize(500,400);   	// 500px large, 400px haut
  win = glutCreateWindow("Pressez r/g/b/y/R/G/B/Y/q");

  // Ordres maintenant exécutés dans la fenêtre « win »
  glClearColor(0.9, 0.9, 0.2, 0); 	// Jaune
  gluOrtho2D(0,600,0,600);			// On garde ces coordonnées

  glutDisplayFunc(Rafraichir); 		// Callback de la fenêtre
  glutKeyboardFunc(Clavier); 		// Callback du clavier

  glutMainLoop();					// Boucle infinie

  /* Le programme n'atteint jamais ce point */

  return 0;
}

