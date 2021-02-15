#include "network.h"

int main() {
	Network net;
	Exemple exemple[NB_EXEMPLES];

	net.nbLayer = 3;
	net.nbInput = 2;
	net.nbOutput = 1;
	net.size_x = 4;
	net.taux = 0.1;

	allocNet(&net);

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

	printf("Apprentissage de la fonction XOR...\n");
	double total_error = 1;
	while (total_error>0.0004)
	{
		total_error = 0;
		for (int i = 0; i < NB_EXEMPLES; ++i)
		{
			propagation(&net, i, exemple);	//Propagation jusqu'à la sortie

			total_error+=fabs(exemple[i].d - net.layers[net.nbLayer - 1].x[0]);

			back_propagation(&net, i, exemple); //back_prop jusqu'aux entrées

			MAJweights(&net);	//Mise à jour des poids
		}
	}
	printf("Sauvegarde des poids dans weights.txt ...\n");
	saveWeights(net);

	printf("Vous pouvez maintenant tester la fonction XOR\n");
	while(1){
		printf("x0 = ");
		scanf("%lf",&exemple[0].x[0]);
		printf("x1 = ");
		scanf("%lf",&exemple[0].x[1]);

		utilisation(&net, 0, exemple);
	}


	freeNet(&net);
	return 0;
}