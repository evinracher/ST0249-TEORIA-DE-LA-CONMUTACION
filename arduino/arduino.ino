#define debug_println(x) Serial.println(x)
#define debug_print(x) Serial.print(x)

bool coordinates, route;
char const CRAZY_SIGNAL = '>';

void setup() {
  coordinates = route = false;
  Serial.begin(9600);
}

void request_input() {
  Serial.write(CRAZY_SIGNAL);
}

void serialEvent() {
  debug_println("input found!!");

  char input[256] = {};
  Serial.readBytesUntil('\n', input, 256);

  debug_print("input: '");
  debug_print(input);
  debug_println("'");

  if (input[0] == CRAZY_SIGNAL) {
    debug_println("CRAZY_SIGNAL found!!");

    if (coordinates) {
      debug_println("route!!");
      coordinates = false;
      route = true;
    } else if (route) {
      debug_println("end!!");
      route = false;
    } else {//! coordinates && ! route
      debug_println("start!!");
      coordinates = true;
    }
  } else {
    if (coordinates) {
      debug_println("coordinate!!");
    } else if (route) {
      debug_println("point in route!!");
    }
  }

  if (coordinates || route) {
    debug_println("request input!!");
    request_input();
  }
}

void loop() {

}
