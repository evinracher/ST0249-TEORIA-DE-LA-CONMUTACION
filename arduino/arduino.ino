#define debug_print(x) Serial.print(x)

void setup() {
  Serial.begin(9600);
}

bool active = false, coordinates = false, route = false;

void serialEvent() {
  byte input[200] = {};

  Serial.readBytesUntil('\n', input, 200);

  if(active) {
    if(coordinates) {
      if(c[0] == '\000') {
        coordinates = false;
        route = true;
      } else {
        //Do something
        debug_print("coordinate");
      }
    } else if(route) {
      if(c[0] == '\000') {
        route = false;
        active = false;
      } else {
        //Do something
        debug_print("point in route");
      }
    } else {
      if(c[0] == '\000') {
        active = true;
        coordinates = true;
      }
    }

    Serial.write('\n');
  }
}

void loop() {
  //Do nothing
}
