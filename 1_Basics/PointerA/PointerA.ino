uint8_t someValue = 0;

void giveRandomValue(uint8_t *valPtr) {
  *valPtr = random(50, 200);
}

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");
}

void loop() {

  giveRandomValue(&someValue);

  Serial.print("New value: ");
  Serial.println(someValue);
  // someValue is updated indirectly by giveRandomValue() using a pointer,
  // Not directly with something like 'someValue = 87;'

  delay(1000);
}

// TODO: Move the giveRandomValue() function into a separate '.h' and '.cpp' file
