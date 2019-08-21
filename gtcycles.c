#include <stdio.h>
#include <unistd.h>

typedef unsigned long long u64;

static u64 get_cycles_gentimer(void) {
	 u64 val;
	 asm volatile("isb\n"
	 		      "mrs %0, cntvct_el0\n"
	 		      : "=r"(val));
	 return val;
}

static u64 get_freq_gentimer(void) {
	 u64 val;
	 asm volatile("isb\n"
	 		      "mrs %0, cntfrq_el0\n"
	 		      : "=r"(val));
	 return val;
}

static u64 measure_freq(int period) {
	u64 a = get_cycles_gentimer();
	sleep(period);
	u64 b = get_cycles_gentimer();
	u64 delta = b - a;

	return delta / period;
}

int main(void) {
	printf("Reported generic timer frequency: %llu Hz\n", get_freq_gentimer());

	printf("Measured generic timer frequency:\n");
	double total = 0;
	for (int i = 0; i < 15; i++) {
		u64 freq = measure_freq(1);
		printf("%llu Hz\n", freq);
		total += (double) freq;
	}

	printf("\nAverage: %.1lf Hz\n", total / 15);

	return 0;
}
