#pragma once
#include<thread>
#include <stdio.h>    
#include <stdlib.h> 


#include<iostream>
#include <chrono>
#include<time.h>

#include <iomanip>
#include <fstream>


#include<math.h>

using namespace std;

#define xfac 2.0f
#define mutationRate 0.05		//<1
#define layerCount 4			// layerCount 

class brain
{
public:
	struct layer
	{
		int in;
		int out;
		float w[40][540];
		long float oval[540];
		float bias[540];

	};

	long double score;

	layer input, h1, h2, output;

	layer l[layerCount + 1];

	float w[40][540][layerCount + 1];
	float b[540][layerCount + 1];

	double dCw[40][540][layerCount + 1];
	double dCb[540][layerCount + 1];

	int backPropCount=0;

	void initialize() {

		printf("\n\n\_____initializing _______\n\n");
		input.in = 540;
		input.out = 40;

		for (int i = 0; i < input.in; i++) {
			for (int j = 0; j < input.out; j++) {
				input.w[j][i] = rand() % 2000 / 100.0f - 10.0f;
			}
			input.bias[i] = rand() % 2000 / 100.0f - 10.0f;
		}

		h1.in = 40;
		h1.out = 40;

		for (int i = 0; i < h1.in; i++) {
			for (int j = 0; j < h1.out; j++) {
				h1.w[j][i] = rand() % 2000 / 100.0f - 10.0f;

			}
			h1.oval[i] = 1;
			h1.bias[i] = rand() % 2000 / 100.0f - 10.0f;
		}

		h2.in = 40;
		h2.out = 1;

		for (int i = 0; i < h2.in; i++) {
			for (int j = 0; j < h2.out; j++) {
				h2.w[j][i] = rand() % 2000 / 100.0f - 10.0f;
			}
			h2.oval[i] = 1;
			h2.bias[i] = rand() % 2000 / 100.0f - 10.0f;
		}

		output.in = 1;
		output.out = 1;

		for (int i = 0; i < output.in; i++) {
			for (int j = 0; j < output.out; j++) {
				output.w[j][i] = rand() % 2000 / 100.0f - 10.0f;
			}
			output.oval[i] = 1;
			output.bias[i] = rand() % 2000 / 100.0f - 10.0f;
		}

		l[1] = input;
		l[2] = h1;
		l[3] = h2;
		l[4] = output;


		for (int n = 1; n < layerCount + 1; n++) {
			for (int j = 0; j < l[n].in; j++) {
				for (int i = 0; i < l[n].out; i++) {
					w[i][j][n] = l[n].w[i][j];
					dCw[i][j][n] = 0;
				}
				b[j][n] = l[n].bias[j];
				dCb[j][n] = 0;
			}
		}


		/*
		for (int n = 1; n < layerCount + 1; n++) {
			for (int j = 0; j < l[n].in; j++) {
				for (int i = 0; i < l[n].out; i++) {

					printf("\nw[%d][%d][%d]=%5f", i, j, n, l[n].w[i][j]);

				}

				printf("\nb[%d][%d]=%5f", j, n, l[n].bias[j]);
			}
		}
		*/
		printf("\n\n\_____initializing  complete_______\n\n");
	}




	long float sigmoid(long float val) {
		return(1 / (1 + 1 / exp(val)));
	}

	void thinK(float inp[], float outp[]) {		//float inp[]

		//printf("\n\n\_____THINKING _______\n\n");
												//k == l[1].in & k1 == l[4].out
		for (int i = 0; i < l[1].in; i++) {
			l[1].oval[i] = inp[i];
		}
		//	printf("\n\n\_____THINKING F1_______\n\n");
		layerCalc(l[1], l[2], l[2].oval);
		//	printf("\n\n\_____THINKING F2_______\n\n");
		layerCalc(l[2], l[3], l[3].oval);
		//	printf("\n\n\_____THINKING F3_______\n\n");
		layerCalc(l[3], l[4], l[4].oval);
		//	printf("\n\n\_____THINKING F4_______\n\n");
		for (int i = 0; i < l[4].out; i++) {
			outp[i] = l[4].oval[i];
		}

		/*
		l[1] = input;
		l[2] = h1;
		l[3] = h2;
		l[4] = output;
		*/
		//printf("\n\n\_____THINKING DONE_______\n\n");
	}
	void layerCalc(layer inp, layer outp, long float oval[]) {
		long float val = 0.0f;
		for (int i = 0; i < outp.in; i++) {
			val = 0.0f;
			for (int j = 0; j < inp.in; j++) {
				val += inp.oval[j] * inp.w[i][j];

			}
			val += outp.bias[i];
			oval[i] = sigmoid(val);

		}

	}

	void mutation() {

		for (int i = 0; i < input.in; i++) {
			for (int j = 0; j < input.out; j++) {
				if (rand() % 1000 / 1000.0f < mutationRate)
					input.w[j][i] += rand() % (int)(xfac * 1000) / 1000.0f - xfac / 2.0f;
			}
		}



		for (int i = 0; i < h1.in; i++) {
			for (int j = 0; j < h1.out; j++) {
				if (rand() % 1000 / 1000.0f < mutationRate)
					h1.w[j][i] += rand() % (int)(xfac * 1000) / 1000.0f - xfac / 2.0f;

			}

		}



		for (int i = 0; i < h2.in; i++) {
			for (int j = 0; j < h2.out; j++) {
				if (rand() % 1000 / 1000.0f < mutationRate)
					h2.w[j][i] += rand() % (int)(xfac * 1000) / 1000.0f - xfac / 2.0f;
			}

		}



		for (int i = 0; i < output.in; i++) {
			for (int j = 0; j < output.out; j++) {
				if (rand() % 1000 / 1000.0f < mutationRate)
					output.w[j][i] += rand() % (int)(xfac * 1000) / 1000.0f - xfac / 2.0f;
			}

		}

	}


