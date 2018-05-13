#include "iNut.h"

volatile char iNut_buffer[INUTCOMMAND_BUFFER + 1];
volatile boolean iNut_receiveFlag = false;
float* iNut_sensors;
int size_of_iNut;

//send data to inut cam bien when it requests
void i2cRequestEvent()
{
    char* data = (byte*)iNut_sensors;
    Wire.write(data, size_of_iNut);
#ifdef INUTCOMMAND_DEBUG
    Serial.println(F("Send data to 'inut - cam bien'"));
#endif
}

//When revice command and save to buffer
void receiveEvent(int howMany)
{
    Wire.readBytes((byte*)iNut_buffer, howMany);
    iNut_buffer[howMany] = 0;
    iNut_receiveFlag = true;
#ifdef INUTCOMMAND_DEBUG
    Serial.print(F("Nhan lenh tu Internet: "));
	String str = iNut_buffer;
	Serial.println(str);
#endif
}

iNut::iNut()
    : commandList(NULL)
    , commandCount(0)
    , defaultHandler(NULL)
    , term('\n')
    , // default terminator for commands, newline character
    last(NULL)
{
    strcpy(delim, " "); // strtok_r needs a null-terminated string
    clearBuffer();
}

void iNut::setup(int sensor_count, int i2c_port)
{
    Wire.begin(i2c_port);

    Wire.onReceive(receiveEvent);
    Wire.onRequest(i2cRequestEvent);

    _sensors = new float[sensor_count];
	size_of_iNut = sensor_count * sizeof(float);
    iNut_sensors = _sensors;
}

/**
 * Retrieve the next token ("word" or "argument") from the command buffer.
 * Returns NULL if no more tokens exist.
 */
char* iNut::next()
{
    return strtok_r(NULL, delim, &last);
}

//set sensor value
void iNut::setValue(int index, float value)
{
    _sensors[index] = value;
}

//add iNut.loop() is ok
void iNut::loop()
{
    if (iNut_receiveFlag) {
		
		String command = iNut_buffer;
		command += term;
		byte idx = 0;
        while (idx < command.length()) {
            char inChar = command[idx++]; // Read single available character, there may be more waiting
#ifdef INUTCOMMAND_DEBUG
            Serial.print(inChar); // Echo back to serial stream
#endif

            if (inChar == term) { // Check for the terminator (default '\r') meaning end of command
#ifdef INUTCOMMAND_DEBUG
                Serial.print("Received: ");
                Serial.println(buffer);
#endif

                char* command = strtok_r(buffer, delim, &last); // Search for command at start of buffer
                if (command != NULL) {
                    boolean matched = false;
                    for (int i = 0; i < commandCount; i++) {
#ifdef INUTCOMMAND_DEBUG
                        Serial.print("Comparing [");
                        Serial.print(command);
                        Serial.print("] to [");
                        Serial.print(commandList[i].command);
                        Serial.println("]");
#endif

                        // Compare the found command against the list of known commands for a match
                        if (strncmp(command, commandList[i].command, INUT_COMMAND_MAX_LENGTH) == 0) {
#ifdef INUTCOMMAND_DEBUG
                            Serial.print("Matched Command: ");
                            Serial.println(command);
#endif

                            // Execute the stored handler function for the command
                            (*commandList[i].function)();
                            matched = true;
                            break;
                        }
                    }
                    if (!matched && (defaultHandler != NULL)) {
                        (*defaultHandler)(command);
                    }
                }
                clearBuffer();
            }
            else if (isprint(inChar)) { // Only printable characters into the buffer
                if (bufPos < INUTCOMMAND_BUFFER) {
                    buffer[bufPos++] = inChar; // Put character into buffer
                    buffer[bufPos] = '\0'; // Null terminate
                }
                else {
#ifdef INUTCOMMAND_DEBUG
                    Serial.println("Line buffer is full - increase INUTCOMMAND_BUFFER");
#endif
                }
            }
        }
		iNut_receiveFlag = false; //mark read
    }
}

/*
 * Clear the input buffer.
 */
void iNut::clearBuffer()
{
    buffer[0] = '\0';
    bufPos = 0;
}

void iNut::setDefaultHandler(void (*function)(const char*))
{
    defaultHandler = function;
}

//add command to i2c
void iNut::addCommand(const char* command, void (*function)())
{
#ifdef INUTCOMMAND_DEBUG
    Serial.print("Adding command (");
    Serial.print(commandCount);
    Serial.print("): ");
    Serial.println(command);
#endif

    commandList = (iNutCommandCallback*)realloc(commandList, (commandCount + 1) * sizeof(iNutCommandCallback));
    strncpy(commandList[commandCount].command, command, INUT_COMMAND_MAX_LENGTH);
    commandList[commandCount].function = function;
    commandCount++;
}