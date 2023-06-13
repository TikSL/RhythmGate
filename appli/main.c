/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "WS2812S.h"
#include "stm32f1_adc.h"

#include "button.h"
#include "traces.h"
#include "led.h"
#include "servo.h"


static void state_machine(void);

static volatile uint32_t t = 0;
static volatile uint32_t t_ecoute = 0;

/**
 * \fn void process_ms(void)
 * \brief Fonction gérant l'incrément ou décrément temporel
 */
void process_ms(void)
{
	if(t)
		t--;
	if(t_ecoute)
		t_ecoute--;
}


/**
 * \fn int main (void)
 * \brief Entrée du programme.
 *
 * \return 0 - Arrêt normal du programme.
 */
int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la première étape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 à la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
	//Attention, les pins PA2 et PA3 ne sont pas reliées jusqu'au connecteur de la Nucleo.
	//Ces broches sont redirigées vers la sonde de débogage, la liaison UART étant ensuite encapsulée sur l'USB vers le PC de développement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le périphérique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//On ajoute la fonction process_ms à la liste des fonctions appelées automatiquement chaque ms par la routine d'interruption du périphérique SYSTICK
	//	Systick_add_callback_function(&process_ms);

	//Initialisation des composants
	LED_init();
	BUTTON_init();
	SERVO_init();
	ADC_init();
	MICRO_init();

	printf("\n");
	printf("#--------------------------#\n");
	printf("|      INITIALISATION      |\n");
	printf("|        COMPLETEE         |\n");
	printf("#--------------------------#\n");

//	Systick_add_callback_function(process_1ms);



	while(1)
	{
//		Micro_detection_test();
		state_machine();
//		DEMO_adc_statemachine();
//		MICRO_joueur_state_machine();
	}
	return 0;
}



/**
 * \fn static state_machine (void)
 * \brief Machine d'état principale du projet
 */
static void state_machine(void){
	typedef enum
		{
			INIT,
			MODE_VEROUILLE,
			MODE_DEVEROUILLE,
			MODE_ENREGISTREMENT,
			MODE_JOUE_SEQUENCE,
			MODE_ECOUTE
		}state_e;

		static state_e mode_state = INIT;
//		static state_e previous_state = INIT;
		bool_e entrance = (mode_state!=previous_state)?TRUE:FALSE;	//ce booléen sera vrai seulement 1 fois après chaque changement d'état.
		previous_state = mode_state;									//previous_state mémorise l'état actuel (qui est le futur état précédent)

		// Detection bouton organisateur
		button_event_organi_e button_organi_event = BUTTON_organi_state_machine();

		// Detection bouton joueur
		button_event_joueur_e button_joueur_event = BUTTON_joueur_state_machine();



		switch(mode_state)
		{
			case INIT:
				print_traces("[MODE	] MODE INIT -> MODE DEVEROUILLE\n");
				mode_state = MODE_DEVEROUILLE;
				SERVO_ouvrir();
				break;

			case MODE_DEVEROUILLE:


				LED_set(LED_ON, Color[2]); //LED blanche full
				if (button_organi_event == BUTTON_EVENT_LONG_PRESS){
					print_traces("[MODE	] MODE DEVEROUILLE -> MODE ENREGISTREMENT\n");
					LED_set(LED_BLINK, Color[3]); //LED verte clignote 3s
					HAL_Delay(3000);
					mode_state = MODE_ENREGISTREMENT;
				}
				if (button_organi_event == BUTTON_EVENT_SHORT_PRESS){
					print_traces("[MODE	] MODE DEVEROUILLE -> MODE JOUE_SEQUENCE\n");
					mode_state = MODE_JOUE_SEQUENCE;
				}
				if (button_organi_event == BUTTON_EVENT_DOUBLE_PRESS){
					print_traces("[MODE	] MODE DEVEROUILLE -> MODE VEROUILLE\n");
					LED_set(LED_BLINK, Color[1]); //LED rouge clignote 5s
					HAL_Delay(5000);
					SERVO_fermer();
					mode_state = MODE_VEROUILLE;
				}
				break;

			case MODE_ENREGISTREMENT:

				LED_set(LED_ON, Color[3]); //LED verte full 5s
				MICRO_enregistrement_sequence();
				print_traces("[MODE	] MODE ENREGISTREMENT -> MODE JOUE_SEQUENCE\n");
				mode_state = MODE_JOUE_SEQUENCE;
				break;

			case MODE_JOUE_SEQUENCE:
				LED_set(LED_ON, Color[4]); //LED bleue full
				HAL_Delay(5000);
				print_traces("[MODE	] MODE JOUE_SEQUENCE -> MODE DEVEROUILLE \n");
				mode_state = MODE_DEVEROUILLE;
				break;

			case MODE_VEROUILLE:
				LED_set(LED_ON, Color[1]); //LED rouge full
				if (button_joueur_event == BUTTON_JOUEUR_EVENT_PRESS){
					print_traces("[MODE	] MODE VEROUILLE -> MODE ECOUTE\n");
					mode_state = MODE_ECOUTE;
				}
				if (button_organi_event == BUTTON_EVENT_LONG_PRESS){
					print_traces("[MODE	] MODE VEROUILLE -> MODE DEVEROUILLE\n");
					mode_state = MODE_DEVEROUILLE;
				}
				break;
			case MODE_ECOUTE:
				if (entrance){
					LED_set(LED_BLINK, Color[5]); //LED rose full 5s
					t_ecoute = 5000;
				}
				micro_event_joueur_e resultat_ecoute = MICRO_joueur_state_machine();
				if (!t_ecoute){
					if (resultat_ecoute == DEVEROUILLE){
					print_traces("[MODE	] MODE ECOUTE -> MODE DEVEROUILLE\n");
					SERVO_ouvrir();
					mode_state = MODE_DEVEROUILLE;
					}
					else{
						mode_state = MODE_ECOUTE;
					}
				}
				else{
					mode_state = MODE_VEROUILLE;
				}
			default:
				break;
		}
}





