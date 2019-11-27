#ifndef ICHICK_ARDUINO_LOG_H
#define ICHICK_ARDUINO_LOG_H

#ifndef LOG
#define LOG(msg) Serial.print(msg)
#endif LOG

#ifndef LOG_N
#define LOG_N(msg) Serial.println(msg)
#endif LOG_N


#endif //ICHICK_ARDUINO_LOG_H
