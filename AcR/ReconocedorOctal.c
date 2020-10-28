#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMESTADOS 10 //El estado de rechazo no lo cuento
#define NUMCOLS 15 //cantidad de transiciones
#define NUMFINALES 5 //cantidad de estados finales
#define ERROR NUMESTADOS

typedef struct{
	int sigEstado;
} elementoMatriz;

static elementoMatriz tabla[NUMESTADOS][NUMCOLS] = 
{
	{{0},{2},{3},{1},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10}},
	{{10},{10},{10},{4},{4},{4},{4},{4},{4},{4},{4},{10},{10},{10},{10}},
	{{10},{10},{10},{1},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10}},
	{{10},{10},{10},{5},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10}},
	{{10},{10},{10},{4},{4},{4},{4},{4},{4},{4},{4},{6},{6},{7},{7}},
	{{10},{10},{10},{8},{8},{8},{8},{8},{8},{8},{8},{10},{10},{10},{10}},
	{{9},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{9},{9}},
	{{9},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{9},{9},{10},{10}},
	{{10},{10},{10},{8},{8},{8},{8},{8},{8},{8},{8},{10},{10},{9},{9}},
	{{9},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10},{10}}
};

static char transiciones[NUMCOLS] = {
	' ', '+', '-', '0', '1', '2', '3', '4', '5', '6', '7', 'u', 'U', 'l', 'L'
};

static int finales[NUMFINALES] = {
	4, 6, 7, 8, 9
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
    	printf("Demasiados/pocos argumentos! \nPara continuar con la ejecucion se utilizara \"    -0777l    \" para demostrar el funcionamiento \n");
    	strcpy(palabra, "     -0777l    ");
//    	return 0;
    }else{
    	strcpy(palabra, argv[1]);
	}
	printf("\n=================================================================================\n");
	int reconocida = reconocer(palabra);
	printf("La cadena%s es una constante entera octal", reconocida?"":" no");
	if(reconocida){
		printf(" y su valor transformado a decimal es %d", strtol(palabra, NULL, 8)); // strtol(string, NULL, base);
	}
	printf("\n=================================================================================\n");
	return 0;
}
