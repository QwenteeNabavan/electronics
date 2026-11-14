const int ntcPin = A0;
const int rPin = 5;
const int gPin = 6;
const int bPin = 7;
const int sensorPin = 10;

float currentTemp = 0;

bool dangerState = false;
bool warnState = false;
bool safeState = false;

void setRgb(int rVal, int gVal, int bVal) {
  analogWrite(rPin, rVal);
  analogWrite(gPin, gVal);
  analogWrite(bPin, bVal);
}

void updateLed(bool danger, bool warning, bool safe) {
  if (safe) {
    setRgb(255, 255, 255);
    delay(1000);
    setRgb(0, 0, 0);
    delay(1000);
  } else if (warning) {
    setRgb(255, 165, 0);
    delay(1000);
    setRgb(0, 0, 0);
    delay(1000);
  } else if (danger) {
    setRgb(255, 0, 0);
    delay(1000);
    setRgb(0, 0, 0);
    delay(1000);
  }
}

float getTemperature() {
  const int maxAdc = 1023;
  const float rSeries = 4700.0;
  const float rNominal = 10000.0;
  const float betaCoeff = 3950.0;
  const float tempNominalK = 25.0 + 273.15;

  int rawVal = analogRead(ntcPin);
  if (rawVal < 1) {
    rawVal = 1;
  }

  // Розрахунок опору NTC-термістора
  float rThermistor = rSeries * ((float)maxAdc / (float)rawVal - 1.0);

  // Обчислення температури у Кельвінах та переведення в Цельсії
  float invTemp = (1.0 / tempNominalK) + (1.0 / betaCoeff) * log(rThermistor / rNominal);
  float kelvin = 1.0 / invTemp;
  float celsius = kelvin - 273.15;

  return celsius;
}

void setup() {
  Serial.begin(9600);

  pinMode(rPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(ntcPin, INPUT);

  analogReference(DEFAULT);
}

void loop() {
  safeState = false;
  warnState = false;
  dangerState = false;

  int pinState = digitalRead(sensorPin);
  float currentTemp = getTemperature();
  Serial.println(currentTemp);

  if (currentTemp < 30) {
    safeState = true;
  }
  if (currentTemp > 30) {
    warnState = true;
  }
  if (sensorPin == HIGH || currentTemp > 50) {
    dangerState = true;
  }

  updateLed(dangerState, warnState, safeState);
}