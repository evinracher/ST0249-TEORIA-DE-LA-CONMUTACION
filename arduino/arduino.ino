#define debug_println(x) Serial.println(x)

void setup() {
  Serial.begin(9600);
}

bool active = false, coordinates = false, route = false;
byte const CRAZY_SIGNAL = '\000';

void request_input() {
  Serial.write(CRAZY_SIGNAL);
}

void serialEvent() {
  byte input[200] = {};
  Serial.readBytesUntil('\n', input, 200);

  if(active) {
    if(coordinates) {
      if(input[0] == CRAZY_SIGNAL) {
        coordinates = false;
        route = true;
      } else {
        //Do something
        debug_println("coordinate");
      }

      request_input();
    } else if(route) {
      if(input[0] == CRAZY_SIGNAL) {
        route = false;
        active = false;
      } else {
        //Do something
        debug_println("point in route");
        request_input();
      }
    } else {
      if(input[0] == CRAZY_SIGNAL) {
        active = true;
        coordinates = true;
        request_input();
      }
    }
  }
}

void loop() {
  //Do nothing
}
