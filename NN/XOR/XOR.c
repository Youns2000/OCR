#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TAUX 0.1
#define NB_EXEMPLES 4

#define INPUTS		2	//nombre d'entrées
#define SIZE_X 		4	//nombre de neurone par couche cachées
#define L 	   		3	//nombre de couches
#define OUTPUTS		1	//nombre de sorties

typedef struct Exemple Exemple;

typedef struct Inputs Inputs;
typedef struct Layers Layers;
typedef struct Outputs Outputs;
typedef struct Network Network;

struct Exemple
{
	double x[SIZE_X];
	double d;
};

struct Layers
{
	double w[SIZE_X][SIZE_X];
	double x[SIZE_X];
	double delta[SIZE_X];
};

struct Network
{
	Layers layers[L];
};

double fonctionActivation(double x) {
	return 1 / (1 + exp(-x)); //fonctionActivation
}

double fonctionActivationDerivative(double x) {
	return x * (1 - x);  //dérivée de la fonctionActivation
}

double sommePondere(Network *net, int layer, int x) {
	double y = 0.0;
	double MAX;
	if (layer == 1) MAX = INPUTS;
	else MAX = SIZE_X;
	for (int i = 0; i < MAX; ++i)
	{
		y += net->layers[layer - 1].w[i][x] * net->layers[layer - 1].x[i];
	}
	return y;
}

double sommePondereDelta(Network *net, int layer, int x) {
	double y = 0.0;
	double MAX;
	if (layer == (L - 2)) MAX = OUTPUTS;
	else MAX = SIZE_X;
	for (int i = 0; i < MAX; ++i)
	{
		y += net->layers[layer].w[x][i] * net->layers[layer + 1].delta[i];
	}
	return y;
}

void printNetwork(Network net) {
	printf("Entrees:\n");
	for (int node = 0; node < INPUTS; ++node)
	{
		printf("x%d : %f\n", node, net.layers[0].x[node]);
		for (int i = 0; i < SIZE_X; ++i)
		{
			printf("	w%d,%d : %f\n", node, i, net.layers[0].w[node][i]);
		}
	}
	printf("\n");
	printf("Noeuds:\n");
	for (int layer = 1; layer < L - 2; ++layer)
	{
		printf("L%d :\n", layer);
		for (int node = 0; node < SIZE_X; ++node)
		{
		printf("	neurone%d : %f\n",node + layer, net.layers[layer].x[node]);
			for (int i = 0; i < SIZE_X; ++i)
			{
		printf("		w%d,%d : %f\n", node, i, net.layers[layer].w[node][i]);
			}
		}
		printf("\n");
	}
	printf("L%d :\n", L-2);
	for (int node = 0; node < SIZE_X; ++node)
	{
		printf("	neurone%d : %f\n", node, net.layers[L - 2].x[node]);
		for (int i = 0; i < OUTPUTS; ++i)
		{
    printf("	w(%d)%d,%d : %f\n", L-2,node,i,net.layers[L - 2].w[node][i]);
		}
	}
	printf("\n");
	printf("Sorties:\n");
	for (int node = 0; node < OUTPUTS; ++node)
	{
		printf("Out : %f\n", net.layers[L - 1].x[node]);
	}
	printf("-------------------------------------------------------------\n");
}

void setWeights(Network *net) {
	srand(time(NULL));
	for (int node = 0; node < INPUTS; ++node)
	{
		for (int i = 0; i < SIZE_X; ++i)
		{
			net->layers[0].w[node][i] = rand() % 11 -5;
		}
	}
	for (int l = 0; l < L - 2; ++l)
	{
		for (int node = 0; node < SIZE_X; ++node)
		{
			for (int i = 0; i < SIZE_X; ++i)
			{
				net->layers[l].w[node][i] = rand() % 11 - 5;
			}
		}
	}
	for (int node = 0; node < SIZE_X; ++node)
	{
		for (int i = 0; i < OUTPUTS; ++i)
		{
			net->layers[L - 2].w[node][i] = rand() % 11 - 5;
		}
	}
}

void initDeltas(Network *net) {
	for (int node = 0; node < SIZE_X; ++node)
	{
		net->layers[0].delta[node] = 0;
	}
	for (int l = 1; l < L - 1; ++l)
	{
		for (int node = 0; node < SIZE_X; ++node)
		{
			net->layers[l].delta[node] = 0;
		}
	}
	for (int node = 0; node < OUTPUTS; ++node)
	{
		net->layers[L - 1].delta[node] = 0;
	}
}

