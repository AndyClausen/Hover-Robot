/*
  Melody

 Plays a melody

 circuit:
 * 8-ohm speaker on digital pin 8

 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Tone

 */
#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_F5, NOTE_D5, NOTE_D5, NOTE_F6, NOTE_G6, NOTE_C7, NOTE_A6
};

int melodyKey1[] = {
  NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_E5, NOTE_CS5, NOTE_CS5, NOTE_E5, NOTE_FS5, NOTE_B5, NOTE_GS5
};

int melodyKey2[] = {
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_AS4, NOTE_G4, NOTE_G4, NOTE_AS4, NOTE_C5, NOTE_F5, NOTE_D5
};

int melodyBass[] = {
  NOTE_D4, NOTE_C4, NOTE_F4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 8, 8, 4, 4, 8, 8, 2
};

int noteDurationsBass = 16;

int soundPin = 8;


bool timer(int t, bool check) {
  if (check == 1)
    return (t <= millis());
  else {
    t += millis();
    return false;
  }
}


void play() {

  //--BASS MELODY--//

  if (!playing) {

    for (int i = 0; i < 2 ; i++) {

      for (int thisNote = 0; thisNote < 16; thisNote++) {
        int noteDuration = 1600 / noteDurationsBass;

        int pauseBetweenNotes = noteDuration * 1.30;

        if (thisNote == 6) {
          for (int i = 0; i < 2 ; i++) {
            tone(soundPin, melodyBass[2], noteDuration);

            delay(pauseBetweenNotes);
            
            //timer(pauseBetweenNotes, 0);
            //if (!timer(pauseBetweenNotes, 1))
              noTone(soundPin);
            //else
              //break;


          }

          delay(pauseBetweenNotes * 2);
          thisNote++;
        }
        else if (thisNote == 14) {
          for (int i = 0; i < 2 ; i++) {
            tone(soundPin, melodyBass[0], noteDuration);

            delay(pauseBetweenNotes);

            noTone(soundPin);
          }

          delay(pauseBetweenNotes * 2);
          thisNote++;
        }
        else {
          for (int i = 0; i < 2 ; i++) {
            tone(soundPin, melodyBass[thisNote % 2], noteDuration);

            delay(pauseBetweenNotes);

            noTone(soundPin);
          }
        }

      }
    }


    //--LEAD MELODY--//

    for (int i = 0; i < 4; i++) {

      // iterate over the notes of the melody:
      for (int thisNote = 0; thisNote < 10; thisNote++) {

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1600 / noteDurations[thisNote];
        tone(soundPin, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(soundPin);
      }
    }

    //--BASS MELODY--//

    for (int i = 0; i < 2 ; i++) {

      for (int thisNote = 0; thisNote < 16; thisNote++) {
        int noteDuration = 1600 / noteDurationsBass;

        int pauseBetweenNotes = noteDuration * 1.30;

        if (thisNote == 6) {
          for (int i = 0; i < 2 ; i++) {
            tone(soundPin, melodyBass[2], noteDuration);

            delay(pauseBetweenNotes);

            noTone(soundPin);
          }

          delay(pauseBetweenNotes * 2);
          thisNote++;
        }
        else if (thisNote == 14) {
          for (int i = 0; i < 2 ; i++) {
            tone(soundPin, melodyBass[0], noteDuration);

            delay(pauseBetweenNotes);

            noTone(soundPin);
          }

          delay(pauseBetweenNotes * 2);
          thisNote++;
        }
        else {
          for (int i = 0; i < 2 ; i++) {
            tone(soundPin, melodyBass[thisNote % 2], noteDuration);

            delay(pauseBetweenNotes);

            noTone(soundPin);
          }
        }

      }
    }


    //--LEAD MELODY KEY DOWN FIRST--//

    for (int i = 0; i < 2; i++) {

      // iterate over the notes of the melody:
      for (int thisNote = 0; thisNote < 10; thisNote++) {

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1600 / noteDurations[thisNote];
        tone(soundPin, melodyKey1[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(soundPin);
      }
    }

    //--LEAD MELODY KEY DOWN SECOND--//

    for (int i = 0; i < 2; i++) {

      // iterate over the notes of the melody:
      for (int thisNote = 0; thisNote < 10; thisNote++) {

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1600 / noteDurations[thisNote];
        tone(soundPin, melodyKey2[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(soundPin);
      }
    }
  }
}



