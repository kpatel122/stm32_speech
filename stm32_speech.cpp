/*
Speech.cpp - A library for speech synthesis with a XFS5152CE
Copyright (c) 2019 Kunal Patel

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

This permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Arduino.h>
#include "stm32_speech.h"

 
void CSpeech::refresh_state()
{
	if(state == SPEECH_STATE_RUNNING)
	{
		//speech must be running check for end byte
		if( Serial2.available() )
		{
			if ( Serial2.read() == 0x4F )
			{
				state = SPEECH_STATE_FINISHED;
				return;
			}
		}
	}
}

CSpeech::CSpeech(int baud) 
{
  options = "[x0][t6][v5][s6][m51][g2][h2][n1]";
  state = SPEECH_STATE_STOPPED;
}
 
void CSpeech::speak(String message)
{
  
  messageString =options + message;

  // Length (with one extra character for the null terminator)
  stringLength = messageString.length() + 1; 

  // Copy it over 
  messageString.toCharArray(messageBuffer, stringLength);
  sendMessagetoSpeechModule();
  state = SPEECH_STATE_RUNNING;
}

void CSpeech::sendMessagetoSpeechModule()
{
  Serial2.write(0xFD);
  Serial2.write((byte)0x0);
  Serial2.write(2 + strlen(messageBuffer));
  Serial2.write(0x01);
  Serial2.write((byte)0x0);
  Serial2.write(messageBuffer);
}

void CSpeech::waitForSpeech(unsigned long timeout) {
  unsigned long start = millis();
  bool done = false;
  while ( ! done && (millis() - start) < timeout ) {
    while ( Serial2.available() ) {
      if ( Serial2.read() == 0x4F ) {
        Serial1.println("0x4F"); //tmp
        done = true;
        break;
      }
    }
  }
}



