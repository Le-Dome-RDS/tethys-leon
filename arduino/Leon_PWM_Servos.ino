/*
  Projet TETHYS / Léon
  Changement de plage du récepteur Radio

  Le récepteur radio (FrSKY X8R) envoie un signal PWM entre 1000us et 2000us
  Le servo peut accepter un signal entre 500us et 2500us

  Cette version prend en charge deux servos (direction gauche, direction droite)

  Gauche : 6 --------> 10
  Droite : 9 --------> 11
   
*/

const int inputPinServoGauche   = 6;  // Signal en provenance du récepteur pour [servomoteur direction gauche]
const int inputPinServoDroit   = 9;  // Signal en provenance du récepteur pour [servomoteur direction droit]
const int outputPinServoGauche  = 10; // Signal modifié vers [servomoteur direction gauche]
const int outputPinServoDroit  = 11; // Signal modifié vers [servomoteur direction droit]

int rcSignalServoGauche = 0, rcSignalServoDroit = 0;
int smoothOutputValueServoGauche = 0, smoothOutputValueServoDroit = 0;        // value output to the PWM
int smoothInputSignalServoGauche = 0, smoothInputSignalServoDroit = 0;        // Valeur du signal d'entrée lissé
int previousRcSignalServoGauche = 0, previousRcSignalServoDroit = 0;
int threshold        = 20;        // Seuil en dessous duquel le nouveau signal n'est pas envoyé au servo, car la variation est considérée comme trop faible. 
int smoothFactor     = 10;

void setup() {
  Serial.begin(9600);
  Serial.print("Prise en charge des signaux PWM : servo droit, servo gauche");
}

void loop() {
  // PWM Signal : Servo gauche
  rcSignalServoGauche = pulseIn(inputPinServoGauche, HIGH);
  if (abs(rcSignalServoGauche-previousRcSignalServoGauche) >= threshold){ // si la variation est suffisante pour être prise en compte    
    // Lissage du signal d'entrée
    smoothInputSignalServoGauche = smoothSignal(rcSignalServoGauche);
    // map it to the range of the analog out:
    smoothOutputValueServoGauche = map(smoothInputSignalServoGauche, 975, 2000, 500, 2500);
    // Generate Output Signal
    pulseOut(outputPinServoGauche, smoothOutputValueServoGauche);
    // update previous signal
    previousRcSignalServoGauche = rcSignalServoGauche;
  }
  // PWM Signal : Servo droit
  rcSignalServoDroit = pulseIn(inputPinServoDroit, HIGH);
  if (abs(rcSignalServoDroit-previousRcSignalServoDroit) >= threshold){ // si la variation est suffisante pour être prise en compte    
    // Lissage du signal d'entrée
    smoothInputSignalServoDroit = smoothSignal(rcSignalServoDroit);
    // map it to the range of the analog out:
    smoothOutputValueServoDroit = map(smoothInputSignalServoDroit, 975, 2000, 500, 2500);
    // Generate Output Signal
    pulseOut(outputPinServoDroit, smoothOutputValueServoDroit);
    // update previous signal
    previousRcSignalServoDroit = rcSignalServoDroit;
  }
}

// Lisser un signal
int smoothSignal(int signal) {
  int smoothInputSignal = rcSignalServoDroit / smoothFactor;
  smoothInputSignal = smoothInputSignal * smoothFactor;
  return smoothInputSignal;
}

// Envoyer un signal à l'un des servos
void pulseOut(int pin, int us) {
   digitalWrite(pin, HIGH);
   delayMicroseconds(us);
   digitalWrite(pin, LOW);
}
