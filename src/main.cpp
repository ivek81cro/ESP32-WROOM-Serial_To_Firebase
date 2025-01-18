#include "serialToFirestore.h"
#include <addons/TokenHelper.h>

SerialToFirestore serialToFirestore;

void setup() {
  serialToFirestore.setup();
}

void loop() {
  serialToFirestore.loop();
}
