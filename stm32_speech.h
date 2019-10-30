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


#define MAX_VOICES 3

enum VOICE_TYPE
{
	VOICE_FEMALE = 3, //numbers from the datasheet
	VOICE_MALE = 51,
	VOICE_DAFFY_DUCK =54
};



#define MAX_SPEECH_PARAMS 4 //as per datasheet
#define VOICE_TYPE_PARAM 0 //gender of speech
#define VOICE_SPEED_PARAM 1
#define VOICE_TONE_PARAM 2
#define VOICE_VOLUME_PARAM 3


class CSpeech
{
  public:
    CSpeech(int baud,VOICE_TYPE voice_type = VOICE_MALE);
    void speak(String message);
    void waitForSpeech(unsigned long timeout = 60000);
    SPEECH_STATE get_state() {return state;}
    void set_state(SPEECH_STATE new_state){state = new_state;}
    //void set_voice (SPEECH_VOICE_TYPE new_voice) {current_voice = &voice[new_voice];update_voice_settings();}


    void set_voice_type (VOICE_TYPE new_voice);
    void set_voice_speed (byte new_speed);
    void set_voice_tone (byte new_tone);
    void set_voice_volume (byte new_volume);
    void update_voice_settings();//must be called after any of the above set


    void refresh_state();

  private:
    void send_message_to_speech_module();
    //void update_voice_settings() {options = "[x0][t1][v5][s5]" + *current_voice + "[g2][h2][n1]";}//options = "[x0][t6][v5][s6]" + *current_voice + "[g2][h2][n1]";}
    SPEECH_STATE state;

    char messageBuffer[MAX_SPEECH_CHAR];
    String incoming;
    String options;
    String messageString;
    int stringLength;
    

    String voice_params[MAX_SPEECH_PARAMS];

    String voice[MAX_VOICES];
    String *current_voice;

};

#endif
