#pragma once
#include <string>
#include <iostream>
#include <chrono>

namespace Time {

	// Get delta time
	double dt();
	
	// Call once per frame, updates delta time
	void step();

}