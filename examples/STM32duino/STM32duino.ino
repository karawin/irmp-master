/*
 * irpm test for stm32f103cb generic blue board under stm32duino
 * Ir pin entry is PC15
 * jp Cocatrix 2017
 */
//#include <Arduino.h>
//#  include <stdint.h>

#include <irmp.h>
#define SERIALX Serial1
#define PIN_LED LED_BUILTIN
IRMP_DATA irmp_data;
HardwareTimer timer(2);

unsigned loopcount = 0; // loop counter

void timer2_init ()
{
/*
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
// init timer2
    TIM_TimeBaseStructure.TIM_ClockDivision                 = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode                   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period                        = 7;//7
    TIM_TimeBaseStructure.TIM_Prescaler                     = ((F_CPU / F_INTERRUPTS)/8) - 1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

// init interrupt
    NVIC_InitStructure.NVIC_IRQChannel                      = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0x0f;//0f
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0x0f;//0f
    NVIC_Init(&NVIC_InitStructure);

    timer_attach_interrupt(TIMER2, TIMER_CC1_INTERRUPT, TIM2_IRQHandler);

    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
*/
/*    
    timer_init(TIMER2);
    timer_pause(TIMER2);
    timer_set_mode(TIMER2, 1, TIMER_OUTPUT_COMPARE);
    timer_set_prescaler(TIMER2, ((F_CPU / F_INTERRUPTS)/8) - 1);
    timer_set_reload(TIMER2, 7);
    timer_attach_interrupt(TIMER2, TIMER_CC1_INTERRUPT, TIM2_IRQHandler);
    timer_resume(TIMER2);
 */
    timer.pause();
    timer.setPrescaleFactor( ((F_CPU / F_INTERRUPTS)/8) - 1);
    timer.setOverflow(7);
    timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
    timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
    timer.attachCompare1Interrupt(TIM2_IRQHandler);
        // Refresh the timer's count, prescale, and overflow
    timer.refresh();

    // Start the timer counting
    timer.resume();
}

void TIM2_IRQHandler()                                                       // Timer2 Interrupt Handler
{
  /*
 //trying to clear SR by reading the register
 if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //clearing UIF
    loopcount++;
    (void) irmp_ISR(); // call irmp ISR
 }
  // call other timer interrupt routines...   
*/

    (void) irmp_ISR(); // call irmp ISR
     loopcount++;
   
}


void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    delay(2000);   
    Serial.println("Init");
    pinMode(PIN_LED, OUTPUT);
    irmp_init();   // initialize irmp
    timer2_init(); // initialize timer2
    Serial.println(((F_CPU / F_INTERRUPTS)/8) - 1);
    Serial.println(F_CPU);
    Serial.println(F_INTERRUPTS);
}

// blink led
void led()
{
    if (loopcount == 0x1875)
           digitalWrite(PIN_LED, HIGH);
    if (loopcount == 0x3750)
    {
            loopcount = 0;// 
            digitalWrite(PIN_LED, LOW);  
    }  
}

void loop() {
  // put your main code here, to run repeatedly:
    led();
    
    if (irmp_get_data (&irmp_data))
    {
            // ir signal decoded, do something here...
            // irmp_data.protocol is the protocol, see irmp.h
            Serial.print("Protocol: ");Serial.println(irmp_data.protocol);
            // irmp_data.address is the address/manufacturer code of ir sender
            // irmp_data.command is the command code
            Serial.print("Data: ");Serial.println(irmp_data.command);
            // irmp_protocol_names[irmp_data.protocol] is the protocol name (if enabled, see irmpconfig.h)
            Serial.print("Name: ");Serial.println(irmp_protocol_names[irmp_data.protocol]);
            Serial.println();
     }
}
