#include "network.h"

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
		y += net->layers[layer - 1].w[i*sx +x] * net->layers[layer - 1].x[i];
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
		y += net->layers[layer].w[x*sx +i] * net->layers[layer + 1].delta[i];
	}
	return y;
}

void printNetwork(Network *net) {
	printf("--------------------------------------------------------------\n");
	int sx = net->size_x;
	printf("Entrees:\n");
	for (int node = 0; node < net->nbInput; ++node)
	{
		printf("x%d : %f\n", node, net->layers[0].x[node]);
		for (int i = 0; i < net->size_x; ++i)
		{
			printf("	w%d,%d : %f\n",
			 node, i, net->layers[0].w[node*sx +i]);
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
				 node, i, net->layers[layer].w[node*sx +i]);
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
			 net->nbLayer-2,node,i,net->layers[net->nbLayer-2].w[node*sx+i]);
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
		net->layers[nl-1].delta[node]=exemple[i].d-net->layers[nl-1].x[node];
	}
	for (int l = net->nbLayer - 2; l > 0; --l)
	{
		for (int node = 0; node < net->size_x; ++node)
		{
			double deriv=fonctionActivationDerivative(net->layers[l].x[node]);
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
				net->layers[l].w[node*sx+i]+= net->taux*x_node*delta_node;
			}
		}
	}
	for (int node = 0; node < net->size_x; ++node)
	{
		for (int i = 0; i < net->nbOutput; ++i)
		{
			double x_node = net->layers[net->nbLayer - 2].x[node];
			double delta_node = net->layers[net->nbLayer - 2 + 1].delta[i];
		net->layers[net->nbLayer-2].w[node*sx+i]+=net->taux*x_node*delta_node;
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
					fprintf(file,"(%d;%d;%d)=%f\n",
						0,node,i,net.layers[0].w[node*sx+i]);
				}
			}
		for (int l = 1; l < net.nbLayer - 2; ++l)
		{
			for (int node = 0; node < sx; ++node)
			{
				for (int i = 0; i < sx; ++i)
				{
					fprintf(file,"(%d;%d;%d)=%f\n",
						l,node,i,net.layers[l].w[node*sx+i]);
				}
			}
		}
		for (int node = 0; node < sx; ++node)
		{
			for (int i = 0; i < net.nbOutput; ++i)
			{
				fprintf(file,"(%d;%d;%d)=%f\n",
				net.nbLayer-2,node,i,net.layers[net.nbLayer-2].w[node*sx+i]);
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

void allocNet(Network *net)
{
	int i;
	net->layers = malloc(net->nbLayer * sizeof(Layers));
	for (i = 0; i < net->nbLayer; i++) {
		net->layers[i].w = malloc(net->size_x*net->size_x * sizeof(double));
		net->layers[i].x = malloc(net->size_x * sizeof(double));
		net->layers[i].delta = malloc(net->size_x * sizeof(double));
	}
}

void freeNet(Network *net)
{
	int i;
	for (i = 0; i < net->nbLayer; i++) {
		free(net->layers[i].w);
		free(net->layers[i].x);
		free(net->layers[i].delta);
	}
	free(net->layers);
}