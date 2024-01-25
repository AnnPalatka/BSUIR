#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Stepper.h>

RF24 radio(9, 10); // Пины CE, CSN

const uint64_t adress = 0xABCDABCD01LL; // Адрес канала для связи

//подключения шагового двигател
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

bool button1Pressed = false;
bool button2Pressed = false;

// Пины подключения драйверов двигателей
const int motorBL_A = 2; // направление
const int motorBL_B = 3; // скорость
const int motorFL_A = 4;
const int motorFL_B = 5;
const int motorFR_A = 6;
const int motorFR_B = 7;
const int motorBR_A = 8;
const int motorBR_B = 9;


const int led1 = 2;
const int led2 = 3;
const int ledRedPin1 = 5;
const int ledGreenPin1 = 6;
const int ledBluePin1 = 9;
const int ledRedPin2 = 10;
const int ledGreenPin2 = 11;
const int ledBluePin2 = 12;

void setup() {
    radio.begin();
    radio.openReadingPipe(1, adress);
    radio.startListening();

    myStepper.setSpeed(36); // скорость вращения (36 градусов в секунду)

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(ledRedPin1, OUTPUT);
    pinMode(ledGreenPin1, OUTPUT);
    pinMode(ledBluePin1, OUTPUT);
    pinMode(ledRedPin2, OUTPUT);
    pinMode(ledGreenPin2, OUTPUT);
    pinMode(ledBluePin2, OUTPUT);

    // Установка пинов для управления двигателями
    pinMode(motorBL_A, OUTPUT);
    pinMode(motorBL_B, OUTPUT);
    pinMode(motorFL_A, OUTPUT);
    pinMode(motorFL_B, OUTPUT);
    pinMode(motorFR_A, OUTPUT);
    pinMode(motorFR_B, OUTPUT);
    pinMode(motorBR_A, OUTPUT);
    pinMode(motorBR_B, OUTPUT);
}

void loop() {
    if (radio.available()) {
        int actions[5];
        radio.read(&actions, sizeof(actions));
        
        performAction(actions[0]);

        tower(actions[1]);

        handleFirstElement(actions[2]);
        handleSecondElement(actions[3]);
        if (actions[3] == 1)
        {
            handleThirdElement(actions[4]);
        }
    }
}

// Функция для обработки третьего элемента массива
void handleFirstElement(int value) {
    if (value == 1) {
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
    }
    else if (value == 0) {
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
    }
}

// Функция для обработки четвертого элемента массива
void handleSecondElement(int value) {
    if (value == 1) {
        analogWrite(ledRedPin1, 255); // Включаем красный светодиод 1
        analogWrite(ledRedPin2, 255); // Включаем красный светодиод 2
    }
    else if (value == 0) {
        analogWrite(ledRedPin1, 0); // Выключаем красный светодиод 1
        analogWrite(ledRedPin2, 0); // Выключаем красный светодиод 2
        analogWrite(ledGreenPin1, 0); // Выключаем зеленый светодиод 1
        analogWrite(ledGreenPin2, 0); // Выключаем зеленый светодиод 2
        analogWrite(ledBluePin1, 0); // Выключаем синий светодиод 1
        analogWrite(ledBluePin2, 0); // Выключаем синий светодиод 2
    }
}

