#include <stdio.h> //Biblioteca de c
#include <string.h> //Biblioteca que reconoce stings
#include <ctype.h> //Biblioteca que reconoce los typedef


//Para declarar las constantes
#define NUMESTADOS 15
#define NUMCOLS 13
#define TAMLEX 32+1 //El +1 es por el /0
#define TAMNOM 20+1


//Para declarar variables globales
FILE * in;

typedef enum { //enumera todos los textos que lo conforman
	INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQ, PARENDER, 
	PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT, ERRORLEXICO
} TOKEN;
//Toma el nombre de token para tomarlo como un tipo de dato nuevo

typedef struct{
	char identifi[TAMLEX];
	TOKEN t; /* t=0, 1, 2, 3 Palabra Reservada, t=ID=4 Identificador */
} RegTS;

RegTS TS[1000] = { {"inicio", INICIO}, {"fin", FIN}, {"leer", LEER}, {"escribir", ESCRIBIR}, {"$", 99} };

typedef struct{
	TOKEN clase;
	char nombre[TAMLEX];
	int valor;
} REG_EXPRESION; //Crea un nuevo tipo de dato llamado REG_EXPRESION

char buffer[TAMLEX];

TOKEN tokenActual;

int flagToken = 0; //Crea una bandera


//Para declarar los prototipos de las funciones 
TOKEN scanner();
int columna(int c);
int estadoFinal(int e);
void Objetivo(void);
void Programa(void);
void ListaSentencias(void);
void Sentencia(void);
void ListaIdentificadores(void);
void Identificador(REG_EXPRESION * presul);
void ListaExpresiones(void);
void Expresion(REG_EXPRESION * presul);
void Primaria(REG_EXPRESION * presul);
void OperadorAditivo(char * presul);
REG_EXPRESION ProcesarCte(void);
REG_EXPRESION ProcesarId(void);
char * ProcesarOp(void);
void Leer(REG_EXPRESION in);
void Escribir(REG_EXPRESION out);
REG_EXPRESION GenInfijo(REG_EXPRESION e1, char * op, REG_EXPRESION e2);
void Match(TOKEN t);
TOKEN ProximoToken();
void ErrorLexico();
void ErrorSintactico();
void Generar(char * co, char * a, char * b, char * c);
char * Extraer(REG_EXPRESION * preg);
int Buscar(char * id, RegTS * TS, TOKEN * t);
void Colocar(char * id, RegTS * TS);
void Chequear(char * s);
void Comenzar(void);
void Terminar(void);
void Asignar(REG_EXPRESION izq, REG_EXPRESION der);


//Comenzando el programa 
int main (int argc, char* argv[]) {
	
	/*Cuando en la linea de comando escribamos por ejemplo COMPILAR MIARCHIVO, 
	argc corresponde a la cantidad de argumentos mientas que argv tiene en cada
	una de sus posiciones un argumento*/
	
	TOKEN tok;
	char nomArchi[TAMNOM];
	int l;
	
	if (argc == 1) { //Corroboramos que nos hayan pasado la accion y un archivo para poder compilar
		printf ("Debe ingresar el nombre del archivo fuente (en lenguaje micro) en la linea de comando\n");
		return -1; //Como detecto un error, directamente termina el programa
	}
	
	if ( argc != 2 ) { //Corroboramos que nos hayan pasado solamente un archivo para compilar y no mas de uno
 		printf("Numero incorrecto de argumentos\n");
 		return -1;
	}

	strcpy(nomArchi, argv[1]); //En una nueva variable declarada al inicio del main, se guarda el nombre del archivo almacenado en argv
	l = strlen(nomArchi); //En lugar de darle un cierto numero para el vector, obtenemos desde una funcion su tamaño

	if ( l > TAMNOM ) { //Verificamos que el 
 		printf("Nombre incorrecto del Archivo Fuente\n");
 		return -1;
	}

	if ( nomArchi[l-1] != 'm' || nomArchi[l-2] != '.' ) { //Corroboramos que el archivo que figura esta en lenguaje micro
 		printf("Nombre incorrecto del Archivo Fuente\n");
 		return -1;
	}
	
	if ( (in = fopen(nomArchi, "r") ) == NULL ) { //Verificamos que el archivo se pueda leer
 		printf("No se pudo abrir archivo fuente\n");
 		return -1;
	}
	
	objetivo (); //Comienza a leer el archivo
	fclose (in); //Termina cerrando el archivo
	return 0;
}	


