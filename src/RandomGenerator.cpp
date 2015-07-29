#include "RandomGenerator.hpp"

#if _RANDOMGENERATOR_VER_ != 122
	#warning Invalid RandomGenerator header version
#endif

/* Random_MT */

#define RND_MT_N 624
#define RND_MT_M 397
#define RND_MT_MIXBITS(u,v) ( ((u) & 0x80000000UL) | ((v) & 0x7fffffffUL) )
#define RND_MT_TWIST(u,v) ((RND_MT_MIXBITS(u,v) >> 1) ^ ((v)&1UL ? 0x9908b0dfUL : 0UL))

Random_MT::Random_MT(){
	Init();
}

void Random_MT::Init(){
	left = 1;
	index = 0;
	state[0] = ((unsigned long)5489UL)&0xffffffffUL;
	for(int j=1; j<RND_MT_N; j++) {
		state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j);
		state[j] &= 0xffffffffUL;
	}
}

void Random_MT::Srand(uint32_t x){
	state[0] = ((unsigned long)x)&0xffffffffUL;
	for(int j=1; j<RND_MT_N; j++) {
		state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j);
		state[j] &= 0xffffffffUL;
	}
	left = 1;
}

uint32_t Random_MT::Next(){
	uint32_t y;
	if (--left == 0){
		uint32_t *p=state;
		int j;
		left = RND_MT_N;
		index = 0;
		for (j=RND_MT_N-RND_MT_M+1; --j; p++) *p = p[RND_MT_M] ^ RND_MT_TWIST(p[0], p[1]);
		for (j=RND_MT_M; --j; p++) *p = p[RND_MT_M-RND_MT_N] ^ RND_MT_TWIST(p[0], p[1]);
		*p = p[RND_MT_M-RND_MT_N] ^ RND_MT_TWIST(p[0], state[0]);
	}
	y = state[index++];
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);
	return y;
}

float Random_MT::xrand(float min, float max){
	return min + ((float)Next()*(1.0/4294967295.0))*(max-min);
}

double Random_MT::xrand(double min, double max){
	return min + ((double)Next()*(1.0/4294967295.0))*(max-min);
}

/* Random_WELL */

Random_WELL::Random_WELL(){
	Init();
}

void Random_WELL::Init(){
	index = 0;
	state[0] = 5489UL;
	for(int i=1; i<16; i++)state[i] = state[i-1]*47;
}

void Random_WELL::Srand(uint32_t x){
	state[0]=x;
	for(int i=1; i<16; i++)state[i] = state[i-1]*47;
}

uint32_t Random_WELL::Next(){
	uint32_t a,b,c,d;
	a = state[index];
	c = state[(index+13)&15];
	b = a^c^(a<<16)^(c<<15);
	c = state[(index+9)&15];
	c ^= (c>>11);
	a = state[index] = b^c;
	d = a^((a<<5)&0xDA442D20UL);
	index = (index + 15)&15;
	a = state[index];
	state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28);
	return state[index];
}

float Random_WELL::xrand(float min, float max){
	return min + ((float)Next()*(1.0/4294967295.0))*(max-min);
}

double Random_WELL::xrand(double min, double max){
	return min + ((double)Next()*(1.0/4294967295.0))*(max-min);
}

/* Random_MWC */

#define RND_MWC_PHI 0x9E3779B9

Random_MWC::Random_MWC(){
	Init();
}

void Random_MWC::Init(){
	Q[0] = 5489UL;
	Q[1] = Q[0] + RND_MWC_PHI;
	Q[1] = Q[0] + RND_MWC_PHI + RND_MWC_PHI;
	for(int z=3; z<4096; z++)Q[z]=Q[z-3]^Q[z-2]^RND_MWC_PHI^z;
	c = 362436;
	i = 4095;
}

void Random_MWC::Srand(uint32_t x){
	int z; Q[0] = x;
	Q[1] = x + RND_MWC_PHI;
	Q[2] = x + RND_MWC_PHI + RND_MWC_PHI;
	for(z=3; z<4096; z++)Q[z]=Q[z-3]^Q[z-2]^RND_MWC_PHI^z;
}

