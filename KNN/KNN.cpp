#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <filesystem>


#define N 1
#define quantDimensoes 2


struct Centroid {
	int id;
	std::vector<float> dimensoes;	
	float distancia;
};


struct Registro {
	int id;
	std::vector<float> dimensoes;
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
std::vector<float>geraDimensoes();
void escreverArquivo(std::string nomeArquivo, auto conteudo);


int main() {		


	int qtdCentroides = 3;
	int qtdRegistros = 10;
	bool alterouAlgumCentroid = true;
		

	vector<Registro> registros;

	for (int i = 0; i < qtdRegistros; i++) {

		Registro reg = { i,geraDimensoes() };
		registros.push_back(reg);
	}


	vector<Centroid> centroides;

	for (int i = 0; i < qtdCentroides;i++) {

		Centroid centroid = {i,geraDimensoes(),-1};
		centroides.push_back(centroid);
	}


	while (alterouAlgumCentroid){

		registros = classificaRegistros(registros, centroides);

		cout << registros.size() << "\n";
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


	
	for (int indexReg = 0; indexReg < qtdRegistros; indexReg++) {

		std::string conteudo = "Registro" + to_string(indexReg) +"[";

		for (int indexDimen = 0; indexDimen < registros[indexReg].dimensoes.size();indexDimen++) {
			
			if (indexDimen < registros[indexReg].dimensoes.size()-1) {
			
				conteudo += to_string(registros[indexReg].dimensoes[indexDimen]) + "-";
			}else{
			
				conteudo += to_string(registros[indexReg].dimensoes[indexDimen]);
			}
			
		}
		conteudo += "[" + to_string(registros[indexReg].classe.id);

		escreverArquivo("Registros.txt", conteudo);
	}


	for (int indexCent = 0; indexCent < qtdCentroides; indexCent++) {

		std::string conteudo = "Centroide" + to_string(indexCent) + "[";

		for (int indexDimen = 0; indexDimen < centroides[indexCent].dimensoes.size(); indexDimen++) {

			if (indexDimen < centroides[indexCent].dimensoes.size() - 1) {

				conteudo += to_string(centroides[indexCent].dimensoes[indexDimen])+"-";
			}
			else {

				conteudo += to_string(centroides[indexCent].dimensoes[indexDimen]);
			}			
		}
		
		escreverArquivo("Centroides.txt", conteudo);
	}
}


void escreverArquivo(std::string nomeArquivo, auto conteudo) {

	if (std::filesystem::exists(nomeArquivo)) {

		std::ofstream arquivoCadastro(nomeArquivo, std::ios::app);

		if (arquivoCadastro.is_open()) {
			arquivoCadastro << conteudo << "\n";
			arquivoCadastro.close();
		}

	}
	else {
		std::ofstream arquivoCadastro(nomeArquivo);
		arquivoCadastro << conteudo << "\n";
		arquivoCadastro.close();
	}

}



std::vector<float>geraDimensoes() {
	
	std::vector<float> dimensoes;
	for (int i = 0; i < quantDimensoes; i++) {
		dimensoes.push_back((float)rand() / (float)(RAND_MAX / N));
	}
	return dimensoes;
}


Centroid calculoDistancia(Registro registro, Centroid centroid) {

	float distancia = 0;
	float potencia = 0;
	
	for (int i = 0; i < quantDimensoes; i++) {

		potencia += pow(registro.dimensoes[i] - centroid.dimensoes[i],2);
	}
	centroid.distancia = sqrt(potencia);	

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
	

	bool alterou = false;
	std::vector<float>dimensoesVar;


	for (int i = 0; i < quantDimensoes; i ++) {
		dimensoesVar.push_back(0);
	}

	//Somatória de todas as dimensoes de cada registro.
	for (int indexClus = 0; indexClus < registrosCluster.size(); indexClus++) {

		for (int indexDimen = 0; indexDimen < registrosCluster[indexClus].dimensoes.size();indexDimen++) {

			float valor = registrosCluster[indexClus].dimensoes[indexDimen];
			dimensoesVar[indexDimen] += valor;
		}		
	}
	
	//Média das somatórias de cada registro.
	for (int indexDimen = 0; indexDimen < quantDimensoes; indexDimen++) {

		dimensoesVar[indexDimen] = dimensoesVar[indexDimen] / registrosCluster.size();		
	}

	//Reposiciona os cluster com base na media.
	for (int indexCent = 0; indexCent < centroides.size(); indexCent++) {

		//A indexação sempre é 0 pois esse vetor sempre vai pertencer a um cluster
		if (registrosCluster[0].classe.id == centroides[indexCent].id) {


			for (int indexDimen = 0; indexDimen < quantDimensoes; indexDimen++) {

				if (centroides[indexCent].dimensoes[indexDimen] != dimensoesVar[indexDimen]) {

					alterou = true;
				}
				centroides[indexCent].dimensoes[indexDimen] = dimensoesVar[indexDimen];
			}			
		}
	}

	RetornoCentroides retorno = { centroides,alterou };
	
	return retorno;
}