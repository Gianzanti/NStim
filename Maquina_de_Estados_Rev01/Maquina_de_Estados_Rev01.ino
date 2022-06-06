
// ---- Mapeamento de Hardware ----
#define SN A1 // Define o pino A1 como pino SN ("Stimulus Negative")
#define SP A2 // Define o pino A2 como pino SP ("Stimulus Positive")

// ----- Constantes  --------
const uint16_t T1_init = 0;
uint16_t T1_comp = 25;
uint16_t T1_cont = 0;

// ----- Variaveis --------
uint16_t T1_limit = 1250;
uint16_t T1_PW = 625;
unsigned int State = 0;

unsigned int nTrainPulses = 1;
unsigned int nPulses = 1;
unsigned int TtrainMin;
unsigned int TtrainMax;
unsigned int Ttrain; //Tempo de duração entre os trens de pulsos
unsigned int TNtrain; //Tempo de duração dos trens de pulsos
unsigned int Frequency = 10;


unsigned int Ttune;
unsigned int TCOMPTP;
unsigned int TCOMPTPW;

bool STIMON = false;        //flag que monitora/controla a estimulação ON
bool STIMFSTPULSE = false; //flag que monitora/controla se ocorreu o primeiro pulso
bool STIMSTATE = false;   //flag que monitora/controla o estado de saida
bool STIMSTATEPW = false; //flag que monitora/controla o estado de mudança de fase
bool STIMINPHASE = true;  //flag que monitora/controla a fase inicial do primeiro pulso
bool STIMMONO = true; // flag que monitora/controla se o sinal é monopolar ou bipolar
bool STIMTRAIN = false; // flag que monitora/controla se ocorrerá trens de pulsos
bool STIMTUNE = false; //flag que monitora/controla se ocorrerá compensação na fase catodica
bool STIMSTATETUNE = false; //flag que monitora/controla o estado de mudança de fase

bool ATTPW = false; //Atingiu o tempo TPW(Tempo de Pulse Width)
bool ATTP = false; //Atingiu o tempo TP(Tempo de pulso)
bool ATTPWC = false; //Atingiu o tempo TPWC(Tempo de Pulse Width Compensado)
bool ATTPC = false; //Atingiu o tempo TPC(Tempo de pulso Compensado)
bool ATTRAIN = false; //Atingiu o tempo TP(Tempo de pulso)
bool STIMCOMP = false; //flag que monitora/controla se ocorrerá compensação na fase catodica
bool STIMPHASE = false;  //flag que monitora/controla a fase inicial do primeiro pulso





// ----- Interrupção ------
ISR(TIMER1_COMPA_vect) {

  TCNT1 = T1_init; // reinicializa TIMER1
  //STIM();
  UpdateTP();
  UpdateState();

}


void setup() {
  Serial.begin(9600);
  pinMode(SN, OUTPUT); //define SN como saída
  pinMode(SP, OUTPUT); //define SP como saída
  //Modo de Comparação
  TCCR1A = 0;

  //Prescaler 1:256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);


  //Inicializa Registradores
  TCNT1 = T1_init;
  OCR1A = T1_comp;


  //Habilita Interrupção do Timer1
  TIMSK1 = ( 1 << OCIE1A);

  SFREQ(Frequency);
  SPW(500);
  Serial.println(T1_PW);
  Serial.println(T1_limit);
  STIMPHASE = true;
  STIMMONO = true;
  STIMTRAIN = true; //Trens de pulsos EN=1
  nTrainPulses = 4;
  STTrainMin();
  STtrain(500);
  STIMTUNE = false;
  STIMCOMP = false;
  SCOMPTPC(1);
  SCOMPTPWC(1);
  delay(1000);
  STIMON = true;
}

void loop() {
  Serial.println(State);
  delay(500);

}

void UpdateTP() {
  if (STIMON) {
    if (STIMCOMP) {
      //Compensado

      if (!ATTPW && !ATTP) {
        if (T1_cont >= T1_PW ) {
          ATTPW = true;
        }
        T1_cont++;
      }

      if (!ATTP && ATTPW) {
        if (T1_cont >= T1_limit ) {
          ATTP = true;
        }
        T1_cont++;
      }

      if (ATTPW && !ATTPWC && !STIMPHASE) {
        if (T1_cont >= TCOMPTPW ) {
          ATTPWC = true;
        }
        T1_cont++;
      }

      if (ATTPW && ATTP && !ATTPC && STIMPHASE) {
        if (T1_cont >= TCOMPTP ) {
          ATTPC = true;
        }
        T1_cont++;
      }


      if (ATTPC && ATTPW && !ATTRAIN && STIMPHASE) {
        if (T1_cont >= Ttrain ) {
          ATTRAIN = true;
        }
        T1_cont++;
      }

      if (ATTP && ATTPW && !ATTRAIN && !STIMPHASE) {
        if (T1_cont >= Ttrain ) {
          ATTRAIN = true;
        }
        T1_cont++;
      }

    }
    else {
      if (!ATTPW && !ATTP) {
        if (T1_cont >= T1_PW ) {
          ATTPW = true;
        }
        T1_cont++;
      }
      if (!ATTP && ATTPW) {
        if (T1_cont >= T1_limit ) {
          ATTP = true;
        }
        T1_cont++;
      }
      if (ATTP && ATTPW && !ATTRAIN) {
        if (T1_cont >= Ttrain ) {
          ATTRAIN = true;
        }
        T1_cont++;
      }
    }
  }
}

