#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NB_EXEMPLES 4
#define SIZE_EXEMPLES 4

typedef struct Network Network;
typedef struct Layers Layers;
typedef struct Exemple Exemple;

struct Exemple
{
	double x[SIZE_EXEMPLES];
	double d;
};

struct Layers
{
	double *w;
	double *x;
	double *delta;
};

struct Network
{
	int nbLayer;
	int nbInput;
	int nbOutput;
	double taux;
	int size_x;
	Layers *layers;
};

double fonctionActivation(double x);
double fonctionActivationDerivative(double x);
double sommePondere(Network *net, int layer, int x);
double sommePondereDelta(Network *net, int layer, int x);
void printNetwork(Network *net);
void setWeights(Network *net);
void initDeltas(Network *net);
void initNodes(Network *net);
void propagation(Network *net, int i, Exemple exemple[]);
void back_propagation(Network *net, int i, Exemple exemple[]);
void MAJweights(Network *net);
void saveWeights(Network net);
void utilisation(Network *net, int i, Exemple exemple[]);
void allocNet(Network *net);
void freeNet(Network *net);

#endif