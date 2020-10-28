%{
    #include <stdio.h>
	
    extern int yylex(void);
    extern void yyerror(char*);
    void errorMaxCaracteres()
    %}
    %union {
        char* cadena;
    }

    %token ASIGNACION PYCOMA CONSTANTE SUMA RESTA PARENIZQUIERDO PARENDERECHO COMA INICIO FIN LEER ESCRIBIR FDT
    %token <cadena> ID

    %%

objetivo: programa FDT
;
programa: INICIO listaSentencias FIN
;
listaSentencias: listaSentencias sentencia
|sentencia
;
sentencia: ID ASIGNACION expresion PYCOMA
|LEER PARENIZQUIERDO listaIdentificadores PARENDERECHO PYCOMA
|ESCRIBIR PARENIZQUIERDO listaExpresiones PARENDERECHO PYCOMA
;
listaIdentificadores: ID
|listaIdentificadores COMA ID
;
listaExpresiones: expresion
|listaExpresiones COMA expresion
;
expresion: primaria
|expresion operadorAditivo primaria
;
primaria: ID|CONSTANTE|PARENIZQUIERDO expresion PARENDERECHO
;
operadorAditivo: SUMA 
|RESTA
;

%%
int main(){
    yyparse();
}
void yyerror (char *s){
    printf("Falla en el codigo: %s\n",s);
}
int yywrap() {
    return 1;
}
void errorMaxCaracteres(){
	cout << endl  << "Error sintactico: La longitud maxima de los identificadores es de 32 caracteres" << ", " << "en linea " << yylineno<< endl;
	exit(0);
}
%
