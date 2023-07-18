#include <LiquidCrystal.h>
#include <DHT11.h>

const int pinoPir1 = 7; // Sensor de presença 1
const int pinoPir2 = 6; // Sensor de presença 2

const int pinoLed1 = 5; // Led Sensor de presença 1
const int pinoLed2 = 4; // Led Sensor de presença 2

int pirState = LOW;
bool burnOn = false;
long timerBurn = 0;

// Sensores de temperatura
const int humidade = 70;

// Display
const int rs = 2, en = 8, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
int contrast = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte customChar[] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000
};

// Relé
int relePin = 10;

// DHT11 Interno
int dht11IntPin = 12;
DHT11 dht11Int(dht11IntPin);

// DHT11 Externo
int dht11ExtPin = 13;
DHT11 dht11Ext(dht11ExtPin);

/*
  Display:

  P 1 _ I N T : t t _ h h _ _ _ _
  P 2 _ E X T : t t _ h h _ _ _ _

*/

void setup()
{
  // Comunicação serial
  Serial.begin(9600);
  Serial.println("initializing setup...");
  
  // Setup sensores de presença e leds
  pinMode(pinoLed1, OUTPUT);
  pinMode(pinoPir1, INPUT);
  
  pinMode(pinoLed2, OUTPUT);
  pinMode(pinoPir2, INPUT);
  
  
  // ## Display LCD
  analogWrite(3, contrast);
  
  // Define o tamanho da tela LCD 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando");
  delay (1000);
  lcd.setCursor(8, 1);
  lcd.print("Setup...");
  delay (2000);
  lcd.createChar(0, customChar);
  lcd.clear();
  
  
  // Relé
  pinMode(relePin, OUTPUT);
}

void loop()
{
  sensorPresenca();
  temperatura();
  rele();
}

void temperatura(){
  
  lcd.setCursor(3, 0);
  lcd.print("INT:" + String((int)dht11Int.readTemperature()));
  
  lcd.setCursor(9,0);
  lcd.write(byte(0));

  lcd.setCursor(10,0);
  lcd.print(String((int)dht11Int.readHumidity()) + "%");

  lcd.setCursor(3, 1);
  lcd.print("EXT:" + String((int)dht11Ext.readTemperature()));
  
  lcd.setCursor(9,1);
  lcd.write(byte(0)); 
  
  lcd.setCursor(10,1);
  lcd.print(String((int)dht11Ext.readHumidity()) + "%");
}

void sensorPresenca(){

  // Se o sensor estiver em HIGH liga o led e o lcd, senão desliga os dois
  if(digitalRead(pinoPir1) == HIGH){
    // Serial.println("Presenca no sensor 1"); 
    digitalWrite(pinoLed1, HIGH);
    lcd.setCursor(0,0);
    lcd.print("M");
    pirState = HIGH;

  }else{
    digitalWrite(pinoLed1, LOW);
    lcd.setCursor(0,0);
    lcd.print("  ");
    pirState = LOW;
  }
    
  // Se o sensor estiver em HIGH liga o led e o lcd, senão desliga os dois
  if(digitalRead(pinoPir2) == HIGH){
    // Serial.println("Presenca no sensor 2"); 
    digitalWrite(pinoLed2, HIGH);
    lcd.setCursor(0,1);
    lcd.print("P");
    pirState = HIGH;
  }else{
    digitalWrite(pinoLed2, LOW);
    lcd.setCursor(0,1);
    lcd.print("  ");
    pirState = LOW;
  }
 }

void rele(){
  Serial.println("State: " + String(pirState) + " burn: " + String(burnOn) + " timerBurn: " + String(timerBurn) + " millis: " + millis());
  if (pirState == HIGH && !burnOn)
  {
    digitalWrite(relePin, LOW);
    Serial.println("Rele On"); 
    burnOn = true;
    timerBurn = millis();
  }
  
  if (millis() > timerBurn + 5000){
    digitalWrite(relePin, HIGH);
    Serial.println("Rele Off");
    burnOn = false;
    timerBurn = 0;
  }
}