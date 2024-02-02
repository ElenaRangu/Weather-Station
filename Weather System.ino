#include <LiquidCrystal.h>

// Definirea pinilor pentru LED-uri, buton și senzor de temperatură
#define LED_RED 5
#define LED_YELLOW 6
#define BUTTON_PIN 2
#define SENSOR_PIN A0

// Definirea pinilor pentru LCD
#define RS 3
#define EN 4
#define DB4 10
#define DB5 11
#define DB6 12
#define DB7 13

// Inițializarea LCD-ului
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

// Variabila pentru starea sistemului (pornit/oprit)
bool systemOn = false;

// Variabile pentru temperaturile minime și maxime
float minTemp = 100;
float maxTemp = -100;

// Variabilă pentru înregistrarea timpului ultimei actualizări a temperaturii
unsigned long lastTempUpdate = 0;

// Intervalul de actualizare a temperaturii (în milisecunde)
const unsigned long tempUpdateInterval = 1500;

// Funcția de inițializare a componentelor și a LCD-ului
void setup() {
  // Setarea modului pinilor pentru LED-uri și buton
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Inițializarea LCD-ului cu 16 coloane și 2 rânduri
  lcd.begin(16, 2);

  // Afisarea mesajului "System OFF" pe LCD
  lcd.setCursor(0, 0);
  lcd.print("System OFF");
}

// Funcția principală care rulează în mod continuu
void loop() {
  // Verificarea stării butonului (pornit/oprit)
  if (digitalRead(BUTTON_PIN) == HIGH) {
    // Schimbarea stării sistemului și anti-rebound delay
    systemOn = !systemOn;
    delay(250);

    // Verificarea dacă sistemul este pornit
    if (systemOn) {
      // Afisarea mesajului "System ON" pe LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("System ON ");
      lcd.setCursor(0, 1);
      lcd.print("Reading temp...");
      delay(1500); // Așteptare pentru stabilizarea senzorului de temperatură
      lcd.clear();
    } else {
      // Afisarea mesajului "System OFF" pe LCD și stinge LED-urile
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("System OFF");
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
    }
  }

  // Verificarea intervalului de actualizare a temperaturii
  if (systemOn && (millis() - lastTempUpdate >= tempUpdateInterval)) {
    // Actualizarea timpului ultimei actualizări
    lastTempUpdate = millis();

    // Citirea temperaturii de la senzor
    float currentTemp = readTemperature();

    // Afisarea temperaturii curente și a minimului și maximului pe LCD
    lcd.setCursor(0, 0);
    lcd.print("Current: ");
    lcd.print(currentTemp);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("M:");
    lcd.print(minTemp);
    lcd.print("Max:");
    lcd.print(maxTemp);
    lcd.print(" C");

    // Actualizarea stării LED-urilor în funcție de temperatura curentă
    updateLEDs(currentTemp);
  }
}

// Funcția de citire a temperaturii de la senzor
float readTemperature() {
  // Citirea valorii brute de la senzor
  int rawReading = analogRead(SENSOR_PIN);

  // Convertirea valorii brute în tensiune (în volți)
  float voltage = rawReading * 5.0 / 1024.0;

  // Convertirea tensiunii în temperatură (în grade Celsius)
  float temp = (voltage - 0.5) * 100;

  // Actualizarea temperaturilor minime și maxime
  updateMinMax(temp);

  // Returnarea temperaturii convertite
  return temp;
}

// Funcția pentru actualizarea temperaturilor minime și maxime
void updateMinMax(float temp) {
  // Actualizarea temperaturii minime
  if (temp < minTemp) {
    minTemp = temp;
  }

  // Actualizarea temperaturii maxime
  if (temp > maxTemp) {
    maxTemp = temp;
  }
}

// Funcția pentru actualizarea stării LED-urilor
void updateLEDs(float temp) {
  // Verificarea temperaturii pentru aprinderea LED-urilor
  if (temp < 5) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
  } else if (temp > 35) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
  }
}
