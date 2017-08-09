#pragma once
#include <string>
#include <iostream>
#include <chrono>

namespace Time {

	// Get delta time
	double dt();

	// Get time elapsed
	double t();
	
	// Call once per frame, updates delta time
	void step();

}