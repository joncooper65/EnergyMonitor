const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;

void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT); //Light resistor TSL257 pin 3 (out) connected to arduino A0 pin
  Serial.begin(9600);
}

void loop() {
  digitalWrite(ledPin, state);
  
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(200);
  int val = analogRead(A0)/10;
  for(int x=0; x<val; x++){
    Serial.print(" ");
  }
  Serial.println("*");
}

void blink() {
  state = !state;
}

