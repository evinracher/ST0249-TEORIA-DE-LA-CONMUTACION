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

String getMsg(int msg) {
  switch (msg) {
    case 0:
      return "Viajando a posicion";
    case 1:
      return "Recoger producto";
    case 2:
      return "Finalizar ruta";
    case 3:
      return "Recoger canastas llenas, y colocar canastas vacias";
    case 4:
      return "Viajando para clasificar";
    case 5:
      return "Clasificar productos en medios";
    case 6:
      return "Iniciar una nueva consolidacion";
    default:
      return "No conocido";
  }
}


String getAction(int action) {
  switch (action) {
    case 0:
      return "Viajar";
    case 1:
      return "Parar para que el usuario tome productos en picker to parts";
    case 2:
      return "Parar y finalizar ruta";
    case 3:
      return "Parar para que el usuario tome productos en pick and sort";
    case 4:
      return "Parar para que el usuario tome productos en pick and sort";
    default:
      return "No conocida";
  }
}

bool x_state = false, y_state = false;
float x_diff = 0, y_diff = 0;
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

            Point prev_point = points[prev_point_id];
            Point curr_point = points[point_id];

            if (curr_point.x != prev_point.x) {
              float diff = curr_point.x - prev_point.x;
              if (y_state) {

                Serial.print("Move from ");
                Serial.print(prev_point_id);
                Serial.print(" to ");
                Serial.println(point_id);
                Serial.print(y_diff > 0 ? "+" : "");
                Serial.print(y_diff);
                Serial.println(" in y");
                y_state = false;
                y_diff = 0;
                Serial.print("Message: ");
                Serial.println(getMsg((message_code[1]) - '0'));
                Serial.print("Action: ");
                Serial.println(getAction(action));

              }
              x_diff += diff;
              x_state = true;
            }

            if (curr_point.y != prev_point.y) {
              float diff = curr_point.y - prev_point.y;
              if (x_state) {

                Serial.print("Move from ");
                Serial.print(prev_point_id);
                Serial.print(" to ");
                Serial.println(point_id);
                Serial.print(x_diff > 0 ? "+" : "");
                Serial.print(x_diff);
                Serial.println(" in x");
                x_state = false;
                x_diff = 0;
                Serial.print("Message: ");
                Serial.println(getMsg((message_code[1]) - '0'));
                Serial.print("Action: ");
                Serial.println(getAction(action));

              }
              y_diff += diff;
              y_state = true;
            }
          } else {
            Serial.print("Stay at ");
            Serial.println(point_id);
          }
        } else {
          Serial.print("Start at ");
          Serial.println(point_id);
        }

        if (action != 0) {
          Serial.print("Message: ");
          Serial.println(getMsg((message_code[1]) - '0'));
          Serial.print("Action: ");
          Serial.println(getAction(action));
        }
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
