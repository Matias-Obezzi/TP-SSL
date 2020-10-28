%{
    #include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
    #include "trabajoBison.tab.h"
%}

DIGITO [0-9]

LETRA [a-zA-Z]

IDENTIFICADOR {LETRA}({LETRA}|{DIGITO})*

constEntera {DIGITO}({DIGITO})*

%%

"fdt" return FDT;
"inicio"		return INICIO;
"fin"			return FIN;

"leer" return LEER;

"escribir" return ESCRIBIR;

":=" {return ASIGNACION;}

{constEntera} {return CONSTANTE;}

{IDENTIFICADOR} {return (ID);}

";" {return PYCOMA;}

"(" {return PARENIZQUIERDO;}

")" {return PARENDERECHO;}

"+" {return SUMA;}

"-" {return RESTA;}

"," {return COMA;}

%%