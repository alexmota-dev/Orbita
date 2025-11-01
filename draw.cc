#include "draw.h"
#include "main.h"
#include <cmath>


static int SMOOTHNESS = 300;

/* colors */
GLfloat colors[][3] = {
        {0.0, 0.0, 0.0},                    /* black      */
        {1.0, 0.0, 0.0},                    /* red        */
        {1.0, 1.0, 0.0},                    /* yellow     */
        {1.0, 0.5, 0.0},                    /* orange     */
        {0.0, 1.0, 0.0},                    /* green      */
        {0.0, 0.0, 1.0},                    /* blue       */
        {1.0, 1.0, 1.0},                    /* white      */
        {1.0, 0.0, 1.0},                    /* magenta    */
        {0.0, 1.0, 1.0},                    /* cyan       */
        {0.5, 0.5, 0.5},                    /* 50% grey   */
        {0.019, 0.32, 0.589},               /* opal       */
        {0.59, 0.0, 0.0},                   /* brick red  */
        {0.1, 0.3, 0.0},                    /* dark green */
        {0.309804, 0.184314, 0.309804},     /* purple     */
        {0.8, 0.498039, 0.196078},          /* gold???    */
        {0.2, 0.4, 0.8},                    /* earth (blue ocean) */
        {0.7, 0.7, 0.65}                    /* moon (light gray) */
};


void drawCircle(int radius)
{
    for (int i = 0; i < SMOOTHNESS; i++)
    {
        double angle = 2 * PI * i / SMOOTHNESS;
        double x = cos(angle) * radius;
        double y = sin(angle) * radius;
        glVertex2f(x, y);
    }
}


void drawPlanet(Planet &p)
{
    glPushMatrix();
    {
        glTranslatef(p.origin.x, p.origin.y, 0);
        
        if (p.color == EARTH)
        {
            // Draw Earth-like planet with ocean blue, green continents, and white clouds
            // Use TRIANGLE_FAN with colored vertices for smooth color interpolation
            glBegin(GL_TRIANGLE_FAN);
            {
                // Center point - blend of ocean and land
                glColor3f(0.25, 0.45, 0.7);
                glVertex2f(0, 0);
                
                // Draw circle with varying colors to simulate Earth's appearance
                // Each vertex gets a color that will interpolate across the surface
                for (int i = 0; i <= SMOOTHNESS; i++)
                {
                    double angle = 2 * PI * i / SMOOTHNESS;
                    double x = cos(angle) * p.radius;
                    double y = sin(angle) * p.radius;
                    
                    // Create patterns using multiple sine waves for variety
                    double pattern1 = (sin(angle * 2.5) + 1.0) * 0.5; // 0 to 1
                    double pattern2 = (cos(angle * 3.7) + 1.0) * 0.5; // 0 to 1
                    double pattern3 = (sin(angle * 5.2) + 1.0) * 0.5; // 0 to 1
                    
                    // Determine color based on patterns
                    // Use weighted blend between ocean (blue), land (green), and clouds (white)
                    float oceanWeight = (1.0 - pattern1) * 0.6; // Ocean dominant
                    float landWeight = pattern1 * 0.6;         // Land where pattern is high
                    float cloudWeight = pattern2 * 0.2;         // Clouds scattered
                    
                    // Normalize weights
                    float totalWeight = oceanWeight + landWeight + cloudWeight;
                    if (totalWeight > 0.001) {
                        oceanWeight /= totalWeight;
                        landWeight /= totalWeight;
                        cloudWeight /= totalWeight;
                    }
                    
                    // Blend colors
                    float r = oceanWeight * 0.2f + landWeight * 0.1f + cloudWeight * 0.9f;
                    float g = oceanWeight * 0.4f + landWeight * 0.6f + cloudWeight * 0.9f;
                    float b = oceanWeight * 0.8f + landWeight * 0.2f + cloudWeight * 0.95f;
                    
                    // Add some variation for realism
                    r += pattern3 * 0.05f;
                    g += pattern2 * 0.05f;
                    b += pattern3 * 0.05f;
                    
                    // Clamp values to [0, 1]
                    if (r > 1.0f) r = 1.0f;
                    if (g > 1.0f) g = 1.0f;
                    if (b > 1.0f) b = 1.0f;
                    if (r < 0.0f) r = 0.0f;
                    if (g < 0.0f) g = 0.0f;
                    if (b < 0.0f) b = 0.0f;
                    
                    glColor3f(r, g, b);
                    glVertex2f(x, y);
                }
            }
            glEnd();
        }
        else if (p.color == MOON)
        {
            // Draw Moon-like planet with gray base and craters
            glBegin(GL_POLYGON);
            {
                // Base moon color (light gray)
                glColor3f(0.7, 0.7, 0.65);
                drawCircle(p.radius);
            }
            glEnd();
            
            // Add darker spots for craters
            glBegin(GL_POLYGON);
            {
                glColor3f(0.5, 0.5, 0.48); // Darker gray for craters
                for (int i = 0; i < SMOOTHNESS; i++)
                {
                    double angle = 2 * PI * i / SMOOTHNESS;
                    // Create crater-like depressions
                    double radiusVariation = p.radius * (0.88 + 0.12 * sin(angle * 4));
                    double x = cos(angle) * radiusVariation;
                    double y = sin(angle) * radiusVariation;
                    glVertex2f(x, y);
                }
            }
            glEnd();
        }
        else
        {
            // Standard solid color for other planets
            glBegin(GL_POLYGON);
            {
                glColor3fv(colors[p.color]);
                drawCircle(p.radius);
            }
            glEnd();
        }
    }
    glPopMatrix();
}

void drawSolarSystem(std::deque<Planet> planets)
{
    for (size_t i = 0; i < planets.size(); i++)
    {
        Planet planet = planets.at(i);
        drawPlanet(planet);
    }
}