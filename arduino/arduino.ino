#define debug_println(x) Serial.println(x)

bool coordinates, route;
byte const CRAZY_SIGNAL = '>';

void setup() {
  coordinates = route = false;
  Serial.begin(9600);
}

void request_input() {
  Serial.write(CRAZY_SIGNAL);
}

void serialEvent() {
  debug_println("input found!");

  char input[256] = {};
  Serial.readBytesUntil('\n', input, 256);

  if (input[0] == CRAZY_SIGNAL) {
    debug_println("CRAZY_SIGNAL found!");

    if (coordinates) {
      coordinates = false;
      route = true;
    } else if (route) {
      route = false;
    } else {//! coordinates && ! route
      coordinates = true;
    }
  } else {
    if (coordinates) {
      //Do something
      debug_println("coordinate");
    } else if (route) {
      //Do something
      debug_println("point in route");
    }
  }

  if (coordinates || route) {
    request_input();
  }
}

void loop() {
  //Do nothing
}
