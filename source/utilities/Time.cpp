#include <utilities\Time.h>

namespace Time
{
	namespace local
	{
		double t = 0;
		// No. of seconds the previous frame took to render
		double dt = 1;
		unsigned long long currentMillis = 0;
		unsigned long long lastMillis = 0;
	}

	double dt()
	{
		return local::dt;
	}

	double t()
	{
		return local::t;
	}

	void step()
	{
		local::lastMillis = local::currentMillis;
		local::currentMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		local::dt = (double)(local::currentMillis - local::lastMillis) / 1000.0;
		local::t += local::dt;
	}

}