void initNodes(Network *net) {
	for (int node = 0; node < SIZE_X; ++node)
	{
		net->layers[0].x[node] = 0;
	}
	for (int l = 1; l < L - 1; ++l)
	{
		for (int node = 0; node < SIZE_X; ++node)
		{
			net->layers[l].x[node] = 0;
		}
	}
	for (int node = 0; node < OUTPUTS; ++node)
	{
		net->layers[L - 1].x[node] = 0;
	}
}

void propagation(Network *net, int i, Exemple exemple[]) {
	for (int input_nodes = 0; input_nodes < INPUTS; ++input_nodes)
	{
		net->layers[0].x[input_nodes] = exemple[i].x[input_nodes];
	}
	for (int l = 1; l < L - 1; ++l)
	{
		for (int node = 0; node < SIZE_X; ++node)
		{
			double somme = sommePondere(net, l, node);
			net->layers[l].x[node] = fonctionActivation(somme);
		}
	}
	for (int node = 0; node < OUTPUTS; ++node)
	{
		double somme = sommePondere(net, L - 1, node);
		net->layers[L - 1].x[node] = fonctionActivation(somme);
	}
}

void back_propagation(Network *net, int i, Exemple exemple[]) {
	for (int node = 0; node < OUTPUTS; ++node)
	{
		net->layers[L - 1].delta[node]=exemple[i].d-net->layers[L - 1].x[node];
	}
	for (int l = L - 2; l > 0; --l)
	{
		for (int node = 0; node < SIZE_X; ++node)
		{
			double deriv=fonctionActivationDerivative(net->layers[l].x[node]);
			double sommePondereD= sommePondereDelta(net, l, node);
			net->layers[l].delta[node]=  deriv * sommePondereD;
		}
	}
}

void MAJweights(Network *net) {
	for (int l = 0; l < L - 1; ++l)
	{
		for (int node = 0; node < SIZE_X; ++node)
		{
			for (int i = 0; i < SIZE_X; ++i)
			{
				double x_node = net->layers[l].x[node];
				double delta_node = net->layers[l + 1].delta[i];
				net->layers[l].w[node][i] += TAUX * x_node * delta_node;
			}
		}
	}
}

void saveWeights(Network net) {
	FILE *file = NULL;

	file = fopen("weights.txt", "w");

	if (file != NULL)
	{
		for (int l = 0; l < L - 2; ++l)
		{
			for (int node = 0; node < SIZE_X; ++node)
			{
				for (int i = 0; i < SIZE_X; ++i)
				{
			fprintf(file,"(%d;%d;%d)=%f\n",l,node,i,net.layers[l].w[node][i]);
				}
			}
		}
		for (int node = 0; node < SIZE_X; ++node)
		{
			for (int i = 0; i < OUTPUTS; ++i)
			{
		fprintf(file,"(%d;%d;%d)=%f\n",L-2,node,i,net.layers[L-2].w[node][i]);
			}
		}
		fclose(file);
	}
	else printf("Impossible d'ouvrir le fichier weights.txt");
}

void utilisation(Network net, int i, Exemple exemple[]) {
	propagation(&net, i, exemple);
	printNetwork(net);
}

int main() {
	Network net;
	Exemple exemple[NB_EXEMPLES];

	exemple[0].x[0] = 1;
	exemple[0].x[1] = 1;
	exemple[0].d = 0;

	exemple[1].x[0] = 0;
	exemple[1].x[1] = 0;
	exemple[1].d = 0;

	exemple[2].x[0] = 1;
	exemple[2].x[1] = 0;
	exemple[2].d = 1;

	exemple[3].x[0] = 0;
	exemple[3].x[1] = 1;
	exemple[3].d = 1;

	setWeights(&net);
	initDeltas(&net);
	initNodes(&net);

	double total_error = 1;
	while(total_error>0.0004)
	{
		total_error = 0;
		for (int i = 0; i < NB_EXEMPLES; ++i)
		{
			propagation(&net, i, exemple);	//Propagation jusqu'à la sortie

			total_error += fabs(exemple[i].d - net.layers[L - 1].x[0]);

			back_propagation(&net, i, exemple); //back_prop jusqu'aux entrées

			MAJweights(&net);	//Mise à jour des poids
		}
	}

	saveWeights(net);

	for (int i = 0; i < NB_EXEMPLES; i++)
	{
		utilisation(net, i, exemple);
	}
	return 0;
}