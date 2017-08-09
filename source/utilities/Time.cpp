#include <utilities\Time.h>

namespace Time {
	namespace local {
		double t = 0;
		double dt = 1;
		unsigned long long currentMillis = 0;
		unsigned long long lastMillis = 0;
	}

	double dt() {
		return local::dt;
	}

	double t() {
		return local::t;
	}

	void step() {
		local::lastMillis = local::currentMillis;
		local::currentMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		local::dt = ((local::currentMillis - local::lastMillis) / (1/60.0))/1000;
		local::t += local::dt;
	}

}