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


void CSpeech::update_voice_settings()
{
	options = "[x0][g2][h2][n1]"; //base values
	for(int i=0;i<MAX_SPEECH_PARAMS;i++)
	{
		options +=voice_params[i];
	}

	//options = "[x0][t1][v5][s5]" + *current_voice + "[g2][h2][n1]";}//options = "[x0][t6][v5][s6]" + *current_voice + "[g2][h2][n1]";
}

void CSpeech::set_voice_type (VOICE_TYPE new_voice)
{
	voice_params[VOICE_TYPE_PARAM] = "[m"+ String(new_voice) +"]";
}

void CSpeech::set_voice_speed (byte new_speed)
{
	voice_params[VOICE_SPEED_PARAM] = "[s"+ String(new_speed) +"]";
}
void CSpeech::set_voice_tone (byte new_tone)
{
	voice_params[VOICE_TONE_PARAM] = "[t"+ String(new_tone) +"]";
}
void CSpeech::set_voice_volume (byte new_volume)
{
	voice_params[VOICE_VOLUME_PARAM] = "[v"+ String(new_volume) +"]";
}

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

CSpeech::CSpeech(int baud, VOICE_TYPE voice_type)
{
	set_voice_type (voice_type);
	set_voice_speed (6);//default value
	set_voice_tone(6);
	set_voice_volume(5);
	update_voice_settings();

	//options = "[x0][t6][v5][s6]" + *current_voice + "[g2][h2][n1]";
	state = SPEECH_STATE_STOPPED;
}
 
void CSpeech::speak(String message)
{
  
  messageString =options + message;



  // Length (with one extra character for the null terminator)
  stringLength = messageString.length() + 1; 

  // Copy it over 
  messageString.toCharArray(messageBuffer, stringLength);
  send_message_to_speech_module();
  state = SPEECH_STATE_RUNNING;
}

void CSpeech::send_message_to_speech_module()
{

  Serial2.write(0xFD); //default
  Serial2.write((byte)0x0); //size high byte
  Serial2.write(2 + strlen(messageBuffer)); //size low byte
  Serial2.write(0x01); //command -speech
  Serial2.write((byte)0x0);//text encoding 0x00=GB2312 0x01=GBK 0x02=BIG5  0x03=UNICODE
  Serial2.write(messageBuffer);//message data

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



