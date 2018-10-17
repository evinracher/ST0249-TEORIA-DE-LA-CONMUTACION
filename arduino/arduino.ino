char const CRAZY_SIGNAL = '>';

enum State {
  INACTIVE, READING_POINTS, READING_ROUTE
} state = INACTIVE;

struct Point {
  int id;
  float x, y;
} points[100] = {};

void setup() {
  state = INACTIVE;
  Serial.begin(115200);
}

void serialEvent() {
  delay(30);

  if (Serial.peek() == CRAZY_SIGNAL) {
    switch (state) {
      case INACTIVE:
        state = READING_POINTS;
        break;
      case READING_POINTS:
        state = READING_ROUTE;
        break;
      case READING_ROUTE:
        state = INACTIVE;
        break;
    }
  } else {
    switch (state) {
      case INACTIVE:
        break;
      case READING_POINTS:
        {
          Point temp;
          temp.id = Serial.parseInt();
          temp.x = Serial.parseFloat();
          temp.y = Serial.parseFloat();
          points[temp.id] = temp;
          Serial.println(temp.id);
          Serial.println(temp.x);
          Serial.println(temp.y);
        }
        break;
      case READING_ROUTE:
        int point_id = Serial.readStringUntil(',').toInt();
        int action = Serial.readStringUntil(',').toInt();
        int request_id = Serial.readStringUntil(',').toInt();
        int reference = Serial.readStringUntil(',').toInt();
        int quantity = Serial.readStringUntil(',').toInt();
        String pos = Serial.readStringUntil(',');
        String message_code = Serial.readStringUntil(',');
        int car_position = Serial.readStringUntil(',').toInt();
        int confirmed_quantity = Serial.readStringUntil(',').toInt();
        String start_hour = Serial.readStringUntil(',');
        String confirmed_hour = Serial.readStringUntil('\n');

        Serial.println(point_id);
        Serial.println(action);
        Serial.println(request_id);
        Serial.println(reference);
        Serial.println(quantity);
        Serial.println(pos);
        Serial.println(message_code);
        Serial.println(car_position);
        Serial.println(confirmed_quantity);
        Serial.println(start_hour);
        Serial.println(confirmed_hour);
        break;
    }
  }

  while (Serial.available()) {
    Serial.read();
  }
}

void loop() {

}
