void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT); //Light resistor TSL257 pin 3 (out) connected to arduino A0 pin
  Serial.begin(9600);
}

void loop() {
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(200);
  int val = analogRead(A0)/10;
  for(int x=0; x<val; x++){
    Serial.print(" ");
  }
  Serial.println("*");
}
