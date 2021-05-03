#pragma once
#include <vector>
#include <numeric>
#include <cmath>
#include <ctime>

class Perlin
{
public:
	Perlin();
	~Perlin();

	//functions to randomise and create noise
	void RandomisePermutations(bool random);
	double noise(double x, double y, double z);

private:

	std::vector<int> permutations;

	double fade(double t);
	double lerp(double scale, double x, double y);
	double grad(int hash, double x, double y, double z);
};