// Функция для обработки пятого элемента массива
void handleThirdElement(int value) {
    if (value == 1) {
        analogWrite(ledGreenPin1, 0); // Устанавливаем зеленый на минимум для светодиода 1
        analogWrite(ledBluePin1, 0); // Устанавливаем синий на минимум для светодиода 1
        analogWrite(ledGreenPin2, 0); // Устанавливаем зеленый на минимум для светодиода 2
        analogWrite(ledBluePin2, 0); // Устанавливаем синий на минимум для светодиода 2
        analogWrite(ledRedPin1, 255); // Устанавливаем красный на максимум для светодиода 1
        analogWrite(ledRedPin2, 255); // Устанавливаем красный на максимум для светодиода 2
    }
    else if (value == 2) {
        analogWrite(ledRedPin1, 0); // Выключаем красный светодиод 1
        analogWrite(ledRedPin2, 0); // Выключаем красный светодиод 2
        analogWrite(ledGreenPin1, 255); // Устанавливаем зеленый на максимум для светодиода 1
        analogWrite(ledGreenPin2, 255); // Устанавливаем зеленый на максимум для светодиода 2
        analogWrite(ledBluePin1, 0); // Устанавливаем синий на минимум для светодиода 1
        analogWrite(ledBluePin2, 0); // Устанавливаем синий на минимум для светодиода 2
    }
    else if (value == 3) {
        analogWrite(ledRedPin1, 0); // Выключаем красный светодиод 1
        analogWrite(ledRedPin2, 0); // Выключаем красный светодиод 2
        analogWrite(ledGreenPin1, 0); // Устанавливаем зеленый на минимум для светодиода 1
        analogWrite(ledGreenPin2, 0); // Устанавливаем зеленый на минимум для светодиода 2
        analogWrite(ledBluePin1, 255); // Устанавливаем синий на максимум для светодиода 1
        analogWrite(ledBluePin2, 255); // Устанавливаем синий на максимум для светодиода 2
    }
}

void tower(int receivedMessage) {
    if (receivedMessage == 1 && !button1Pressed) {
        button1Pressed = true;
    }
    else if (receivedMessage == 2 && !button2Pressed) {
        button2Pressed = true;
    }

    if (button1Pressed) {
        myStepper.step(-1); // вращение влево
        if (receivedMessage != 1) {
            myStepper.step(0); // остановка двигателя
            button1Pressed = false;
        }
    }
    else if (button2Pressed) {
        myStepper.step(1); // вращение вправо
        if (receivedMessage != 2) {
            myStepper.step(0); // остановка двигателя
            button2Pressed = false;
        }
    }
}

void performAction(int action) {
    switch (action) {
    case 0:
        // Ничего не делать
        stopMotors();
        break;
    case 1:
        // Ехать вперед
        forward();
        break;
    case 2:
        // Ехать назад
        backward();
        break;
    case 3:
        // Поворот влево при езде вперед
        forwardLeft();
        break;
    case 4:
        // Поворот вправо при езде вперед
        forwardRight();
        break;
    case 5:
        // Поворот влево при езде назад
        backwardLeft();
        break;
    case 6:
        // Поворот вправо при езде назад
        backwardRight();
        break;
    case 7:
        // Поворот влево на месте
        spinLeft();
        break;
    case 8:
        // Поворот вправо на месте
        spinRight();
        break;
    case 9:
        // Езда боком влево
        sidewaysLeft();
        break;
    case 10:
        // Езда боком вправо
        sidewaysRight();
        break;
    default:
        stopMotors();
    }
}

// Функции управления двигателями в зависимости от вашей конфигурации
void forward() {
    digitalWrite(motorBL_A, HIGH);
    digitalWrite(motorBL_B, LOW);
    digitalWrite(motorFL_A, HIGH);
    digitalWrite(motorFL_B, LOW);
    digitalWrite(motorFR_A, HIGH);
    digitalWrite(motorFR_B, LOW);
    digitalWrite(motorBR_A, HIGH);
    digitalWrite(motorBR_B, LOW);
}

void backward() {
    digitalWrite(motorBL_A, LOW);
    digitalWrite(motorBL_B, HIGH);
    digitalWrite(motorFL_A, LOW);
    digitalWrite(motorFL_B, HIGH);
    digitalWrite(motorFR_A, LOW);
    digitalWrite(motorFR_B, HIGH);
    digitalWrite(motorBR_A, LOW);
    digitalWrite(motorBR_B, HIGH);
}

void forwardLeft() {
    forward();
    digitalWrite(motorBL_B, LOW); // Уменьшаем скорость левой стороны
}

void forwardRight() {
    forward();
    digitalWrite(motorFR_B, LOW); // Уменьшаем скорость правой стороны
}

