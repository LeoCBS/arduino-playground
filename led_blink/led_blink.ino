int pin = 8; // represent positve output led

void setup() { 
  pinMode(pin, OUTPUT);// set pin 8 as led output

}

void loop() {
  digitalWrite(pin, HIGH); // write 5v in pin 8
  
  delay(5000); 
   
  digitalWrite(pin, LOW);// write 0v in pin 8
  delay(1000); 

}
