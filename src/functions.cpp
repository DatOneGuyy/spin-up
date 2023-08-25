#include "main.h"
#include "functions.hpp"

double controller_map(double i) {
	double sign = i == 0 ? 0 : fabs(i) / i;
    double input = fabs(i * 100);
    double output = 0;

    if (input < 5) {
        output = 0;
    } else if (input < 100.0 / 1.3) {
        output = input * 1.3;
    } else {
        output = 100;
    }

    return output / 100 * sign;
}

double ptv(double percent) {
	return percent * 120;
}

double avg(double a, double b) {
  return (a + b) / 2;
}

double c(double min, double max, double value) {
  if (value < min) {
    return min;
  } else if (value > max) {
    return max;
  } else {
    return value;
  }
}

double slew(double rate, int count, double target, double base) {
	if (fabs(base + rate * count) < fabs(target)) {
		return base + rate * count;
	} else {
		return target;
	}
}

double sign(double x) {
  if (x == 0) {
    return 0;
  } else {
    return fabs(x) / x;
  }
}

void shift(void) {
  for (int i = 0; i < smoothing - 1; i++) {
    speeds[i] = speeds[i + 1];
  }
  speeds[smoothing - 1] = speeds[smoothing - 2];
}

double average_speed(void) {
  double sum = 0;
  for (int i = 0; i < smoothing; i++) {
    sum += speeds[i] * (i + 1);
  }
  return sum / ((smoothing * (smoothing + 1.0)) / 2.0);
}