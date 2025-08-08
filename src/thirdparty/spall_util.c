#include "spall_util.h"

#ifdef ENABLE_SPALL
SpallProfile spall_ctx = {0};
SpallBuffer  spall_buffer = {0};

#if _WIN32
#include <Windows.h>
uint64_t get_time_in_nanos(void) {
	static double invfreq;
	if (!invfreq) {
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		invfreq = 1000000000.0 / frequency.QuadPart;
	}
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	uint64_t ts = (uint64_t)((double)counter.QuadPart * invfreq);
	return ts;
}
#else
uint64_t get_time_in_nanos(void) {
	struct timespec spec;
	clock_gettime(CLOCK_MONOTONIC, &spec);
	uint64_t ts = ((uint64_t)spec.tv_sec * 1000000000ull) + (uint64_t)spec.tv_nsec;
	return ts;
}
#endif


#endif
