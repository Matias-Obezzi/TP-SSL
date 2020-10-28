#include "utils/TinyExpr.c" //para realizar la operacion en caso de que sea una expresion aritmetica
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define NUMESTADOS 6 //El estado de rechazo no lo cuento
#define NUMCOLS 16 //cantidad de transiciones
#define NUMFINALES 2 //cantidad de estados finales
#define ERROR NUMESTADOS

typedef struct{
	int sigEstado;
} elementoMatriz;

elementoMatriz tabla[NUMESTADOS][NUMCOLS] = 
{
	{{0},{1},{1},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{6},{6},{6}},
	{{6},{6},{6},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{6},{6},{6}},
	{{5},{1},{1},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{3},{1},{4}},
	{{6},{6},{6},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{6},{6},{6}},
	{{6},{6},{6},{6},{2},{2},{2},{2},{2},{2},{2},{2},{2},{6},{6},{6}},
	{{5},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6}}
};

static char transiciones[] = {
	' ', '+', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '*', '/'
};

static int finales[] = {
	2, 5
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
	return final(estado)==1;
}


int main(int argc, char *argv[]){
	char palabra[200];
    if(argc!=2){
    	printf("Demasiados/pocos argumentos! \nPara continuar con la ejecucion se utilizara \"    -4235432.210+32.210    \" para demostrar el funcionamiento \n");
    	strcpy(palabra, "     -4235432.210+32.210   ");
    }else{
    	strcpy(palabra, argv[1]);
	}
	int reconoce = reconocer(palabra);
	printf("\n==============================================================================\n");
	printf("La cadena%s es una expresion aritmetica simple ", reconoce?"":" no");
	if(reconoce){
		//use la funcion te_interp de una biblioteca descargada que se encuentra en utils para devolver el valor dado que tmbn acepta operaciones entre floats y enteros
		printf("y su resultado es %f", te_interp(palabra, 0));
	}
	printf("\n==============================================================================\n");
	return 0;
}
