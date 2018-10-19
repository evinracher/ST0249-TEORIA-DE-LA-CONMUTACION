char const CRAZY_SIGNAL = '>';
int prev_point_id;

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
        prev_point_id = -1;
        break;
      case READING_ROUTE:
        state = INACTIVE;
        Serial.println("Stop");
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

        if (prev_point_id != -1) {
          if (prev_point_id != point_id) {
            Serial.print("Move from ");
            Serial.print(prev_point_id);
            Serial.print(" to ");
            Serial.println(point_id);

            Point prev_point = points[prev_point_id];
            Point curr_point = points[point_id];

            if (curr_point.x != prev_point.x) {
              float diff = curr_point.x - prev_point.x;
              Serial.print(diff > 0 ? "+" : "");
              Serial.print(diff);
              Serial.println(" in x");
            }

            if (curr_point.y != prev_point.y) {
              float diff = curr_point.y - prev_point.y;
              Serial.print(diff > 0 ? "+" : "");
              Serial.print(diff);
              Serial.println(" in y");
            }
          } else {
            Serial.print("Stay at ");
            Serial.println(point_id);
          }
        } else {
          Serial.print("Start at ");
          Serial.println(point_id);
        }

        Serial.print("Message: ");
        Serial.println(message_code);
        Serial.print("Action: ");
        Serial.println(action);

        prev_point_id = point_id;
        break;
    }
  }

  while (Serial.available()) {
    Serial.read();
  }
}

void loop() {

}
