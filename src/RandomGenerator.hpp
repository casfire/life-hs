/* RandomGenerator v1.2.2 Date: 2013-04-20 */

#pragma once
#ifndef _RANDOMGENERATOR_HPP_
#define _RANDOMGENERATOR_HPP_
#define _RANDOMGENERATOR_VER_ 122

#include <stdint.h>

enum RandAlg{ RNDSTDLIB, RNDLCG, RNDMWC, RNDWELL, RNDMT };
extern class Random randomGenerator; //global Random instance

//STDLIB (msvcrt's version)
#define RND_STDLIB_MAX 0x7FFF
class Random_STDLIB{
	unsigned int seed;
public:
	Random_STDLIB();
	void Init();
	void Srand(uint32_t x);
	uint32_t Next();
	float xrand(float min=0.f, float max=1.f);
	double xrand(double min, double max);
};

//LCG
#define RND_LCG_MAX 0xFFFFFFFFUL
class Random_LCG{
	uint32_t seed;
public:
	Random_LCG();
	void Init();
	void Srand(uint32_t x);
	uint32_t Next();
	float xrand(float min=0.f, float max=1.f);
	double xrand(double min, double max);
};

//MWC
#define RND_MWC_MAX 0xFFFFFFFFUL
class Random_MWC{
	uint32_t Q[4096], c, i;
public:
	Random_MWC();
	void Init();
	void Srand(uint32_t x);
	uint32_t Next();
	float xrand(float min=0.f, float max=1.f);
	double xrand(double min, double max);
};

//WELL512
#define RND_WELL_MAX 0xFFFFFFFFUL
class Random_WELL{
	uint32_t state[16], index;
public:
	Random_WELL();
	void Init();
	void Srand(uint32_t x);
	uint32_t Next();
	float xrand(float min=0.f, float max=1.f);
	double xrand(double min, double max);
};

//MT19937
#define RND_MT_MAX 0xFFFFFFFFUL
#define RND_MT_N 624
class Random_MT{
	uint32_t state[RND_MT_N], left, index;
public:
	Random_MT();
	void Init();
	void Srand(uint32_t x);
	uint32_t Next();
	float xrand(float min=0.f, float max=1.f);
	double xrand(double min, double max);
};

/* Random class */
class Random{
	Random_STDLIB rStdlib;
	Random_LCG rLcg;
	Random_MWC rMwc;
	Random_WELL rWell;
	Random_MT rMt;
	RandAlg algorithm;
public:
	Random();
	void Init();
	void Srand(uint32_t x);
	uint32_t Next();
	uint32_t Rand(); //same as Next()
	float xrand(float min=0.f, float max=1.f);
	double xrand(double min, double max);
	void Set(RandAlg alg);
	RandAlg Get();
};

#endif
