#pragma once
#include <chrono>
#include <iostream>
#include <string>


namespace Time {

// Get delta time in seconds
double dt();

// Get time elapsed in seconds
double t();

// Call once per frame, updates delta time
void step();

}