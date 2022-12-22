// Universidade Estadual do Maranhao
// Centro de Ciencias Tecnologicas
// Engenharia de Computacao

// Microprocessadores e Microcontroladores

// Alunos:
// Gabriel Cancio Costa
// Joao Gabriel Pereira Ferreira
// Jonas Carvalho de Sousa Neto
// Luis Gustavo Diniz Pereira
// Schenaid Cristiano Mendes Pereira Junior|

// Atom Lite (ESP32)

#include <M5Atom.h>
#include <stdbool.h>

// Create Tasks
TaskHandle_t task1;
TaskHandle_t task2;

// Create queue for action button/led
QueueHandle_t xQueueButton;

typedef struct {
  bool status;
} info_button_t;

info_button_t info_button;

void setup() {
  M5.begin(true, false, true); // Initialize M5Atom.
  Serial.begin(115200);        // Serial Monitor in 115200.
  
  // Initialize Button
  xQueueButton = xQueueCreate(5, sizeof(info_button));

  // Run tasks
  xTaskCreatePinnedToCore(ledTask, "task1", 1000, NULL, 0, &task1, 0);
  xTaskCreatePinnedToCore(buttonTask, "task2", 1000, NULL, 1, &task2, 1);
}

void bright(int intensity) { // Set intensity bright simple
  M5.dis.setBrightness(intensity); 
}

void buttonTask(void *pvParameters) {
  info_button_t info_button;
  
  while (true) {
    if (M5.Btn.read()) {
      // debounce 300ms
      vTaskDelay(300 / portTICK_RATE_MS);
      
      info_button.status = !info_button.status;
    } 
    // Send information for queue
    xQueueSend(xQueueButton, &info_button, portMAX_DELAY);
  }
}

void ledTask(void *pvParameters) {
  info_button_t info_button;
  while (true) {
    // Get information from queue
    xQueueReceive(xQueueButton, &info_button, portMAX_DELAY);

    // Processing button_status
    if (info_button.status) {
      M5.dis.fillpix(0x00F00F);
      Serial.println("On.\n");
      bright(100); 
    } else {
      Serial.println("Off.\n");
      M5.dis.clear();
    }
  }
}

void loop() {
  
}