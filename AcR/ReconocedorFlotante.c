#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMESTADOS 6 //El estado de rechazo no lo cuento
#define NUMCOLS 14 //cantidad de transiciones
#define NUMFINALES 3 //cantidad de estados finales
#define ERROR NUMESTADOS

typedef struct{
	int sigEstado;
} elementoMatriz;

static elementoMatriz tabla[NUMESTADOS][NUMCOLS] = 
{
	{{0},{1},{1},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{6}},
	{{6},{6},{6},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2 },{6}},
	{{3},{6},{6},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{4}},
	{{3},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6}},
	{{6},{6},{6},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{6}},
	{{3},{6},{6},{5},{5},{5},{5},{5},{5},{5},{5},{5},{5},{6}}
};

static char transiciones[NUMCOLS] = {
	' ', '+', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'
};

static int finales[NUMFINALES] = {
	2, 3, 5
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
    	printf("Demasiados/pocos argumentos! \nPara continuar con la ejecucion se utilizara \"    -423543.2210    \" para demostrar el funcionamiento \n");
    	strcpy(palabra, "     -423543.2210    ");
//    	return 0;
    }else{
    	strcpy(palabra, argv[1]);
	}
	printf("\n===================================================================\n");
	int reconocida = reconocer(palabra);
	printf("La cadena%s es de tipo flotante", reconocida?"":" no");
	if(reconocida){
		printf(" y su valor flotante es %f", atof(palabra));
	}
	printf("\n===================================================================\n");
	return 0;
}
