#include <thread>
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <chrono>

using namespace std::chrono;

#define Pi 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825

double f(double x)
{
	return 4.0/sqrt(4.0-x*x);
}

int main()
{
	double a = 0.0;
	double b = 1.0;
	size_t N = 100000000000;
	double h = (b-a)/N;
	cilk::reducer_opadd <double> I (0.0);
	double I1 = 0.0;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	cilk_for (size_t i = 0; i < N; i++)
	{
		I += f(i*h+a) * h;
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> duration = (t2 - t1);
	printf("Time parallel: %f seconds\n\n", duration.count());

	t1 = high_resolution_clock::now();
	for (size_t i = 0; i < N; i++)
	{
		I1 += f(i*h + a) * h;
	}
	t2 = high_resolution_clock::now();
	duration = (t2 - t1);

	printf("Time: %f seconds\n\n", duration.count());

	printf("Integral parallel = %f\n", I.get_value());
	printf("Integral = %f\n", I1);

	printf("Difference = %f\n", abs(I.get_value()-2*Pi/3));

	getchar();
	return 0;
}