void backwardLeft() {
    backward();
    digitalWrite(motorBL_B, LOW); // Уменьшаем скорость левой стороны
}

void backwardRight() {
    backward();
    digitalWrite(motorBR_B, LOW); // Уменьшаем скорость правой стороны
}

void spinLeft() {
    digitalWrite(motorBL_A, HIGH);
    digitalWrite(motorBL_B, LOW);
    digitalWrite(motorFL_A, HIGH);
    digitalWrite(motorFL_B, LOW);
    digitalWrite(motorFR_A, LOW);
    digitalWrite(motorFR_B, HIGH);
    digitalWrite(motorBR_A, LOW);
    digitalWrite(motorBR_B, HIGH);
}

void spinRight() {
    digitalWrite(motorBL_A, LOW);
    digitalWrite(motorBL_B, HIGH);
    digitalWrite(motorFL_A, LOW);
    digitalWrite(motorFL_B, HIGH);
    digitalWrite(motorFR_A, HIGH);
    digitalWrite(motorFR_B, LOW);
    digitalWrite(motorBR_A, HIGH);
    digitalWrite(motorBR_B, LOW);
}

void sidewaysLeft() {
    digitalWrite(motorBL_A, HIGH);
    digitalWrite(motorBL_B, LOW);
    digitalWrite(motorFL_A, LOW);
    digitalWrite(motorFL_B, HIGH);
    digitalWrite(motorFR_A, HIGH);
    digitalWrite(motorFR_B, LOW);
    digitalWrite(motorBR_A, LOW);
    digitalWrite(motorBR_B, HIGH);
}

void sidewaysRight() {
    digitalWrite(motorBL_A, LOW);
    digitalWrite(motorBL_B, HIGH);
    digitalWrite(motorFL_A, HIGH);
    digitalWrite(motorFL_B, LOW);
    digitalWrite(motorFR_A, LOW);
    digitalWrite(motorFR_B, HIGH);
    digitalWrite(motorBR_A, HIGH);
    digitalWrite(motorBR_B, LOW);
}

void stopMotors() {
    digitalWrite(motorBL_A, LOW);
    digitalWrite(motorBL_B, LOW);
    digitalWrite(motorFL_A, LOW);
    digitalWrite(motorFL_B, LOW);
    digitalWrite(motorFR_A, LOW);
    digitalWrite(motorFR_B, LOW);
    digitalWrite(motorBR_A, LOW);
    digitalWrite(motorBR_B, LOW);
}




#include "esp_camera.h"
#include <WiFi.h>

#define CAMERA_MODEL_AI_THINKER 
#define CAMERA_MODEL_ESP32_CAM_BOARD

#include "camera_pins.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char* ssid = "**********";
const char* password = "**********";

void startCameraServer();
void setupLedFlash(int pin);

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG; // for streaming
    //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
    //                      for larger pre-allocated frame buffer.
    if (config.pixel_format == PIXFORMAT_JPEG) {
        if (psramFound()) {
            config.jpeg_quality = 10;
            config.fb_count = 2;
            config.grab_mode = CAMERA_GRAB_LATEST;
        }
        else {
            // Limit the frame size when PSRAM is not available
            config.frame_size = FRAMESIZE_SVGA;
            config.fb_location = CAMERA_FB_IN_DRAM;
        }
    }
    else {
        // Best option for face detection/recognition
        config.frame_size = FRAMESIZE_240X240;


    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t* s = esp_camera_sensor_get();
    // initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID) {
        s->set_vflip(s, 1); // flip it back
        s->set_brightness(s, 1); // up the brightness just a bit
        s->set_saturation(s, -2); // lower the saturation
    }
    // drop down frame size for higher initial frame rate
    if (config.pixel_format == PIXFORMAT_JPEG) {
        s->set_framesize(s, FRAMESIZE_QVGA);
    }



    WiFi.begin(ssid, password);
    WiFi.setSleep(false);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    startCameraServer();

    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
}

void loop() {
    // Do nothing. Everything is done in another task by the web server
    delay(10000);
}