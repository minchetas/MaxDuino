#include "Encoder.h"
#include "userconfig.h"
#include "pinSetup.h"

#ifdef ENCODER

#include "Rotary.h"
#include "ButtonEvents.h"
#include "Bounce2.h"

// Because the button polling functions are called from multiple places 
// (button_play(), button_stop(), etc) we need to keep the encoder's 
// detected value a minimum of N times, until it is considered idle.
#define MULTIPLE_CHECK_COUNTER  4;

int8_t buttonUp = 0;
int8_t buttonDown = 0;
int8_t buttonPlay = 0;
int8_t buttonStop = 0;

Rotary g_Encoder(PIN_ENCODER_DT, PIN_ENCODER_CLK);
int8_t g_PreviousSteps;
volatile int8_t g_EncoderSteps;

// Encoder Button
ButtonEvents g_EncoderButton;
ButtonEvent g_ButtonEvent;


void timerIsr() {
  uint8_t encoderDir = g_Encoder.process();
  if (encoderDir == DIR_CW)
    g_EncoderSteps++;
  else if (encoderDir == DIR_CCW)
    g_EncoderSteps--;
}

bool ProcessEncoderRotation() {
  int8_t encoderSteps = 0;
  cli();
  encoderSteps = g_EncoderSteps;
  g_EncoderSteps = 0;
  sei();

  if (encoderSteps == 0) {        
    return false;
  } else {        
    buttonUp = buttonDown = false;
    #ifdef _ENCODER_DEBUG_
    #endif
    if (encoderSteps > 0)   {
      buttonDown += MULTIPLE_CHECK_COUNTER;
    } else {
      buttonUp += MULTIPLE_CHECK_COUNTER;      
    }
  }    
}

void ProcessEncoderButton() {                    
  if (g_EncoderButton.update()) {                        
    buttonPlay = buttonStop = false;
    switch(g_EncoderButton.event()) {
      case (ButtonEvent::tap) : { 
        buttonPlay += MULTIPLE_CHECK_COUNTER;
        break;                 
      }
      case (ButtonEvent::doubleTap) : {        
        break;         
      }
      case (ButtonEvent::hold) : { 
        buttonStop += MULTIPLE_CHECK_COUNTER;                
        break; 
      }
    }        
  }      
}

bool button_any() {        
  return ( 
    buttonPlay  || 
    buttonStop  ||
    buttonUp    ||
    buttonDown  
  );    
};

bool button_play() {
  if (buttonPlay > 0) {
    buttonPlay--;
    if(buttonPlay < 0) buttonPlay = 0; 
  }    
  return buttonPlay;
};

bool button_stop() {    
  if (buttonStop > 0) {
    buttonStop--;
    if(buttonStop < 0) buttonStop = 0; 
  }    
  return buttonStop;
};

bool button_up() {     
  if (buttonUp > 0) {
    buttonUp--;
    if(buttonUp < 0) buttonUp = 0;    
  }        
  return buttonUp;        
};

bool button_down() {
  if (buttonDown > 0) {
    buttonDown--;
    if(buttonDown < 0) buttonDown = 0;    
  }        
  return buttonDown;       
};

bool button_menu() {
  return false;
};

bool button_root() {
  return false;
};


//********************************************************
// *** SETUP
//********************************************************

void encoderSetup() {  
  pinMode(PIN_ENCODER_SWITCH, INPUT_PULLUP);
  g_EncoderButton.attach(PIN_ENCODER_SWITCH);
  g_EncoderButton.debounceTime(15);    
  attachInterrupt(0, timerIsr, CHANGE);
  attachInterrupt(1, timerIsr, CHANGE);    
}

//********************************************************
// *** LOOP
//********************************************************

void encoderLoop() {
  ProcessEncoderRotation();    
  ProcessEncoderButton();
}

#endif // -- ENCODER