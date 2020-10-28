#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMESTADOS 8 //El estado de rechazo no lo cuento
#define NUMCOLS 17 //cantidad de transiciones
#define NUMFINALES 5 //cantidad de estados finales
#define ERROR NUMESTADOS

typedef struct{
	int sigEstado;
} elementoMatriz;

static elementoMatriz tabla[NUMESTADOS][NUMCOLS] = 
{
	{{0},{3},{1},{8},{4},{4},{4},{4},{4},{4},{4},{4},{4},{8},{8},{8},{8}},
	{{8},{8},{8},{8},{2},{2},{2},{2},{2},{2},{2},{2},{2},{8},{8},{8},{8}},
	{{7},{8},{8},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{8},{8},{7},{7}},
	{{8},{8},{8},{8},{4},{4},{4},{4},{4},{4},{4},{4},{4},{8},{8},{8},{8}},
	{{7},{8},{8},{4},{4},{4},{4},{4},{4},{4},{4},{4},{4},{5},{5},{6},{6}},
	{{7},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{7},{7}},
	{{7},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{7},{7},{8},{8}},
	{{7},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8}}
};

static char transiciones[NUMCOLS] = {
	' ', '+', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'u', 'U', 'l', 'L'
};

static int finales[NUMFINALES] = {
	2, 4, 5, 6, 7
};

int buscarColumna(char letra){
	int i;
	for(i = 0; i<NUMCOLS; i++){
		if(letra==transiciones[i]){
			return i;
		}
	}
	return -1;
}

int final(int estado){
	int i;
	for(i=0; i<NUMFINALES; i++){
		if(finales[i]==estado){
			return 1;
		}
	}
	return 0;
}

int reconocer(char palabra[]){
	char letra = palabra[0];
	int estado;
	elementoMatriz temp;
	int i = 0;
	do{
		letra = palabra[i];
		int col = buscarColumna(letra);
		if(col!=-1){
			temp = tabla[estado][col];
			estado = temp.sigEstado;
			i+=1;
		}else{
			estado = ERROR;
		}
	}while(palabra[i] != '\0' && estado != ERROR);
	return final(estado);
}

int main(int argc, char *argv[]){
	char palabra[200];
    if(argc!=2){
    	printf("Demasiados/pocos argumentos! \nPara continuar con la ejecucion se utilizara \"    +423543ul    \" para demostrar el funcionamiento \n");
    	strcpy(palabra, "     +423543ul    ");
//    	return 0;
    }else{
    	strcpy(palabra, argv[1]);
	}
	printf("\n======================================================================\n");
	int reconocida = reconocer(palabra);
	printf("La cadena%s es una constante entera decimal", reconocida?"":" no");
	if(reconocida){
		printf(" y su valor decimal es %d", atoi(palabra));
	}
	printf("\n======================================================================\n");
	return 0;
}
