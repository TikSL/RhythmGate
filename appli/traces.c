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


void print_traces(char * texte){

#if TRACES == 1
	printf(texte);
#endif
	return 0;
}
