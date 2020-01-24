#ifndef __iNUT__
#define __iNUT__
#ifdef CUSTOM_INUT

#include "Wire.h"
#define INUTCOMMAND_BUFFER 64

#else
#include <Wire.h>
#define INUTCOMMAND_BUFFER 32

#endif

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif
#include <string.h>
#include<stdarg.h>

#ifndef INUT_COMMAND_MAX_LENGTH
#define INUT_COMMAND_MAX_LENGTH 20
#endif
void i2cRequestEvent();
void receiveEvent(int howMany);

#define INUTCOMMAND_DEBUG
#undef INUTCOMMAND_DEBUG //uncomment dòng này dể debug chương trình inutcommand
#define BIT true
#define FLOAT false

class iNut {
public:
	void setup(int sensor_count = 8, int i2c_port = 10);
	void loop();
	void setValue(int index, float value);
	float getValue(int index);
	char *next();
	void addCommand(const char *command, void(*function)());  // Add a command to the processing dictionary.
    void setDefaultHandler(void (*function)(const char *));   // A handler to call when no valid command received.
	void clearBuffer();   // Clears the input buffer.
	iNut();

	void turnOn(int index, int idx);
	void turnOff(int index, int idx);
	void alarm(int code, int n_args =0 ,...);

	~iNut();
	
private:
	bool *_types;
	float *_sensors;
	float *_alarmSensors;
	int _sensor_count;
	struct iNutCommandCallback {
      char command[INUT_COMMAND_MAX_LENGTH + 1];
      void (*function)();
    }; 
	iNutCommandCallback *commandList;   // Actual definition for command/handler array
    byte commandCount;
	
	// Pointer to the default handler function
    void (*defaultHandler)(const char *);
	
	char delim[2]; // null-terminated list of character to be used as delimeters for tokenizing (default " ")
    char term;     // Character that signals end of command (default '\n')
	
	char buffer[INUTCOMMAND_BUFFER + 1]; // Buffer of stored characters while waiting for terminator character
    byte bufPos;                        // Current position in the buffer
    char *last;                         // State variable used by strtok_r during processing
};

#endif 