//PROCEDIMIENTOS DE ANALISIS SINTACTICO (PAS)___________________________________________
//OBJETIVO_______________________________________________________________
void Objetivo(void) {
	Programa(); //funcion PAS
	Match(FDT); //Corroborar que el token recibido es ese
	Terminar(); //Terminar la lectura del archivo
}

//PROGRAMA_______________________________________________________________
void Programa(void) {
	Comenzar(); //Inicia el programa
	Match(INICIO); //Corrobora el token recibido
	ListaSentencias(); //Funcion PAS
	Match(FIN); //Corrobora el token recibido
}

//LISTA DE SENTENCIAS____________________________________________________
void ListaSentencias(void){
	Sentencia(); //Funcion PAS
	while ( 1 ) //Comienza un bucle que permite leer cada sentencia de una lista de sentencias
	{
		switch ( ProximoToken() ){ //Compara el token con alguno de los casos de acabo
			case ID : 
			case LEER : 
			case ESCRIBIR :	
				Sentencia(); //En el caso que sea ID, LEER o ESCRIBIR (las cuales son palabras reservadas), va a la funcion PAS
				break;
			default : return; //Caso contrario, termina
		}
	}
}

//SENTENCIA______________________________________________________________
void Sentencia(void){
	TOKEN tok = ProximoToken(); //Crea una variable que almacena lo que retorna la funcion auxiliar ProximoToken()
	REG_EXPRESION izq, der; //Crea dos variables de la estructura REG_EXPRESION
	switch ( tok ){ 
	
		//Caso que el token sea un identificador
		 case ID :
			 Identificador(&izq); //Verifica que es un identificador
			 Match(ASIGNACION); //Corribora que se haya recibido el token correcto
			 Expresion(&der); //Utiliza otra función PAS para analizar la otra parte 
			 Asignar(izq, der); //Se llama a la función Asignar que más adelante explicaremos
			 Match(PUNTOYCOMA); 
			 break; //Se termina el análisis 
			 
		//Caso que el token sea la palabra reservar LEER
		 case LEER : 
			 Match(LEER);
			 Match(PARENIZQ); //Corrobora dos tokens antes de iniciar 
			 ListaIdentificadores(); //Llama a una función PAS
			 Match(PARENDER);
			 Match(PUNTOYCOMA);
			 break; //Vuelve a corroborar y termina esta parte del análisis 
			 
		//Caso que el token sea la palabra reservar ESCRIBIR
		 case ESCRIBIR :
			 Match(ESCRIBIR);
			 Match(PARENIZQ);
			 ListaExpresiones();
			 Match(PARENDER);
			 Match(PUNTOYCOMA);
			 break; //Ocurre lo mismo que en el caso de LEER pero se llama a una función PAS distinta
			 
		//Si no se cumple ninguno de los casos termina la funcion	 
		 default : return;
	}
}

//LISTA DE IDENTIFICADORES_______________________________________________
void ListaIdentificadores(void){
	TOKEN t;
	REG_EXPRESION reg;
	Identificador(&reg);
	Leer(reg);
	for ( t = ProximoToken(); t == COMA; t = ProximoToken() ){
		Match(COMA);
		Identificador(&reg);
		Leer(reg);
	} //Mientras que el token recibido sea una coma, se verifica que tiene que recibir una coma y un identificador y el mismo lo lee
}

//IDENTIFICADOR__________________________________________________________
void Identificador(REG_EXPRESION * presul){
	Match(ID);
	*presul = ProcesarId(); //matchea y el puntero queda con lo que retorna la función llamada
}

//LISTA DE EXPRESIONES___________________________________________________
void ListaExpresiones(void){
	TOKEN t;
	REG_EXPRESION reg;
	Expresion(&reg);
	Escribir(reg);
	for ( t = ProximoToken(); t == COMA; t = ProximoToken() ){
		Match(COMA);
		Expresion(&reg);
		Escribir(reg);
	} //Ocurre lo mismo que lista de identificadores
}

//EXPRESION______________________________________________________________
void Expresion(REG_EXPRESION * presul){
	REG_EXPRESION operandoIzq, operandoDer;
	char op[TAMLEX];
	TOKEN t;
	Primaria(&operandoIzq);
	for ( t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken() ){
		OperadorAditivo(op);
		Primaria(&operandoDer); //Se llama para saber que tipo de elemento se le pasa a la función 
		operandoIzq = GenInfijo(operandoIzq, op, operandoDer);
	}
	*presul = operandoIzq;
} //Se analiza expresión por expresión corroborando que el token utilizado para dividir ambas partes, sea el de suma o el de resta

