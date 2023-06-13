/**
  ******************************************************************************
  * @file    traces.c
  * @author  Romain
  * @date    13-April-2023
  * @brief   Log function.
  ******************************************************************************
*/


#include "traces.h"
#include <stdio.h>

#define TRACES 1 // Mettre à 1 pour activer les traces

void print_traces(char * texte){

#if TRACES
	printf(texte);
#endif
	return 0;
}