	void backProp(float y[],int count) {
		//printf("\n\n\_____backprop _______\n\n");
		double da[540][layerCount + 1];
		double dw[40][540][layerCount + 1];
		double db[540][layerCount + 1];

		

		if (backPropCount == count) {
			for (int n = 1; n < layerCount + 1; n++) {
				for (int j = 0; j < l[n].in; j++) {
					for (int i = 0; i < l[n].out; i++) {
						l[n].w[i][j] -= dCw[i][j][n]*0.2f/count;
						dCw[i][j][n] = 0;
						//printf("\n___dw %5f\n", dw[i][j][n]);
					}
					l[n].bias[j] -= dCb[j][n]*0.2f /count;
					dCb[j][n] = 0;
				}
			}
			backPropCount = 0;
		}
		else {
			backPropCount++;
		}





		for (int i = 0; i < l[layerCount].out; i++) {
			da[i][layerCount] = 2 * (l[layerCount].oval[i] - y[i]);

		}

		for (int k = 1; k < layerCount; k++) {
			for (int i = 0; i < l[layerCount - k].out; i++) {

				double dan = 0;
				for (int n = 0; n < l[layerCount - k + 1].out; n++) {
					dan += da[n][layerCount - k + 1];
				}
				for (int j = 0; j < l[layerCount - k].in; j++) {
					da[j][layerCount - k] = 0;
				}

				for (int j = 0; j < l[layerCount - k].in; j++) {
					dw[i][j][layerCount - k] = l[layerCount - k].oval[j] * l[layerCount - k + 1].oval[i] * (1 - l[layerCount - k + 1].oval[i]) * dan;
					da[j][layerCount - k] += w[i][j][layerCount - k] * l[layerCount - k + 1].oval[i] * (1 - l[layerCount - k + 1].oval[i]) * dan;
					//printf("\n___da %5f\n", da[j][layerCount - k]);
				}
				db[i][layerCount - k + 1] = l[layerCount - k + 1].oval[i] * (1 - l[layerCount - k + 1].oval[i]) * dan;
			}

		}

		for (int n = 1; n < layerCount + 1; n++) {
			for (int j = 0; j < l[n].in; j++) {
				for (int i = 0; i < l[n].out; i++) {
					dCw[i][j][n] += dw[i][j][n];
				}
				dCb[j][n] += db[j][n];
			}
		}


		








	}

	int sign(float val) {
		if (val >= 0)return (1);
		else return (-1);
	}


	void saveToFile() {

		for (int n = 1; n < layerCount + 1; n++) {
			for (int j = 0; j < l[n].in; j++) {
				for (int i = 0; i < l[n].out; i++) {
					w[i][j][n] = l[n].w[i][j];
				}
				b[j][n] = l[n].bias[j];
			}
		}

		for (int i = 0; i < l[1].in; i++) {
			b[i][1] = 0;
		}
		for (int i = 0; i < l[layerCount].out; i++)
			for (int j = 0; j < l[layerCount].in; j++) {
				w[i][j][layerCount] = 0;
			}

		ofstream outFile;
		outFile.open("weights.txt");
		outFile.close();



		outFile.open("weights.txt", ios::app);



		for (int n = 1; n < layerCount + 1; n++) {
			for (int j = 0; j < l[n].in; j++) {
				for (int i = 0; i < l[n].out; i++) {

					outFile << w[i][j][n];
					printf("\nw[%d][%d][%d]=%5f", i, j, n, w[i][j][n]);
					outFile << " \n ";
				}
				outFile << b[j][n];
				printf("\nb[%d][%d]=%5f", j, n, b[j][n]);
				outFile << " \n ";
			}
		}

		outFile.close();
	}

	void loadFromFile() {

		ifstream inFile;
		inFile.open("weights.txt");
		if (!inFile) {
			cout << "Unable to open file";
			initialize();
		}
		else {
			initialize();
			for (int n = 1; n < layerCount + 1; n++) {
				for (int j = 0; j < l[n].in; j++) {
					for (int i = 0; i < l[n].out; i++) {

						inFile >> w[i][j][n];
						printf("\nw[%d][%d][%d]=%5f", i, j, n, w[i][j][n]);
					}
					inFile >> b[j][n];
					printf("\nb[%d][%d]=%5f", j, n, b[j][n]);
				}
			}
			inFile.close();
			printf("\n\n\n");
			for (int n = 1; n < layerCount + 1; n++) {
				for (int j = 0; j < l[n].in; j++) {
					for (int i = 0; i < l[n].out; i++) {
						l[n].w[i][j] = w[i][j][n];
						printf("\nw[%d][%d][%d]=%5f", i, j, n, l[n].w[i][j]);

					}
					l[n].bias[j] = b[j][n];
					printf("\nb[%d][%d]=%5f", j, n, l[n].bias[j]);
				}
			}


		}



	}











	float sigmDeriv(float val) {

		return(val * (1 - val));
	}

};



