/*
Speech.h - A library for speech synthesis with a XFS5152CE
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

#ifndef STM32_SPEECH_H
#define STM32_SPEECH_H





#define MAX_SPEECH_CHAR 256

enum SPEECH_STATE
{
	SPEECH_STATE_STOPPED,
	SPEECH_STATE_RUNNING,
	SPEECH_STATE_FINISHED
};

class CSpeech
{
  public:
    CSpeech(int baud);
    void speak(String message);
    void waitForSpeech(unsigned long timeout = 60000);
    SPEECH_STATE get_state() {return state;}
    void set_state(SPEECH_STATE new_state){state = new_state;}

    void refresh_state();

  private:
    void sendMessagetoSpeechModule();
    SPEECH_STATE state;

    char messageBuffer[MAX_SPEECH_CHAR];
    String incoming;
    String options;
    String messageString;
    int stringLength;
    
};

#endif
