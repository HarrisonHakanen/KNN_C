#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <math.h>


struct Centroid {
	int id;
	float x;
	float y;
	float distancia;
};


struct Registro {
	int id;
	float x;
	float y;
	Centroid classe;
};


struct RetornoCentroides {
	std::vector<Centroid> centroides;
	bool alterou;
};




using namespace std;

Centroid calculoDistancia(Registro registro, Centroid centroid);
vector<Registro> classificaRegistros(vector<Registro> registros, vector<Centroid> centroides);
vector<Registro> separaOsClusters(vector<Registro> registros, int indexReg);
RetornoCentroides reposicionaClusters(vector<Registro> registrosCluster, vector<Centroid>centroides);


int main() {

	int qtdCentroides = 3;

	vector<Registro> registros;

	Registro reg1 = { 0,2,2 };
	Registro reg2 = { 1,2,4 };
	Registro reg3 = { 2,4,2.5 };
	Registro reg4 = { 3,6,2.5 };
	Registro reg5 = { 4,8,1.5 };
	Registro reg6 = { 5,8.5,3.5 };
	Registro reg7 = { 6,9.8,4.5 };
	Registro reg8 = { 7,10,1.8 };
	Registro reg9 = { 8,11,3.2 };
	Registro reg10 = { 9,5,4.8 };
	Registro reg11 = { 11,6,4.5 };
	Registro reg12 = { 11,6.5,5 };
	Registro reg13 = { 12,7.3,5.3 };
	Registro reg14 = { 13,5.6,5.8 };
	Registro reg15 = { 14,6.6,6.3 };
	

	registros.push_back(reg1);
	registros.push_back(reg2);
	registros.push_back(reg3);
	registros.push_back(reg4);
	registros.push_back(reg5);
	registros.push_back(reg6);
	registros.push_back(reg7);
	registros.push_back(reg8);
	registros.push_back(reg9);
	registros.push_back(reg10);
	registros.push_back(reg11);
	registros.push_back(reg12);
	registros.push_back(reg13);
	registros.push_back(reg14);
	registros.push_back(reg15);


	bool alterouAlgumCentroid = true;
	vector<Centroid> centroides;

	for (int i = 0; i < qtdCentroides;i++) {

		float x = rand() % 11;
		float y = rand() % 4;

		Centroid centroid = {i,x,y, -1 };
		centroides.push_back(centroid);
	}

	while (alterouAlgumCentroid){

		registros = classificaRegistros(registros, centroides);


		vector<Centroid> novosCentroides;
		vector<Registro>jaPassou;
		for (int indexReg = 0; indexReg < registros.size(); indexReg++) {

			bool passou = false;
			for (int indexPassou = 0; indexPassou < jaPassou.size(); indexPassou++) {

				if (jaPassou[indexPassou].classe.id == registros[indexReg].classe.id) {
					passou = true;
				}
			}

			if (!passou) {


				vector<Registro> registrosCluster = separaOsClusters(registros, indexReg);
				
				if (registrosCluster.size() > 0) {

					RetornoCentroides retorno = reposicionaClusters(registrosCluster, centroides);					
					alterouAlgumCentroid = retorno.alterou;					
					centroides = retorno.centroides;
				}

			}

			jaPassou.push_back(registros[indexReg]);						
		}

	}
}


Centroid calculoDistancia(Registro registro, Centroid centroid) {

	centroid.distancia = sqrt(pow((registro.x - centroid.x), 2) + pow((registro.y - centroid.y), 2));

	return centroid;
}


vector<Registro> classificaRegistros(vector<Registro> registros,vector<Centroid> centroides) {


	for (int indexReg = 0; indexReg < registros.size(); indexReg++) {


		for (int indexCent = 0; indexCent < centroides.size(); indexCent++) {

			centroides[indexCent] = calculoDistancia(registros[indexReg], centroides[indexCent]);
		}

		int indexMenorCentroid = -1;
		float menorDistancia = -1;
		for (int indexCent = 0; indexCent < centroides.size(); indexCent++) {

			if (indexCent == 0) {

				indexMenorCentroid = indexCent;
				menorDistancia = centroides[indexCent].distancia;
			}
			else {
				if (centroides[indexCent].distancia < menorDistancia) {
					indexMenorCentroid = indexCent;
					menorDistancia = centroides[indexCent].distancia;
				}
			}
		}

		registros[indexReg].classe = centroides[indexMenorCentroid];
	}

	return registros;
}


vector<Registro> separaOsClusters(vector<Registro> registros, int indexReg) {

	vector<Registro> registrosCluster;
	registrosCluster.push_back(registros[indexReg]);

	if (indexReg + 1 < registros.size()) {

		for (int indexReg2 = indexReg + 1; indexReg2 < registros.size(); indexReg2++) {

			if (registros[indexReg].classe.id == registros[indexReg2].classe.id) {

				registrosCluster.push_back(registros[indexReg2]);
			}
		}
	}

	return registrosCluster;
}


RetornoCentroides reposicionaClusters(vector<Registro> registrosCluster,vector<Centroid>centroides) {
	
	float novoX = 0;
	float novoY = 0;
	bool alterou = false;

	for (int indexClus = 0; indexClus < registrosCluster.size(); indexClus++) {

		novoX += registrosCluster[indexClus].x;
		novoY += registrosCluster[indexClus].y;
	}

	novoX = novoX / registrosCluster.size();
	novoY = novoY / registrosCluster.size();

	for (int indexCent = 0; indexCent < centroides.size(); indexCent++) {

		if (registrosCluster[0].classe.id == centroides[indexCent].id) {

			if (centroides[indexCent].x != novoX || centroides[indexCent].y != novoY) {
				alterou = true;
			}
			centroides[indexCent].x = novoX;
			centroides[indexCent].y = novoY;
		}
	}

	RetornoCentroides retorno = { centroides,alterou };
	
	return retorno;
}