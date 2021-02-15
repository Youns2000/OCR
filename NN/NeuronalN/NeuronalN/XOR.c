#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//#define SIZE_EXEMPLES 4

typedef struct Exemple Exemple;

typedef struct Inputs Inputs;
typedef struct Layers Layers;
typedef struct Outputs Outputs;
typedef struct Network Network;

struct Exemple
{
	double *x;
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

double fonctionActivation(double x) {
	return 1 / (1 + exp(-x)); //fonctionActivation
}

double fonctionActivationDerivative(double x) {
	return x * (1 - x);  //dérivée de la fonctionActivation
}

double sommePondere(Network *net, int layer, int x) {
	int sx = net->size_x;
	double y = 0.0;
	int MAX;
	if (layer == 1) MAX = net->nbInput;
	else MAX = net->size_x;
	for (int i = 0; i < MAX; ++i)
	{
		y += net->layers[layer - 1].w[i*sx + x] * net->layers[layer - 1].x[i];
	}
	return y;
}

double sommePondereDelta(Network *net, int layer, int x) {
	int sx = net->size_x;
	double y = 0.0;
	int MAX;
	if (layer == (net->nbLayer - 2)) MAX = net->nbOutput;
	else MAX = net->size_x;
	for (int i = 0; i < MAX; ++i)
	{
		y += net->layers[layer].w[x*sx + i] * net->layers[layer + 1].delta[i];
	}
	return y;
}

void printNetwork(Network *net) {
	int sx = net->size_x;
	printf("Entrees:\n");
	for (int node = 0; node < net->nbInput; ++node)
	{
		printf("x%d : %f\n", node, net->layers[0].x[node]);
		for (int i = 0; i < net->size_x; ++i)
		{
			printf("	w%d,%d : %f\n",
				node, i, net->layers[0].w[node*sx + i]);
		}
	}
	printf("\n");
	printf("Noeuds:\n");
	for (int layer = 1; layer < net->nbLayer - 2; ++layer)
	{
		printf("L%d :\n", layer);
		for (int node = 0; node < sx; ++node)
		{
			printf("	neurone%d : %f\n",
				node + layer, net->layers[layer].x[node]);
			for (int i = 0; i < sx; ++i)
			{
				printf("		w%d,%d : %f\n",
					node, i, net->layers[layer].w[node*sx + i]);
			}
		}
		printf("\n");
	}
	printf("L%d :\n", net->nbLayer - 2);
	for (int node = 0; node < sx; ++node)
	{
		printf("	neurone%d : %f\n",
			node, net->layers[net->nbLayer - 2].x[node]);
		for (int i = 0; i < net->nbOutput; ++i)
		{
			printf("		w(%d)%d,%d : %f\n",
				net->nbLayer - 2, node, i, net->layers[net->nbLayer - 2].w[node*sx + i]);
		}
	}
	printf("\n");
	printf("Sorties:\n");
	for (int node = 0; node < net->nbOutput; ++node)
	{
		printf("Out : %f\n", net->layers[net->nbLayer - 1].x[node]);
	}
	printf("--------------------------------------------------------------\n");
}

void setWeights(Network *net) {
	int sx = net->size_x;
	srand((unsigned int)time(NULL));
	for (int node = 0; node < net->nbInput; ++node)
	{
		for (int i = 0; i < sx; ++i)
		{
			net->layers[0].w[node*sx + i] = rand() % 11 - 5;
		}
	}
	for (int l = 0; l < net->nbLayer - 2; ++l)
	{
		for (int node = 0; node < sx; ++node)
		{
			for (int i = 0; i < sx; ++i)
			{
				net->layers[l].w[node*sx + i] = rand() % 11 - 5;
			}
		}
	}
	for (int node = 0; node < sx; ++node)
	{
		for (int i = 0; i < net->nbOutput; ++i)
		{
			net->layers[net->nbLayer - 2].w[node*sx + i] = rand() % 11 - 5;
		}
	}
}

void initDeltas(Network *net) {
	for (int node = 0; node < net->size_x; ++node)
	{
		net->layers[0].delta[node] = 0;
	}
	for (int l = 1; l < net->nbLayer - 1; ++l)
	{
		for (int node = 0; node < net->size_x; ++node)
		{
			net->layers[l].delta[node] = 0;
		}
	}
	for (int node = 0; node < net->nbOutput; ++node)
	{
		net->layers[net->nbLayer - 1].delta[node] = 0;
	}
}

void initNodes(Network *net) {
	for (int node = 0; node < net->size_x; ++node)
	{
		net->layers[0].x[node] = 0;
	}
	for (int l = 1; l < net->nbLayer - 1; ++l)
	{
		for (int node = 0; node < net->size_x; ++node)
		{
			net->layers[l].x[node] = 0;
		}
	}
	for (int node = 0; node < net->nbOutput; ++node)
	{
		net->layers[net->nbLayer - 1].x[node] = 0;
	}
}

void propagation(Network *net, int i, Exemple exemple[]) {
	for (int input_nodes = 0; input_nodes < net->nbInput; ++input_nodes)
	{
		net->layers[0].x[input_nodes] = exemple[i].x[input_nodes];
	}
	for (int l = 1; l < net->nbLayer - 1; ++l)
	{
		for (int node = 0; node < net->size_x; ++node)
		{
			double somme = sommePondere(net, l, node);
			net->layers[l].x[node] = fonctionActivation(somme);
		}
	}
	for (int node = 0; node < net->nbOutput; ++node)
	{
		double somme = sommePondere(net, net->nbLayer - 1, node);
		net->layers[net->nbLayer - 1].x[node] = fonctionActivation(somme);
	}
}

void back_propagation(Network *net, int i, Exemple exemple[]) {
	int nl = net->nbLayer;
	for (int node = 0; node < net->nbOutput; ++node)
	{
		net->layers[nl - 1].delta[node] = exemple[i].d - net->layers[nl - 1].x[node];
	}
	for (int l = net->nbLayer - 2; l > 0; --l)
	{
		for (int node = 0; node < net->size_x; ++node)
		{
			double deriv = fonctionActivationDerivative(net->layers[l].x[node]);
			double sommePondereD = sommePondereDelta(net, l, node);
			net->layers[l].delta[node] = deriv * sommePondereD;
		}
	}
}

void MAJweights(Network *net) {
	int sx = net->size_x;
	for (int l = 0; l < net->nbLayer - 2; ++l)
	{
		for (int node = 0; node < net->size_x; ++node)
		{
			for (int i = 0; i < net->size_x; ++i)
			{
				double x_node = net->layers[l].x[node];
				double delta_node = net->layers[l + 1].delta[i];
				net->layers[l].w[node*sx + i] += net->taux*x_node*delta_node;
			}
		}
	}
	for (int node = 0; node < net->size_x; ++node)
	{
		for (int i = 0; i < net->nbOutput; ++i)
		{
			double x_node = net->layers[net->nbLayer - 2].x[node];
			double delta_node = net->layers[net->nbLayer - 2 + 1].delta[i];
			net->layers[net->nbLayer - 2].w[node*sx + i] += net->taux*x_node*delta_node;
		}
	}
}

void saveWeights(Network net) {
	FILE *file = NULL;
	int sx = net.size_x;
	file = fopen("weights.txt", "w");

	if (file != NULL)
	{
		for (int node = 0; node < net.nbInput; ++node)
		{
			for (int i = 0; i < sx; ++i)
			{
				fprintf(file, "(%d;%d;%d)=%f\n",
					0, node, i, net.layers[0].w[node*sx + i]);
			}
		}
		for (int l = 1; l < net.nbLayer - 2; ++l)
		{
			for (int node = 0; node < sx; ++node)
			{
				for (int i = 0; i < sx; ++i)
				{
					fprintf(file, "(%d;%d;%d)=%f\n",
						l, node, i, net.layers[l].w[node*sx + i]);
				}
			}
		}
		for (int node = 0; node < sx; ++node)
		{
			for (int i = 0; i < net.nbOutput; ++i)
			{
				fprintf(file, "(%d;%d;%d)=%f\n",
					net.nbLayer - 2, node, i, net.layers[net.nbLayer - 2].w[node*sx + i]);
			}
		}
		fclose(file);
	}
	else printf("Impossible d'ouvrir le fichier weights.txt");
}

void utilisation(Network *net, int i, Exemple exemple[]) {
	propagation(net, i, exemple);
	printNetwork(net);
}

void allocNet(Network *net) {
	net->layers = malloc(net->nbLayer * sizeof(Layers));
	for (int i = 0; i < net->nbLayer; i++) {
		net->layers[i].w = malloc(net->size_x*net->size_x * sizeof(double));
		net->layers[i].x = malloc(net->size_x * sizeof(double));
		net->layers[i].delta = malloc(net->size_x * sizeof(double));
	}
}

void freeNet(Network *net) {
	for (int i = 0; i < net->nbLayer; i++) {
		free(net->layers[i].w);
		free(net->layers[i].x);
		free(net->layers[i].delta);
	}
	free(net->layers);
}

int main() {
	while(1){
	int choix1;
	system("clear");
	printf("Bienvenu dans le réseau de neurone le plus efficace de sa génération:\n");
	printf("(entrez 1,2 ou 3)\n\n");
	printf("1. Apprendre le XOR\n");
	printf("2. Apprendre autre chose\n");
	printf("3. Quitter\n\n");
	scanf("%d", &choix1);

	Network net;

	switch (choix1)
	{
	case 1:
		net.nbLayer = 3;
		net.nbInput = 2;
		net.nbOutput = 1;
		net.size_x = 4;
		net.taux = 0.1;

		int nbExemples = 4;
		Exemple exemple[4];
		//exemple = malloc(nbExemples * sizeof(double)*net.nbInput * sizeof(double));
		for (int i = 0; i < nbExemples; i++) {
			exemple[i].x = malloc(net.nbInput * sizeof(double));
		}

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

		allocNet(&net);
		setWeights(&net);
		initDeltas(&net);
		initNodes(&net);

		printf("Apprentissage...\n");
		double total_error = 1;
		while (total_error > 0.0004)
		{
			total_error = 0;
			for (int i = 0; i < nbExemples; ++i)
			{
				propagation(&net, i, exemple);	//Propagation jusqu'à la sortie

				total_error += fabs(exemple[i].d - net.layers[net.nbLayer - 1].x[0]);

				back_propagation(&net, i, exemple); //back_prop jusqu'aux entrées

				MAJweights(&net);	//Mise à jour des poids
			}
		}

		printf("Sauvegarde des poids dans weights.txt...\n");
		saveWeights(net);

		for (int i = 0; i < nbExemples; i++)
		{
			utilisation(&net, i, exemple);
		}

		freeNet(&net);
		int c = getchar();
		break;
	case 2:
		printf("Veuillez renseigner les champs suivants pour initialiser le reseau de neurone:\n\n");
		printf("Nombre d'entrees: ");
		scanf("%d", &net.nbInput);
		printf("\nNombre de couches cachees: ");
		scanf("%d", &net.nbLayer);
		printf("\nNombre de neurones par couches: ");
		scanf("%d", &net.size_x);
		printf("\nNombre de sorties: ");
		scanf("%d", &net.nbOutput);
		net.nbLayer += 2;


		printf("\n\nIl faut que vous donniez au moins 4 exemples pour l'apprentissage\n");
		printf("Nombre d'exemple: ");
		scanf("%d", &nbExemples);

		exemple[nbExemples];

		for (int ex = 0; ex < nbExemples; ++ex)
		{
			printf("Exemple %d\n", ex);
			for (int input = 0; input < net.nbInput; ++input)
			{
				printf("	Entree %d:", input);
				scanf("%lf", &exemple[ex].x[input]);
			}
			printf("	Valeur attendu:");
			scanf("%lf", &exemple[ex].d);
		}

		allocNet(&net);
		setWeights(&net);
		initDeltas(&net);
		initNodes(&net);

		printf("Apprentissage...\n");
		total_error = 1;
		while (total_error > 0.0004)
		{
			total_error = 0;
			for (int i = 0; i < nbExemples; ++i)
			{
				propagation(&net, i, exemple);	//Propagation jusqu'à la sortie

				total_error += fabs(exemple[i].d - net.layers[net.nbLayer - 1].x[0]);

				back_propagation(&net, i, exemple); //back_prop jusqu'aux entrées

				MAJweights(&net);	//Mise à jour des poids
			}
		}

		printf("Sauvegarde des poids dans weights.txt...\n");
		saveWeights(net);

		for (int i = 0; i < nbExemples; i++)
		{
			utilisation(&net, i, exemple);
		}
		int c = getchar();
		break;
	case 3:
		return 0;
		break;
	default:
		return 0;
	}
	}
	return 0;
}