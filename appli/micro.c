/**
  ******************************************************************************
  * @file    traces.c
  * @author  Romain
  * @date    05-May-2023
  * @brief   All the functions related to the microphone
  ******************************************************************************
*/

#include "stdlib.h"
#include "config.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"
#include "systick.h"
#include "micro.h"
#include "stm32f1_adc.h" // Module pour ADC0 lecture du micro

#define MODE_PASSWORD 0 // 0 pour password déjà enregistré, 1 pour pouvoir l'enregistrer

#define BLIND_DURATION 100
#define VALEUR_SEUIL_DETECTION 2300
#define COEF_ERREUR 5
#define TAILLE_TABLEAUX 10

static bool_e initialized_micro = FALSE;
static volatile uint32_t t_blind = 0;
static volatile uint16_t t = 0;
static volatile int absolute_time = 0;
static volatile uint16_t t_enregistrement = 0;
uint32_t compteur = 0;
uint32_t index = 0;
static uint32_t nbr_toc = 7;
static int player_try[10];
static int password[TAILLE_TABLEAUX] = {3720, 4180, 4940, 5730, 6180, 6930, 8620, 0, 0, 0}; // Mario
//static int password[TAILLE_TABLEAUX] = {0, 1000, 2000, 3000, 4000, 5000} // 1 coup / s


/**
 * \fn void process_ms_micro (void)
 * \brief Fonction gérant l'incrément ou décrément temporel
 */
static void process_ms_micro(void)
{
	if(t)
		t--;
	if (t_enregistrement)
		t_enregistrement--;
	absolute_time++;

}


void MICRO_init(void)
{
	Systick_add_callback_function(&process_ms_micro);
	initialized_micro = TRUE;
}


micro_event_joueur_e MICRO_joueur_state_machine(void){

	typedef enum{
		INIT = 0,
		RESET_INDEX,
		WAIT_PULSE,
		PULSE_HIGH,
		BLIND,
		COMPUTE,
		CODE_CHECK,
		CODE_OK,
		WAIT_PERIOD,
	}state_micro_e;

	static state_micro_e state = INIT;
	int16_t mesure_son;
	ret = VEROUILLE;

	if (initialized_micro){

		switch(state)
		{
		case INIT:
			MICRO_init();

			state = RESET_INDEX;
			break;
		case RESET_INDEX:
			index = 0;
			for(int k=0; k < nbr_toc; k++){
				player_try[k] = 0;
			}
			absolute_time = 0;
			state = WAIT_PULSE;
			break;
		case WAIT_PULSE:
			if(!t){
				t = 10;
				mesure_son = ADC_getValue(0);
				if(mesure_son > VALEUR_SEUIL_DETECTION)
				{
					t = BLIND_DURATION;
					state = PULSE_HIGH;
				}
			}
			break;
		case PULSE_HIGH:
			player_try[index] = absolute_time;
			index++;
			printf("[MICRO] Detection son n° %d		Temps_abs = %d ms.\n", index - 1, player_try[index-1]);
			state = COMPUTE;
			break;
		case COMPUTE:
			if (index == nbr_toc){
				state = CODE_CHECK;
			}
			else {
				state = WAIT_PULSE;
			}

			break;
		case CODE_CHECK:
			printf("[MICRO] Verification du code (Coef erreur : %d)\n", COEF_ERREUR);


			for (uint32_t k = 1; k < nbr_toc; k++){
				int player_try_k = 100 * (player_try[k] - player_try[0]) / (player_try[nbr_toc - 1] - player_try[0]);
				int password_k = 100 * (password[k] - password[0]) / (password[nbr_toc - 1] - password[0]);
				int erreur_k = abs(player_try_k - password_k);
				printf("[MICRO] Evaluation mesure n° %d :    Joueur : %d     Password : %d      Erreur : %d\n", k, player_try_k, password_k, erreur_k);

				if (erreur_k > COEF_ERREUR){
					printf("[MICRO] Evaluation mesure n° %d NOK\n", k);
					t = 3000;
					state = INIT;
					break;
				}
			}
			absolute_time = 0;
			state = CODE_OK;

			break;
		case CODE_OK:
			printf("C'EST GAGNE !!!\n ");
			ret = DEVEROUILLE;
			break;
		}
		return ret;
	}
}

void MICRO_enregistrement_sequence(void){
#if MODE_PASSWORD
	for(int k=0; k < TAILLE_TABLEAUX; k++){
		password[k] = 0;
	}
	int index = 0;
	absolute_time = 0;
	t_enregistrement = 5000;
	while (index < TAILLE_TABLEAUX || t_enregistrement){
		if (!t){
			t= 20;
			int16_t mesure_son = ADC_getValue(0);
			if(mesure_son > VALEUR_SEUIL_DETECTION)
			{
				t = BLIND_DURATION;
				password[index] = absolute_time;
			}
		}

	printf("[MICRO] Enregistrement séquence : ");
	for(int k=0; k < TAILLE_TABLEAUX; k++){
			printf("%d", password[k]);
		}

	printf("\n");
	}
#endif
}


void Micro_detection_test(void)
	{
		typedef enum
		{
			INIT = 0,
			WAIT_SOUND,
			BUTTON_SOUND,
			SOUND_DETECTED
		}state_micro;

		static state_micro state = INIT;
		int16_t mesure_son;

		if(initialized_micro)
		{
			mesure_son = ADC_getValue(0);
			switch(state)
			{
				case INIT:
					state = WAIT_SOUND;
					break;
				case WAIT_SOUND:
					if(!t){
						t = 10;
//						printf("[MICRO] Mesure %d\n", mesure_son);
						if(mesure_son > VALEUR_SEUIL_DETECTION)
						{
							state = SOUND_DETECTED;
							t = BLIND_DURATION;
						}
					}

					break;
				case SOUND_DETECTED:
					compteur++;
					printf("[MICRO] Detection son n° %d		Mesure : %d			%d\n", compteur, mesure_son, t);
					state = WAIT_SOUND;
					break;

				default:
					state = INIT;
					break;
			}
		}
	}





