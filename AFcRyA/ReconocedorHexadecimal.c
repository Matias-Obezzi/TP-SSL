#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMESTADOS 12 //El estado de rechazo no lo cuento
#define NUMCOLS 31 //cantidad de transiciones
#define NUMFINALES 5 //cantidad de estados finales
#define ERROR NUMESTADOS

typedef struct{
	int sigEstado;
} elementoMatriz;

static elementoMatriz tabla[NUMESTADOS][NUMCOLS] = 
{
	{{0},{2},{3},{1},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12}},
	{{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{4},{4},{12},{12},{12},{12}},
	{{12},{12},{12},{1},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12}},
	{{12},{12},{12},{6},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12}},
	{{12},{12},{12},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{12},{12},{12},{12},{12},{12}},
	{{10},{12},{12},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{12},{12},{7},{7},{8},{8}},
	{{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{9},{9},{12},{12},{12},{12}},
	{{10},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{10},{10}},
	{{10},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{10},{10},{12},{12}},
	{{12},{12},{12},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{12},{12},{12},{12},{12},{12}},
	{{10},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12},{12}},
	{{10},{12},{12},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{12},{12},{12},{12},{11},{11}}
};

static char transiciones[NUMCOLS] = {
	' ', '+', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F', 'x', 'X', 'u', 'U', 'l', 'L'
};

static int finales[NUMFINALES] = {
	5, 7, 8, 10, 11
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
    	printf("Demasiados/pocos argumentos! \nPara continuar con la ejecucion se utilizara \"    0xA9uL    \" para demostrar el funcionamiento \n");
    	strcpy(palabra, "     0xA9uL    ");
//    	return 0;
    }else{
    	strcpy(palabra, argv[1]);
	}
	printf("\n======================================================================================\n");
	int reconocida = reconocer(palabra);
	printf("La cadena%s es una constante entera hexadecimal", reconocida?"":" no");
	if(reconocida){
		printf(" y su valor transformado a decimal es %d", strtol(palabra, NULL, 16)); // strtol(string, NULL, base);
	}
	printf("\n======================================================================================\n");
	return 0;
}