//PRIMARIA_______________________________________________________________
void Primaria(REG_EXPRESION * presul){
	TOKEN tok = ProximoToken();
	switch ( tok ){
	
		case ID : //Si es un identificador solo se llama a la función mandándole un puntero
			Identificador(presul);
			break;
		
		case CONSTANTE : //Si es constante se corrobora que se pasó correctamente y modifica el puntero 
			Match(CONSTANTE);
			*presul = ProcesarCte();
			break;
	
		case PARENIZQ : //Se verifica que dentro de los paréntesis haya una expresión y si ambos paréntesis están ubicados
			Match(PARENIZQ);
			Expresion(presul);
			Match(PARENDER);
			break;
	
		default : 
			return;
	}
}

//OPERADOR ADITIVO_______________________________________________________
void OperadorAditivo(char * presul){
	TOKEN t = ProximoToken();
	if ( t == SUMA || t == RESTA ){
		 Match(t);
		 strcpy(presul, ProcesarOp());
	}else{ 
		ErrorSintactico(t);
	}	
} //Se verifica que el token que figura sea suma o resta, caso contrario es un error sintáctico 

//RUTINAS SEMÁNTICAS ___________________________________________________________________
//Aquellas funciones utilizadas para poder comprender los elementos utilizados y acciones necesarias para poder lograr cumplir el análisis 

//PROCESAR CONSTANTE_____________________________________________________
// Convierte cadena que representa numero a numero entero y construye un registro semantico 
REG_EXPRESION ProcesarCte(void){
	REG_EXPRESION reg;
	reg.clase = CONSTANTE;
	strcpy(reg.nombre, buffer);
	sscanf(buffer, "%d", &reg.valor);
	return reg;
}

//PROCESAR IDENTIFICADOR_________________________________________________
//Declara ID y construye el correspondiente registro semántico
REG_EXPRESION ProcesarId(void){
	REG_EXPRESION reg;
	Chequear(buffer);
	reg.clase = ID;
	strcpy(reg.nombre, buffer);
	return reg;
}

//PROCESAR OPERADOR______________________________________________________
//Declara OP y construye el correspondiente registro semántico
char * ProcesarOp(void){
	return buffer;
}

//LEER___________________________________________________________________
//Genera la instruccion para leer
void Leer(REG_EXPRESION in){
	Generar("Read", in.nombre, "Entera", "");
}

//ESCRIBIR_______________________________________________________________
//Genera la instruccion para escribir 
void Escribir(REG_EXPRESION out){
	Generar("Write", Extraer(&out), "Entera", "");
}

//GENERAR INFIJO_________________________________________________________
//Genera la instruccion para una operacion infija y construye un registro semantico con el resultado
REG_EXPRESION GenInfijo(REG_EXPRESION e1, char * op, REG_EXPRESION e2){
	REG_EXPRESION reg;
	static unsigned int numTemp = 1;
	char cadTemp[TAMLEX] ="Temp&";
	char cadNum[TAMLEX];
	char cadOp[TAMLEX];
	if ( op[0] == '-' ){
	 strcpy(cadOp, "Restar");
	}
	if ( op[0] == '+' ){
		strcpy(cadOp, "Sumar");
	} 
	sprintf(cadNum, "%d", numTemp);
	numTemp++;
	strcat(cadTemp, cadNum);
	if ( e1.clase == ID) {
		Chequear(Extraer(&e1));
	}
	if ( e2.clase == ID) {
		Chequear(Extraer(&e2));
	}
	Chequear(cadTemp);
	Generar(cadOp, Extraer(&e1), Extraer(&e2), cadTemp);
	strcpy(reg.nombre, cadTemp);
	return reg;
}


//FUNCIONES AUXILIARES _________________________________________________________________
//Son funciones que son creadas con el objetivo de cumplir acciones que estan fuera del analisis

//MATCH__________________________________________________________________
//Se encarga de comparar el elemento siguiente con el pasado por parametro
void Match(TOKEN t){
if ( !(t == ProximoToken()) )
	{ErrorSintactico();
} 
flagToken = 0;
}

//PROXIMO TOKEN__________________________________________________________
//Tal como dice su nombre, retorna el proximo token a leer
TOKEN ProximoToken(){
	if ( !flagToken ){
		tokenActual = scanner();
		if ( tokenActual == ERRORLEXICO ){
			ErrorLexico();
		} 
		flagToken = 1;
		if ( tokenActual == ID ) {
			Buscar(buffer, TS, &tokenActual);
		}
	}
	return tokenActual;
}

