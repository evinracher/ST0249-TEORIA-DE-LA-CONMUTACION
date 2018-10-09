#define debug_println(x) Serial.println(x)

void setup() {
  Serial.begin(9600);
}

bool active = false, coordinates = false, route = false;
byte const CRAZY_SIGNAL = '\000';

void request_input() {
  debug_println("sending signal");
  Serial.write(CRAZY_SIGNAL);
}

void serialEvent() {
  char input[200] = {};
  Serial.readBytesUntil('\n', input, 200);
  Serial.print(input);
}

void loop() {
}
