#include <ostream>
#include <sstream>
#include <math.h>
#include <iostream>
#include "Planet.h"

Planet::Planet(int mass, int radius, Point origin, Vector heading, int color)
        : mass(mass), radius(radius), origin(origin), heading(heading), color(color)
{
}

std::string Planet::toString()
{
    std::stringstream x;
    
    x << "Mass: " << mass << "\n"
      << "Radius: " << radius << "\n"
      << "Position: " << origin.toString() << "\n"
      << "Heading: " << heading.toString() << "\n"
      << "Color: " << color << "\n";

    return x.str();
}

// Calculates the gravitational pull from this planet to a given planet P
double Planet::calculateGravity(Planet &p)
{
    int mass1 = mass;
    int mass2 = p.mass;
    double distanceBetween = origin.distance(p.origin);

    return ((mass1 * mass2) / pow(distanceBetween, 2));
}

// Calculates the horizontal and vertical distance to another given planet P.
void Planet::distance(Planet &p, double &horizontalDistance, double &verticalDistance)
{
    horizontalDistance = p.origin.x - origin.x;
    verticalDistance = p.origin.y - origin.y;

    //printf("H: %f V: %f\n", horizontalDistance, verticalDistance);
}

// Given a planet, calculates a gravitational acceleration vector towards that planet.
// Returns acceleration (force/mass) in the direction of the other planet
Vector Planet::findVector(Planet &p)
{
    double horizontalDistance = 0;
    double verticalDistance = 0;
    distance(p, horizontalDistance, verticalDistance);
    
    // Use Euclidean distance (not Manhattan distance)
    double distanceBetween = sqrt(horizontalDistance * horizontalDistance + verticalDistance * verticalDistance);
    
    // Avoid division by zero
    if (distanceBetween < 0.01) {
        return Vector(0, 0);
    }
    
    // Calculate gravitational force: F = G * m1 * m2 / r^2
    // Using simplified gravity constant for simulation
    double G = 0.075; // Gravitational constant (adjusted for stable circular orbit)
    double forceMagnitude = (G * mass * p.mass) / (distanceBetween * distanceBetween);
    
    // Convert force to acceleration: a = F / m
    double accelerationMagnitude = forceMagnitude / mass;
    
    // Normalize direction vector (unit vector pointing from this planet to p)
    double unitX = horizontalDistance / distanceBetween;
    double unitY = verticalDistance / distanceBetween;
    
    // Acceleration vector in direction of the other planet
    double xComponent = accelerationMagnitude * unitX;
    double yComponent = accelerationMagnitude * unitY;

    Vector toReturn(xComponent, yComponent);
    return toReturn;
}

// Updates planet position and velocity based on gravitational forces
// Uses Euler integration: a = F/m, v = v + a*dt, p = p + v*dt
void Planet::sumVector(std::deque<Planet> dq)
{
    double dt = 0.005; // Time step for integration (smaller step for better stability)
    
    // Calculate total acceleration from all other planets
    Vector acceleration(0, 0);
    
    for (size_t i = 0; i < dq.size(); i++)
    {
        // Skip self by comparing positions (with small tolerance for floating point)
        double dx = origin.x - dq.at(i).origin.x;
        double dy = origin.y - dq.at(i).origin.y;
        double distSq = dx * dx + dy * dy;
        
        // If distance is very small (same planet), skip
        if (distSq < 0.0001) {
            continue;
        }
        
        // Get acceleration vector towards this other planet
        Vector accelFromPlanet = findVector(dq.at(i));
        acceleration.x += accelFromPlanet.x;
        acceleration.y += accelFromPlanet.y;
    }
    
    // Update velocity: v = v + a*dt
    heading.x += acceleration.x * dt;
    heading.y += acceleration.y * dt;
    
    // Update position: p = p + v*dt
    origin.x += heading.x * dt;
    origin.y += heading.y * dt;
}
