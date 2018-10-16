#define debug_println(x) Serial.println(x)
#define debug_print(x) Serial.print(x)

char const CRAZY_SIGNAL = '>';

enum State {
  idle, coordinates, route
};

State state;

void setup() {
  state = idle;
  Serial.begin(115200);
}

void request_input() {
  Serial.write(CRAZY_SIGNAL);
}

void serialEvent() {
  delay(60);
  debug_println("input found!!");

  String input = Serial.readStringUntil('\n');

  debug_print("input: '"), debug_print(input), debug_println("'");

  if (input.indexOf(CRAZY_SIGNAL) != -1) {
    debug_println("CRAZY_SIGNAL found!!");

    switch (state) {
      case idle:
        debug_println("start!!");
        state = coordinates;
        break;
      case coordinates:
        debug_println("route!!");
        state = route;
        break;
      case route:
        debug_println("end!!");
        state = idle;
        break;
    }
  } else {
    switch (state) {
      case idle:
        debug_println("nothing!!");
        break;
      case coordinates:
        debug_println("coordinate!!");
        break;
      case route:
        debug_println("point in route!!");
        break;
    }
  }

  if (state != idle) {
    debug_println("request input!!");
    request_input();
  }
}

void loop() {

}
