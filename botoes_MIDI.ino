#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

// Definicao dos pinos para controle do multiplex
#define pin_S1 2  //Definicao dos pinos
#define pin_S2 3
#define pin_S3 4
#define pin_S4 5
#define pin_in A0

#define LIMITE 150 // Valor arbitrario que define quando o ADC diz que um "botao" foi aplicado
#define NUMERO_TECLAS 2

// Notas na 4a oitava
#define nota_do 48
#define nota_do_sharp 49
#define nota_re 50
#define nota_re_sharp 51
#define nota_mi 52
#define nota_fa 53
#define nota_fa_sharp 54
#define nota_sol 55
#define nota_sol_sharp 56
#define nota_la 57
#define nota_la_sharp 58
#define nota_si 59

void select_input ( int selec) {
  int bit_selecao[4] = {0, 0, 0, 0};
  for (int i = 0; i < 4; i++) {
    bit_selecao[i] = bitRead(selec, i);
  }
  digitalWrite(pin_S1, bit_selecao[0]);
  digitalWrite(pin_S2, bit_selecao[1]);
  digitalWrite(pin_S3, bit_selecao[2]);
  digitalWrite(pin_S4, bit_selecao[3]);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  pinMode(pin_S1, OUTPUT);  // Configura os pinos como entrada ou saida
  pinMode(pin_S2, OUTPUT);
  pinMode(pin_S3, OUTPUT);
  pinMode(pin_S4, OUTPUT);
  pinMode(pin_in, INPUT);
  Serial.begin(31250);
}

void loop() {
  int leitura;
  bool botao0 = 0;
  bool botao1 = 0;
  for (int selecionar = 0; selecionar <= NUMERO_TECLAS; selecionar++) {
    select_input(selecionar);
    leitura = digitalRead(pin_in);
    if (leitura == LOW) {
      switch (selecionar) {
        case 0:
          botao0 = 1;
          noteOn(1, nota_do, 64);
          MidiUSB.flush();
          break;
        case 1:
          botao1 = 1;
          noteOn(1, nota_re, 64);
          MidiUSB.flush();
          break;
      }
    }
    else {
      switch (selecionar) {
        case 0:
          if (botao0 == 1) {
            botao0 = 0;
            noteOff(1, nota_do, 64);
            MidiUSB.flush();
          }
          break;
        case 1:
          if (botao1 == 1) {
            botao1 = 0;
            noteOff(1, nota_re, 64);
            MidiUSB.flush();
          }
          break;
      }
    }
  }
  delay(100);
}
