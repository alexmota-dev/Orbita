/*
Orbit: Solar System Simulation
*/
#include "main.h"
#include "draw.h"
#include <string>
#include <iostream>

#define DELTA_TIME 100

bool isAnimating;
std::deque<Planet> planets;

int main(int argc, char **argv)
{

    /* General initialization for GLUT and OpenGL
    Must be called first */
    glutInit(&argc, argv);

    /* we define these setup procedures */
    glut_setup();
    gl_setup();
    my_setup();

    /* go into the main event loop */
    glutMainLoop();

    return (0);
}


/* This function sets up the windowing related glut calls */
void glut_setup(void)
{

    /* specify display mode -- here we ask for a GLfloat buffer and RGB coloring */
    /* NEW: tell display we care about depth now */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    /* make a 400x400 window with the title of "GLUT Skeleton" placed at the top left corner */
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Orbit: Solar System Simulator");

    /*initialize callback functions */
    glutDisplayFunc(my_display);
    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(my_keyboard);
    glutIdleFunc(my_idle);
}



/* This function sets up the initial states of OpenGL related environment */
void gl_setup(void)
{
    /* specifies a background color: black in this case */
    glClearColor(0, 0, 0, 0);

    /* Setup for 2d projection */
    glMatrixMode(GL_PROJECTION);

	//This defines our "scale" for our solar system
    gluOrtho2D(-50, 50, -50, 50);
}


void my_setup(void)
{
    //draw_lighting(); Ignore for now, might do lighting later
    isAnimating = true; // Start animation automatically

    // Moon (satellite) - smaller, orbiting Earth
    int mass = 4;
    int radius = 1;
    Point origin(20, 0); // Start farther from Earth for more stable orbit
    // Calculate orbital velocity for circular orbit: v = sqrt(G*M/r)
    // With G=0.075, M=1000, r=20: v = sqrt(0.075*1000/20) = sqrt(3.75) ≈ 1.94
    // Using exact value for stable circular orbit
    Vector heading(0, 1.94); // Upward velocity component (perpendicular to radius)
    
    Planet moon(mass, radius, origin, heading, MOON);

    // Earth - larger, centered
    int mass1 = 1000; // Increased mass for stronger gravity
    int radius1 = 7;
    Point origin1(0, 0); // Center at origin
    Vector heading1(0, 0); // Earth doesn't move (or moves very little)

    Planet earth(mass1, radius1, origin1, heading1, EARTH);

    // Add Earth first, then Moon
    planets.push_back(earth);
    planets.push_back(moon);

}


void my_reshape(int w, int h)
{
    /* define view port -- x, y, (origin is at lower left corner) width, height */
    glViewport(0, 0, std::min(w, h), std::min(w, h));
}


void my_keyboard(unsigned char key, int /*x*/, int /*y*/)
{

    switch (key)
    {
        case 'p':
        case 'P':
            // Toggle animation on/off
            isAnimating = !isAnimating;
            break;
        case 'q':
        case 'Q':
            exit(0);
        case 'r':
        case 'R':
            // Reset simulation
            planets.clear();
            my_setup();
            break;
        default:
            return;
            break;
    }

    glutPostRedisplay();
}


void my_idle(void)
{
    // This function is called continuously when the system is idle
    if (isAnimating)
    {
        // Calculate forces first, then update positions (to avoid updating while calculating)
        // Create a copy of current positions for force calculations
        std::deque<Planet> planetsCopy = planets;
        
        // Update physics for all planets using the snapshot
        // Skip index 0 (Earth - it doesn't move much or at all)
        for (size_t i = 1; i < planets.size(); i++)
        {
            planets.at(i).sumVector(planetsCopy);
        }
        glutPostRedisplay(); // Request a redraw
    }
}

void my_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    // Draw all planets
    drawSolarSystem(planets);
    
    /* buffer is ready */
    glutSwapBuffers();
}

