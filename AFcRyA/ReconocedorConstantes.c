#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMESTADOS 16 //El estado de rechazo no lo cuento
#define NUMCOLS 31 //cantidad de transiciones
#define NUMFINALES 9 //cantidad de estados finales
#define ERROR NUMESTADOS

typedef struct{
	int sigEstado;
} elementoMatriz;

static elementoMatriz tabla[NUMESTADOS][NUMCOLS] = 
{
	{{0},{4},{3},{1},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16}},
	{{16},{16},{16},{2},{2},{2},{2},{2},{2},{2},{2},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{5},{5},{16},{16},{16},{16}},
	{{13},{16},{16},{2},{2},{2},{2},{2},{2},{2},{2},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{14},{14},{12},{12}},
	{{16},{16},{16},{7},{6},{6},{6},{6},{6},{6},{6},{6},{6},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16}},
	{{16},{16},{16},{1},{8},{8},{8},{8},{8},{8},{8},{8},{8},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16}},
	{{16},{16},{16},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{16},{16},{16},{16},{16},{16}},
	{{13},{16},{16},{6},{6},{6},{6},{6},{6},{6},{6},{6},{6},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{13},{13}},
	{{16},{16},{16},{15},{15},{15},{15},{15},{15},{15},{15},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{10},{10},{16},{16},{16},{16}},
	{{13},{16},{16},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{14},{14},{12},{12}},
	{{13},{16},{16},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{9},{16},{16},{14},{14},{12},{12}},
	{{16},{16},{16},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{16},{16},{16},{16},{16},{16}},
	{{13},{16},{16},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{11},{16},{16},{16},{16},{13},{13}},
	{{13},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{13},{13},{16},{16}},
	{{13},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16}},
	{{13},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{13},{13}},
	{{13},{16},{16},{15},{15},{15},{15},{15},{15},{15},{15},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{13},{13}}
};

static char transiciones[NUMCOLS] = {
	' ', '+', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F', 'x', 'X', 'u', 'U', 'l', 'L'
};

static int finales[NUMFINALES] = {
	2, 6, 8, 9, 11, 12, 13, 14, 15
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

int tipos(char palabra[]){
	int i = 0;
	while(palabra[i]!='\0'){
		if(palabra[i]!=' '){
			if(palabra[i]=='+' || palabra[i]=='-'){
				if(palabra[i+1]=='0'){
					return palabra[i+2] == 'x' || palabra[i+2] == 'X' ? 1 : 2;
				}else{
					return 0;
				}
			}else if(palabra[i]=='0'){
				if(palabra[i+1]=='X' || palabra[i+1]=='x'){
					return 1;
				}else{
					return 2;
				}
			}else{
				return 0;
			}
		}
		i++;
	}
	return 3;
}
int main(int argc, char *argv[]){
	char palabra[200];
    if(argc!=2){
    	printf("Demasiados/pocos argumentos! \nPara continuar con la ejecucion se utilizara \"    +0xAb0uL    \" para demostrar el funcionamiento \n");
    	strcpy(palabra, "    +0xAb0uL    ");
//    	return 0;
    }else{
    	strcpy(palabra, argv[1]);
	}
	printf("\n==========================================================================================\n");
	int reconoce = reconocer(palabra);
	printf("La cadena%s una constante entera descripta en la gramatica de C\n", reconoce?"":" no");
	if(reconoce){
		char tipo[12];
		switch(tipos(palabra)){
			case 0:
				printf("Es de tipo %s y su valor decimal es %d", "decimal", atoi(palabra));
				break;
			case 1:
				printf("Es de tipo %s y su valor transformado en decimal es %d", "hexadecimal", strtol(palabra, NULL, 16));
				break;
			case 2:
				printf("Es de tipo %s y su valor transformado en decimal es %d", "octal", strtol(palabra, NULL, 8));
				break;
			default:
				printf("Su tipo no fue reconocido");
		}
	}
	printf("\n==========================================================================================\n");
	return 0;
}
