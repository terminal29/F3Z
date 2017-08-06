#include <utilities\Time.h>

namespace Time {
	namespace local {
		double dt = 1 / 60;
		unsigned long long currentMillis = 0;
		unsigned long long lastMillis = 0;
	}

	double dt() {
		return local::dt;
	}

	void step() {
		local::lastMillis = local::currentMillis;
		local::currentMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		local::dt = ((local::lastMillis - local::currentMillis) / (1/60.0));
	}

}