uint32_t Random_MWC::Next(){
	uint64_t t, a = 0x000000000000495E; /* 18782LL */
	uint32_t x, r = 0xfffffffe;
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = t + c;
	if(x < c){ x++; c++; }
	return (Q[i] = r - x);
}

float Random_MWC::xrand(float min, float max){
	return min + ((float)Next()*(1.0/4294967295.0))*(max-min);
}

double Random_MWC::xrand(double min, double max){
	return min + ((double)Next()*(1.0/4294967295.0))*(max-min);
}

/* Random_LCG */

#define RND_LCG_A 1664525UL
#define RND_LCG_C 1013904223UL

Random_LCG::Random_LCG(){
	Init();
}

void Random_LCG::Init(){
	seed = 0;
}

void Random_LCG::Srand(uint32_t x){
	seed = x;
}

uint32_t Random_LCG::Next(){
	seed = ((RND_LCG_A*seed+RND_LCG_C)%RND_LCG_MAX);
	return seed;
}

float Random_LCG::xrand(float min, float max){
	return min + ((float)Next()*(1.0/4294967295.0))*(max-min);
}

double Random_LCG::xrand(double min, double max){
	return min + ((double)Next()*(1.0/4294967295.0))*(max-min);
}

/* Random_STDLIB */

Random_STDLIB::Random_STDLIB(){
	Init();
}

void Random_STDLIB::Init(){
	seed = 0;
}

void Random_STDLIB::Srand(uint32_t x){
	seed = x;
}

uint32_t Random_STDLIB::Next(){
	seed = seed*0x343fd+0x269EC3;
	return (seed >> 0x10) & 0x7FFF;
}

float Random_STDLIB::xrand(float min, float max){
	return min + ((float)Next()/RND_STDLIB_MAX)*(max-min);
}

double Random_STDLIB::xrand(double min, double max){
	return min + ((double)Next()/RND_STDLIB_MAX)*(max-min);
}

/* Random */

Random::Random(){
	Init();
}

void Random::Init(){
	algorithm = RNDSTDLIB;
}

void Random::Srand(uint32_t x){
	switch(algorithm){
	default: case RNDSTDLIB: rStdlib.Srand(x); break;
	case RNDLCG: rLcg.Srand(x); break;
	case RNDMWC: rMwc.Srand(x); break;
	case RNDWELL: rWell.Srand(x); break;
	case RNDMT: rMt.Srand(x); break;
	}
}

uint32_t Random::Next(){
	switch(algorithm){
	default: case RNDSTDLIB: return rStdlib.Next(); break;
	case RNDLCG: return rLcg.Next(); break;
	case RNDMWC: return rMwc.Next(); break;
	case RNDWELL: return rWell.Next(); break;
	case RNDMT: return rMt.Next(); break;
	}
}

uint32_t Random::Rand(){
	switch(algorithm){
	default: case RNDSTDLIB: return rStdlib.Next(); break;
	case RNDLCG: return rLcg.Next(); break;
	case RNDMWC: return rMwc.Next(); break;
	case RNDWELL: return rWell.Next(); break;
	case RNDMT: return rMt.Next(); break;
	}
}

float Random::xrand(float min, float max){
	switch(algorithm){
	default: case RNDSTDLIB: return rStdlib.xrand(min,max); break;
	case RNDLCG: return rLcg.xrand(min,max); break;
	case RNDMWC: return rMwc.xrand(min,max); break;
	case RNDWELL: return rWell.xrand(min,max); break;
	case RNDMT: return rMt.xrand(min,max); break;
	}
}

double Random::xrand(double min, double max){
	switch(algorithm){
	default: case RNDSTDLIB: return rStdlib.xrand(min,max); break;
	case RNDLCG: return rLcg.xrand(min,max); break;
	case RNDMWC: return rMwc.xrand(min,max); break;
	case RNDWELL: return rWell.xrand(min,max); break;
	case RNDMT: return rMt.xrand(min,max); break;
	}
}

void Random::Set(RandAlg alg){
	algorithm = alg;
}

RandAlg Random::Get(){
	return algorithm;
}

/* Global */

Random randomGenerator;