//ERROR LEXICO Y SINTACTICO______________________________________________
//Te retorna un texto que te notifica que hay un error
void ErrorLexico(){
	printf("Error Lexico\n");
}
void ErrorSintactico(){
	printf("Error Sintactico\n");
}

//GENERAR________________________________________________________________
//Produce la salida de la instruccion para la MV por stdout
void Generar(char * co, char * a, char * b, char * c){
	printf("%s %s%c%s%c%s\n", co, a, ',', b, ',', c);
}

//EXTRAER________________________________________________________________
//Retorna la cadena del registro semantico 
char * Extraer(REG_EXPRESION * preg){
	return preg->nombre;
}

//BUSCAR_________________________________________________________________
//Determina si un identificador esta en la TS 
int Buscar(char * id, RegTS * TS, TOKEN * t){
	int i = 0;
	while ( strcmp("$", TS[i].identifi) ){
		 if ( !strcmp(id, TS[i].identifi) ) {
			 *t = TS[i].t;
			 return 1;
		 }
		 i++;
	}
	return 0;
}

//COLOCAR________________________________________________________________
//Agrega un identificador a la TS
void Colocar(char * id, RegTS * TS){
	int i = 4;
	while ( strcmp("$", TS[i].identifi) ){
		i++;
	} 
	if ( i < 999 ){
	 strcpy(TS[i].identifi, id );
	 TS[i].t = ID;
	 strcpy(TS[++i].identifi, "$" );
	}
}

//CHEQUEAR_______________________________________________________________
//Si la cadena No esta en la Tabla de Simbolos la agrega, y si es el nombre de una variable genera la instruccion
void Chequear(char * s){
	TOKEN t;
	if ( !Buscar(s, TS, &t) ){
		 Colocar(s, TS);
		 Generar("Declara", s, "Entera", "");
	}
}

//COMENZAR_______________________________________________________________
//Inicializaciones Semanticas
void Comenzar(void){}

//TERMINAR_______________________________________________________________
//Genera la instruccion para terminar la ejecucion del programa
void Terminar(void){
	Generar("Detiene", "", "", "");
}

//ASIGNAR________________________________________________________________
//Genera la instruccion para la asignacion
void Asignar(REG_EXPRESION izq, REG_EXPRESION der){
	Generar("Almacena", Extraer(&der), izq.nombre, "");
}

//SCANNER___________________________________________________________________________
TOKEN scanner(){
int tabla[NUMESTADOS][NUMCOLS] = {
 { 1, 3, 5, 6, 7, 8, 9, 10, 11, 14, 13, 0, 14 },
 { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
 { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 }
};
int car;
int col;
int estado = 0;
int i = 0;
do
{
 car = fgetc(in);
 col = columna(car);
 estado = tabla[estado][col];
 if ( col != 11 )
 {
 buffer[i] = car;
 i++;
 }
}
while ( !estadoFinal(estado) && !(estado == 14) );
buffer[i] = '\0';
switch ( estado )
{
 case 2 : if ( col != 11 )
 {
 ungetc(car, in);
 buffer[i-1] = '\0';
 }
 return ID;
 case 4 : if ( col != 11 )
 {
 ungetc(car, in);
 buffer[i-1] = '\0';
 }
 return CONSTANTE;
 case 5 : return SUMA;
 case 6 : return RESTA;
 case 7 : return PARENIZQ;
 case 8 : return PARENDER;
 case 9 : return COMA;
 case 10 : return PUNTOYCOMA;
 case 12 : return ASIGNACION;
 case 13 : return FDT;
 case 14 : return ERRORLEXICO;
}
return 0;
}
int estadoFinal(int e)
{
if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 ) return 0;
return 1;
}
int columna(int c)
{
if ( isalpha(c) ) return 0;
if ( isdigit(c) ) return 1;
if ( c == '+' ) return 2;
if ( c == '-' ) return 3;
if ( c == '(' ) return 4;
if ( c == ')' ) return 5;
if ( c == ',' ) return 6;
if ( c == ';' ) return 7;
if ( c == ':' ) return 8;
if ( c == '=' ) return 9;
if ( c == EOF ) return 10;
if ( isspace(c) ) return 11;
return 12;
}
/********Fin Scanner**********************************************/

