#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>

typedef struct valeurCapteurs {
  int analogique;
  int numerique;
  unsigned long tempsEnMilliseconds;
} valeursCapteurs;

QueueHandle_t q_tache_1;
QueueHandle_t q_tache_2;
QueueHandle_t q_tache_3;
QueueHandle_t q_tache_4;

SemaphoreHandle_t  xMutexSerial = NULL;

valeursCapteurs vals;
int val_numerique;
int val_analogique;
int PIN_3 = 3;
int PIN_4 = 4;

void tache_1(void*);
void tache_2(void*);
void tache_3(void*);
void tache_4(void*);
void tache_5(void*);

void printer(String);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  //Serial.println("debut");
  pinMode(PIN_3, INPUT);
  pinMode(PIN_4, INPUT);

  xMutexSerial = xSemaphoreCreateMutex();
  if (xMutexSerial != NULL) {
    Serial.println("Mutex 1 created");
  }
  q_tache_1 = xQueueCreate(10, sizeof(int));
  q_tache_2 = xQueueCreate(10, sizeof(int));
  q_tache_3 = xQueueCreate(10, sizeof(valeurCapteurs));
  q_tache_4 = xQueueCreate(10, sizeof(valeurCapteurs));

  xTaskCreate(tache_1, "tache_1", 100,  NULL,  3  ,  NULL );
  xTaskCreate(tache_2, "tache_2", 100,  NULL,  3  ,  NULL );
  xTaskCreate(tache_3, "tache_3", 100,  NULL,  3  ,  NULL );
  xTaskCreate(tache_4, "tache_4", 100,  NULL,  3  ,  NULL );
  xTaskCreate(tache_5, "tache_5", 200,  NULL,  3  ,  NULL );
}

void loop() {
  // put your main code here, to run repeatedly:
}




void tache_1(void* p) {
  while (1) {
    val_analogique = analogRead(A0);
    xQueueSendToBack(q_tache_1, &val_analogique, portMAX_DELAY );
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void tache_2(void* p) {
  while (1) {
    val_numerique =  ((digitalRead(PIN_3)==HIGH)?1:0) + ((digitalRead(PIN_4)==HIGH)?1:0);
    xQueueSendToBack(q_tache_2, &val_numerique, portMAX_DELAY );
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void tache_3(void* p) {
  while (1) {
    if ((xQueueReceive( q_tache_1, &vals.analogique, portMAX_DELAY ) == pdTRUE) && (xQueueReceive( q_tache_2, &vals.numerique, portMAX_DELAY ) == pdTRUE)) {
      vals.tempsEnMilliseconds = millis();
      xQueueSendToBack(q_tache_3, &vals, portMAX_DELAY );
    }
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void tache_4(void* p) {
  valeursCapteurs val;
  while (1) {
    if (xQueueReceive( q_tache_3, &val, portMAX_DELAY ) == pdTRUE) {
      xQueueSendToBack(q_tache_4, &val, portMAX_DELAY );
      printer((String)("valeur analogique : ") + val.analogique + (String)("     valeur numerique : ") + val.numerique + (String)("\ntemps (ms) : ") + val.tempsEnMilliseconds );
    }
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void tache_5(void* p) {
  while (1) {
    if ((xQueueReceive( q_tache_4, &vals, portMAX_DELAY ) == pdTRUE)) {
      printer((String)("      temps (min) : ") + (vals.tempsEnMilliseconds/60000)+ (String)(" min\n"));
    }
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void printer(String s) {
  if( xSemaphoreTake(xMutexSerial, portMAX_DELAY) == pdTRUE){
    Serial.print(s);
    xSemaphoreGive(xMutexSerial); // release mutex
  }
 
}