void UpdateState() {
  switch (State) {

    case 0: //S0
      if (STIMON) {
        State = 1;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 1: //S1
      if (STIMMONO) { //Monofásico
        State = 2;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Bifásico
        State = 9;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 2: //S2
      if (STIMTRAIN) { //Trem de pulso
        State = 5;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
        nPulses = 1;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Sem trem de pulso
        State = 3;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 3: //S3
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 4;
      }
      else {    //Não atingiu o Tempo TPW
        State = 3;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 4: //S4
      if (ATTP) { //Atingiu o Tempo TP
        State = 3;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
      }
      else {    //Não atingiu o Tempo TP
        State = 4;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 5: //S5
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 6;
      }
      else {    //Não atingiu o Tempo TPW
        State = 5;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 6: //S6
      if (ATTP) { //Atingiu o Tempo TP
        State = 7;
      }
      else {    //Não atingiu o Tempo TP
        State = 6;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 7: //S7
      if (nPulses == nTrainPulses) {
        State = 8;
        T1_cont = 0;
        ATTPW = true;
        ATTP = true;
      }
      else {
        nPulses++;
        State = 5;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
      }
      break;

    case 8: //S8
      if (ATTRAIN) { //Atingiu o Tempo TTRAIN
        State = 5;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
        ATTRAIN = false;
        digitalWrite(SP, LOW);
      }
      else {    //Não atingiu o Tempo TTRAIN
        State = 8;
        nPulses = 1;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;

    case 9: //S9
      if (STIMCOMP) { //Compensado
        State = 25;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Não compensado
        State = 10;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 10: //S10
      if (STIMTRAIN) { //Trem de pulso
        State = 16;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
        nPulses = 1;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Sem trem de pulso
        State = 11;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 11: //S11
      if (STIMPHASE) { //Bifásico Positivo
        State = 12;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Bifásico Negativo
        State = 14;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 12: //S12
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 13;
      }
      else {    //Não atingiu o Tempo TPW
        State = 12;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 13: //S13
      if (ATTP) { //Atingiu o Tempo TP
        State = 12;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
      }
      else {    //Não atingiu o Tempo TP
        State = 13;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;


    case 14: //S14
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 15;
      }
      else {    //Não atingiu o Tempo TPW
        State = 14;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;

    case 15: //S15
      if (ATTP) { //Atingiu o Tempo TP
        State = 14;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
      }
      else {    //Não atingiu o Tempo TP
        State = 15;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 16: //S16
      if (STIMPHASE) { //Bifásico Positivo
        State = 21;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Bifásico Negativo
        State = 17;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 17: //S17
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 18;
      }
      else {    //Não atingiu o Tempo TPW
        State = 17;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;

    case 18: //S18
      if (ATTP) { //Atingiu o Tempo TP
        State = 19;
      }
      else {    //Não atingiu o Tempo TP
        State = 18;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 19: //S19
      if (nPulses == nTrainPulses) { //Atingiu o número de pulsos
        State = 20;
        T1_cont = 0;
        ATTPW = true;
        ATTP = true;
      }
      else { //Não atingiu o número de pulsos
        nPulses++;
        State = 17;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
      }
      break;

    case 20: //S20
      if (ATTRAIN) { //Atingiu o Tempo TTRAIN
        State = 17;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
        ATTRAIN = false;
      }
      else {    //Não atingiu o Tempo TTRAIN
        State = 20;
        nPulses = 1;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;


    case 21: //S21
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 22;
      }
      else {    //Não atingiu o Tempo TPW
        State = 21;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 22: //S22
      if (ATTP) { //Atingiu o Tempo TP
        State = 23;
      }
      else {    //Não atingiu o Tempo TP
        State = 22;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;

    case 23: //S23
      if (nPulses == nTrainPulses) { //Atingiu o número de pulsos
        State = 24;
        T1_cont = 0;
        ATTPW = true;
        ATTP = true;
      }
      else { //Não atingiu o número de pulsos
        nPulses++;
        State = 21;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
      }
      break;

    case 24: //S24
      if (ATTRAIN) { //Atingiu o Tempo TTRAIN
        State = 21;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
        ATTRAIN = false;
      }
      else {    //Não atingiu o Tempo TTRAIN
        State = 24;
        nPulses = 1;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;


    case 25: //S25
      if (STIMTRAIN) { //Trem de pulso
        State = 33;
        T1_cont = 0;
        ATTPWC = false;
        ATTPC = false;
        ATTPW = false;
        ATTP = false;
        nPulses = 1;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Sem trem de pulso
        State = 26;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 26: //S26
      if (STIMPHASE) { //Bifásico Positivo
        State = 27;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Bifásico Negativo
        State = 30;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 27: //S27
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 28;
      }
      else {    //Não atingiu o Tempo TPW
        State = 27;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 28: //S28
      if (ATTP) { //Atingiu o Tempo TP
        State = 29;
      }
      else {    //Não atingiu o Tempo TP
        State = 28;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;

    case 29: //S29
      if (ATTPC) { //Atingiu o Tempo TP+C
        State = 27;
        T1_cont = 0;
        ATTPW = false;
        ATTP = false;
        ATTPC = false;
      }
      else {    //Não atingiu o Tempo TP+C
        State = 29;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;


    case 30: //S30
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 31;
      }
      else {    //Não atingiu o Tempo TPW
        State = 30;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;


    case 31: //S31
      if (ATTPWC) { //Atingiu o Tempo TPW+C
        State = 32;
      }
      else {    //Não atingiu o Tempo TPW+C
        State = 31;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 32: //S32
      if (ATTP) { //Atingiu o Tempo TP
        State = 30;
        T1_cont = 0;
        ATTPWC = false;
        ATTPW = false;
        ATTP = false;
      }
      else {    //Não atingiu o Tempo TP
        State = 32;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 33: //S33
      if (STIMPHASE) { //Bifásico Positivo
        State = 39;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      else {    //Bifásico Negativo
        State = 34;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 34: //S34
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 35;
      }
      else {    //Não atingiu o Tempo TPW
        State = 34;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;

    case 35: //S35
      if (ATTPWC) { //Atingiu o Tempo TPW+C
        State = 36;
      }
      else {    //Não atingiu o Tempo TPW+C
        State = 35;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 36: //S36
      if (ATTP) { //Atingiu o Tempo TP
        State = 37;
      }
      else {    //Não atingiu o Tempo TP
        State = 36;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 37: //S37
      if (nPulses == nTrainPulses) { //Atingiu o número de pulsos
        State = 38;
        T1_cont = 0;
        ATTPWC = true;
        ATTPW = true;
        ATTP = true;
      }
      else { //Não atingiu o número de pulsos
        nPulses++;
        State = 34;
        T1_cont = 0;
        ATTPWC = false;
        ATTPW = false;
        ATTP = false;
      }
      break;

    case 38: //S38
      if (ATTRAIN) { //Atingiu o Tempo TTRAIN
        State = 34;
        T1_cont = 0;
        ATTPWC = false;
        ATTPW = false;
        ATTP = false;
        ATTRAIN = false;
      }
      else {    //Não atingiu o Tempo TTRAIN
        State = 38;
        nPulses = 1;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;


    case 39: //S39
      if (ATTPW) { //Atingiu o Tempo TPW
        State = 40;
      }
      else {    //Não atingiu o Tempo TPW
        State = 39;
        digitalWrite(SP, LOW);
        digitalWrite(SN, HIGH);
      }
      break;

    case 40: //S40
      if (ATTP) { //Atingiu o Tempo TP
        State = 41;
      }
      else {    //Não atingiu o Tempo TP
        State = 40;
        digitalWrite(SP, HIGH);
        digitalWrite(SN, LOW);
      }
      break;

    case 41: //S41
      if (ATTPC) { //Atingiu o Tempo TP+C
        State = 42;
      }
      else {    //Não atingiu o Tempo TP+C
        State = 41;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;

    case 42: //S42
      if (nPulses == nTrainPulses) { //Atingiu o número de pulsos
        State = 43;
        T1_cont = 0;
        ATTPC = true;
        ATTPW = true;
        ATTP = true;
      }
      else { //Não atingiu o número de pulsos
        nPulses++;
        State = 39;
        T1_cont = 0;
        ATTPC = false;
        ATTPW = false;
        ATTP = false;
      }
      break;

    case 43: //S43
      if (ATTRAIN) { //Atingiu o Tempo TTRAIN
        State = 39;
        T1_cont = 0;
        ATTPC = false;
        ATTPW = false;
        ATTP = false;
        ATTRAIN = false;
      }
      else {    //Não atingiu o Tempo TTRAIN
        State = 43;
        nPulses = 1;
        digitalWrite(SP, LOW);
        digitalWrite(SN, LOW);
      }
      break;


    default:
      break;
  }
}

void STIM() {
  if (STIMON) {
    if (STIMFSTPULSE) {
      if ((T1_cont >= T1_PW && T1_cont <= T1_limit && STIMSTATEPW == false && STIMTRAIN == false) || (T1_cont >= T1_PW && T1_cont <= T1_limit && STIMSTATEPW == false &&  STIMTRAIN == true && nPulses <= nTrainPulses) ) {
        if (STIMMONO) {
          digitalWrite(SN, !STIMSTATE);
          STIMSTATE = !STIMSTATE;
          STIMSTATEPW = true;
        }
        else {
          if (STIMTUNE) {
            if (!STIMSTATE) {


              if ((T1_cont >= Ttune && T1_cont <= T1_limit && STIMSTATETUNE == false && STIMTRAIN == false) || (T1_cont >= Ttune && T1_cont <= T1_limit && STIMSTATETUNE == false &&  STIMTRAIN == true && nPulses <= nTrainPulses) ) {
                digitalWrite(SN, LOW);
                digitalWrite(SP, LOW);
                STIMSTATETUNE = true;
              }
              else if ((T1_cont >= Ttune && T1_cont <= T1_limit && STIMSTATETUNE == true && STIMTRAIN == false) || (T1_cont >= Ttune && T1_cont <= T1_limit && STIMSTATETUNE == true &&  STIMTRAIN == true && nPulses <= nTrainPulses) ) {
                digitalWrite(SN, !STIMSTATE);
                digitalWrite(SP, STIMSTATE);
                STIMSTATE = !STIMSTATE;
                STIMSTATEPW = true;
                STIMSTATETUNE = false;
              }
            }
          }
          else {
            digitalWrite(SN, !STIMSTATE);
            digitalWrite(SP, STIMSTATE);
            STIMSTATE = !STIMSTATE;
            STIMSTATEPW = true;
          }
        }
        ;
      }
      if ((T1_cont >= T1_PW && T1_cont >= T1_limit && STIMTRAIN == false) || (T1_cont >= T1_PW && T1_cont >= T1_limit && nPulses <= nTrainPulses && STIMTRAIN == true)) {
        if (STIMMONO) {
          digitalWrite(SN, !STIMSTATE);
        }
        else {
          digitalWrite(SN, !STIMSTATE);
          digitalWrite(SP, STIMSTATE);
        }
        STIMSTATE = !STIMSTATE;
        STIMSTATEPW = false;
        T1_cont = 0;
        nPulses++;
        if (nPulses >= nTrainPulses && STIMTRAIN == true) {
          digitalWrite(SN, LOW);
          digitalWrite(SP, HIGH);
          STIMSTATEPW = false;
        }
      }
      else if (T1_cont >= T1_PW && T1_cont >= T1_limit && nPulses >= nTrainPulses && STIMTRAIN == true && T1_cont >= Ttrain) {

        digitalWrite(SN, LOW);
        digitalWrite(SP, HIGH);
        STIMSTATE = false;
        STIMSTATEPW = false;
        T1_cont = 0;
        nPulses = 1;;
      }
      T1_cont++;
    }
    else {
      if (STIMINPHASE) { //A fase inicial do primeiro pulso é positivo?
        STIMSTATE = true; //Estado inicial da saída é acionada
        digitalWrite(SN, HIGH);
        STIMFSTPULSE = true;
      }
      else { //Se a fase inicial do primeiro pulso não for positivo
        STIMSTATE = false; //Estado inicial da saída é desacionada
        digitalWrite(SN, LOW);
        STIMFSTPULSE = true;
      }
    }
  }
}



void SFREQ(unsigned int freq) { //Seleciona a frequência
  T1_limit = 2 * 1250 / freq;
}


void SPW(unsigned int PW) { //Seleciona a largura de pulso (Pulse Width)
  T1_PW = 2 * 1250 * 0.001 * 0.1 * PW;
}



void SCOMPTPC(unsigned int Tt) { //Calcula o pulsos necessários para compensar em TP
  TCOMPTP  = T1_limit + (2 * 1250 * 0.001 * 20);
}

void SCOMPTPWC(unsigned int Tt) { //Calcula o pulsos necessários para compensar em TPW
  TCOMPTPW  = T1_PW + (2 * 1250 * 0.001 * 20);
}


void STTrainMin() { //Seleciona a frequência
  TtrainMin = nTrainPulses * T1_limit;
}

void STtrain(unsigned int TT) {
  Ttrain = 2 * 1250 * 0.001 * TT;
  if (Ttrain <= TtrainMin) {
    if (Frequency <= 10) {
      Ttrain = 250; //equivale a 0,1s
    }
    else {
      Ttrain = TtrainMin;
    }
  }
}
