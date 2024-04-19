#include <stdbool.h>
#include <stdio.h>
#include <sdkconfig.h>
#include <inttypes.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "driver/ledc.h"
#include "freertos/timers.h"
#include "sys/time.h"
#define SPP_TAG "SPP_ACCEPTOR_DEMO"
#define SPP_SERVER_NAME "SPP_SERVER"
#define EXAMPLE_DEVICE_NAME "LucesRGB"
#define SPP_SHOW_DATA 0
#define SPP_SHOW_SPEED 1
#define SPP_SHOW_MODE SPP_SHOW_SPEED    /*Choose show mode: show data or speed*/
#define CONFIG_FREERTOS_HZ 100


uint16_t receivedValue = 0;
uint8_t pasos = 0;
uint8_t juego = 1;
TimerHandle_t xTimers;
TimerHandle_t x1Timers;
TickType_t myInterval ;
TickType_t myInterval1 ;
int intervalo = 10;
int modificointervalo = 0;
int llamado = 0;
int lumenes = 0;
int timerId = 1;
int timerId1 = 1;


static const esp_spp_mode_t esp_spp_mode = ESP_SPP_MODE_CB;
static const bool esp_spp_enable_l2cap_ertm = true;
static struct timeval  time_old;
static const esp_spp_sec_t sec_mask = ESP_SPP_SEC_AUTHENTICATE;
static const esp_spp_role_t role_slave = ESP_SPP_ROLE_SLAVE;


static char *bda2str(uint8_t * bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18)
    {
        return NULL;
    }
    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}



void inicioescena(){
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);//azul
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5);//azul
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);//azul
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3);//azul
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6);//azul
}

void PuertoSalida(ledc_mode_t grupo, ledc_channel_t puerto, int luminosidad){
    ledc_set_duty(grupo, puerto, luminosidad);//rojo
    ledc_update_duty(grupo, puerto);//rojo
}
 
void ApagarTimer(){
    if(xTimers != NULL) {
        xTimerStop(xTimers, 0);
        } 
}

typedef void (*TimerCallback)(TimerHandle_t xTimers);

void llamada1(TimerHandle_t xTimers) {
   if (pasos == 0){
         lumenes = lumenes + 1;
       ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, lumenes);//rojo
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);//rojo
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, lumenes);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);//verde
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, lumenes);//azul
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            if (lumenes == 255){
            pasos = 1;
            }
        }
   else if (pasos == 1){
        lumenes = lumenes - 1;
       ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, lumenes);//rojo
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);//rojo
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, lumenes);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);//verde
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, lumenes);//azul
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        if (lumenes == 0){
            pasos = 2;
        }
    }
   else if (pasos == 2){
        lumenes = lumenes + 1;
       ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, lumenes);//azul
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        if (lumenes == 255){
            pasos = 3;
        }
    }
else if (pasos == 3){
        lumenes = lumenes - 1;
       ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, lumenes);//azul
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        if (lumenes == 0){
            pasos = 4;
            lumenes = 255;
        }
    }
 else if (pasos == 4){
        lumenes = lumenes - 1;
      ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, lumenes);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);//verde
        if (lumenes == 0){
            pasos = 5;
            lumenes = 255;
        }
    }
 else if (pasos == 5){
        lumenes = lumenes - 1;
      ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, lumenes);//rojo
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);//rojo
        if (lumenes == 0){
            pasos = 0;
        }
    }
}
void llamada2(TimerHandle_t xTimers) {
     receivedValue = 225;
   if (pasos == 0){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    pasos = 1; }
    else if(pasos == 1){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
    pasos = 2; }
    else if(pasos == 2){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
    pasos = 3; }
    else if(pasos == 3){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    pasos = 4; }
    else if(pasos == 4){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
    pasos = 5; }
    else if(pasos == 5){
    receivedValue = 0;
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
    pasos = 6; }
    else if(pasos == 6){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
    pasos = 7; }
    else if(pasos == 7){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    pasos = 8; }
    else if(pasos == 8){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
    pasos = 9; }
    else if(pasos == 9){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
    pasos = 10; }
    else if(pasos == 10){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    pasos = 11; }
        else if(pasos == 11){
    receivedValue = 0;
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);

    pasos = 12; }
    else if(pasos == 12){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    pasos = 13; }
    else if(pasos == 13){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
    pasos = 14; }
    else if(pasos == 14){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
    pasos = 15; }
    else if(pasos == 15){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
    pasos = 16; }
    else if(pasos == 16){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    pasos = 17; }
        else if(pasos == 17){
    receivedValue = 0;
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);

    pasos = 0; }

}
void llamada3(TimerHandle_t xTimers) {
    if(pasos == 0){
    receivedValue = receivedValue +1;
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);
        if(receivedValue == 255){
            pasos = 1;
            receivedValue = 0;
        }
    }else if(pasos == 1){
    receivedValue = receivedValue +1;
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5);
        if(receivedValue == 255){
            pasos = 2;
            receivedValue = 0;
        }
    }else if(pasos == 2){
    receivedValue = receivedValue +1;
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        if(receivedValue == 255){
            pasos = 3;
            receivedValue = 0;
        }
    }else if(pasos == 3){
    receivedValue = receivedValue +1;
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3);
        if(receivedValue == 255){
            pasos = 4;
            receivedValue = 0;
        }
    }else if(pasos == 4){
    receivedValue = receivedValue +1;
       ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6);
        if(receivedValue == 255){
            pasos = 5;
        }
    }else if(pasos == 5){
    receivedValue = receivedValue -1;
       ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6);
        if(receivedValue == 0){
            pasos = 6;
            receivedValue = 255;
        }
    }else if(pasos == 6){
    receivedValue = receivedValue -1;
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3);
        if(receivedValue == 0){
            pasos = 7;
            receivedValue = 255;
        }
    }else if(pasos == 7){
    receivedValue = receivedValue -1;
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        if(receivedValue == 0){
            pasos = 8;
            receivedValue = 255;
        }
    }else if(pasos == 8){
    receivedValue = receivedValue -1;
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5);
        if(receivedValue == 0){
            pasos = 9;
            receivedValue = 255;
        }
    }else if(pasos == 9){
    receivedValue = receivedValue -1;
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);
        if(receivedValue == 0){
            pasos = 0;
            receivedValue = 0;
        }
    }
}
void llamada4(TimerHandle_t xTimers) {
    if(receivedValue <= 255 && pasos == 0){
        receivedValue = receivedValue+1;
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);//verde
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);//verde
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);//verde
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4);//verde
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, receivedValue);//verde
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7);//verde
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);//verde
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5);//verde}
            if(receivedValue == 256){
                pasos = 1;
            }
    }
 else if(receivedValue > 0 && pasos == 1){
    receivedValue = receivedValue - 1;
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);//verde
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);//verde
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);//verde
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4);//verde
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, receivedValue);//verde
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7);//verde
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);//verde
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5);//verde
            if(receivedValue == 0){
                pasos = 0;
            }
 }
}
void llamada301(TimerHandle_t x_Timers){
    switch (pasos) {
        case 0:
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);// rojo puerto 4
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, lumenes);// rojo puerto 1
            printf("paso 0\n");           
            break;

        case 1:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);// rojo puerto 1
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, lumenes);// rojo puerto 2
            printf("paso 1\n");
            break;

        case 2:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, 0);// rojo puerto 1
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, lumenes);// rojo puerto 3
            printf("paso 2\n");
            break;

        case 3:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, 0);// rojo puerto 1
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, lumenes);// rojo puerto 4
            printf("paso 3\n");
            break;

        case 4:
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, 0);// rojo puerto 4
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, lumenes);// rojo puerto 5
            pasos = -1;
            printf("paso 4\n");
            break;
        default:
            break;
    }
    pasos += 1;
}
void llamada302(){
    
}
void llamada304(){
    
}

void llamada305(TimerHandle_t xTimers) {
    
   if (pasos == 0){
         printf("subimos\n");
        lumenes = lumenes + 5;
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, lumenes);// rojo puerto 1
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, lumenes);// rojo puerto 2
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, lumenes);// rojo puerto 3
        PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, lumenes);// rojo puerto 4
        PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, lumenes);// rojo puerto 4
        if (lumenes == 255){
            pasos = 1;
            }
        }
   else if (pasos == 1){
        printf("bajamos\n");
        lumenes = lumenes - 5;
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, lumenes);// rojo puerto 1
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, lumenes);// rojo puerto 2
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, lumenes);// rojo puerto 3
        PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, lumenes);// rojo puerto 4
        PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, lumenes);// rojo puerto 4
        if (lumenes == 0){
            pasos = 0;
        }
   }
}
void llamada306(TimerHandle_t x_Timers){
        switch (pasos) {
        case 0:
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, 0);// verde puerto 4
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, lumenes);// verde puerto 1
            printf("paso 0\n");           
            break;

        case 1:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0);// verde puerto 1
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, lumenes);// verde puerto 2
            printf("paso 1\n");
            break;

        case 2:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, 0);// verde puerto 1
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, lumenes);// verde puerto 3
            printf("paso 2\n");
            break;

        case 3:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, 0);// verde puerto 1
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, lumenes);// verde puerto 4
            printf("paso 3\n");
            break;

        case 4:
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0);// verde puerto 4
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, lumenes);// verde puerto 5
            pasos = -1;
            printf("paso 4\n");
            break;
        default:
            break;
    }
    pasos += 1;
}
void llamada307(TimerHandle_t xTimers) {
    switch (pasos) {
        case 0:
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);// rojo puerto 4
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, lumenes);// rojo puerto 1
            printf("paso 0\n");           
            break;

        case 1:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);// rojo puerto 1
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, lumenes);// rojo puerto 2
            printf("paso 1\n");
            break;

        case 2:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, 0);// rojo puerto 1
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, lumenes);// rojo puerto 3
            printf("paso 2\n");
            break;

        case 3:
            PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, 0);// rojo puerto 1
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, lumenes);// rojo puerto 4
            printf("paso 3\n");
            break;

        case 4:
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, 0);// rojo puerto 4
            PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, lumenes);// rojo puerto 5
            pasos = -1;
            printf("paso 4\n");
            break;
        default:
            break;
    }
    pasos += 1;

}
void llamada308(TimerHandle_t xTimers) {
    
   if (pasos == 0){
         printf("subimos\n");
        lumenes = lumenes + 5;
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, lumenes);// rojo puerto 1
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, lumenes);// rojo puerto 2
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, lumenes);// rojo puerto 3
        PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, lumenes);// rojo puerto 4
        PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, lumenes);// rojo puerto 4
        if (lumenes == 255){
            pasos = 1;
            }
        }
   else if (pasos == 1){
        printf("bajamos\n");
        lumenes = lumenes - 5;
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, lumenes);// rojo puerto 1
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, lumenes);// rojo puerto 2
        PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, lumenes);// rojo puerto 3
        PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, lumenes);// rojo puerto 4
        PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, lumenes);// rojo puerto 4
        if (lumenes == 0){
            pasos = 0;
        }
   }
}



void llamade1(TimerHandle_t xTimers){}
void llamade2(TimerHandle_t xTimers) {

     receivedValue = 225;
   if (pasos == 0){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    pasos = 1; }
    else if(pasos == 1){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
    pasos = 2; }
    else if(pasos == 2){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
    pasos = 3; }
    else if(pasos == 3){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    pasos = 4; }
    else if(pasos == 4){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
    pasos = 5; }
    else if(pasos == 5){
    receivedValue = 0;
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
    pasos = 6; }
    else if(pasos == 6){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
    pasos = 7; }
    else if(pasos == 7){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    pasos = 8; }
    else if(pasos == 8){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
    pasos = 9; }
    else if(pasos == 9){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
    pasos = 10; }
    else if(pasos == 10){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    pasos = 11; }
        else if(pasos == 11){
    receivedValue = 0;
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);

    pasos = 12; }
    else if(pasos == 12){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    pasos = 13; }
    else if(pasos == 13){ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);
    pasos = 14; }
    else if(pasos == 14){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
    pasos = 15; }
    else if(pasos == 15){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
    pasos = 16; }
    else if(pasos == 16){ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    pasos = 17; }
        else if(pasos == 17){
    receivedValue = 0;
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);

    pasos = 0; }
}
void llamade3(TimerHandle_t xTimers){}
void llamade307(TimerHandle_t xTimers){}
void llamade308(TimerHandle_t xTimers){}


esp_err_t set_timer(int escena1, TickType_t MyInterval){
        TimerCallback function;

    switch (escena1) {
            case 1:
                function = llamada1;   
                break;
            case 301:
                function = llamada301;   
                break;
            
            case 305:
                function = llamada305;   
                break;
            case 306:
                function = llamada306;   
                break;
            case 307:
                function = llamada307;   
                break;
            case 308:
                function = llamada308;   
                break;
                default:
                return ESP_ERR_INVALID_ARG;
                break;

    }

 

    xTimers = xTimerCreate("Timer",       // Just a text name, not used by the kernel.
                          (pdMS_TO_TICKS(MyInterval)),   // The timer period in ticks.
                           pdTRUE,        // The timers will auto-reload themselves when they expire.
                           ( void * )timerId,  // Assign each timer a unique id equal to its array index.
                            function // Each timer calls the same callback when it expires.
                                   );

          if( xTimers == NULL )
          {
              // The timer was not created.
          }
          else
          {
            xTimerStart(xTimers, 0);
          }
    return ESP_OK;
 }


esp_err_t set_timer1(int escena2, TickType_t MyInterval1){
        TimerCallback function;

    switch (escena2) {
            case 1:
                function = llamade1;   
                break;
            case 2:
                function = llamade2;   
                break;
            case 307:
                function = llamade307;   
                break;
            case 308:
                function = llamade308;   
                break;
                default:
                return ESP_ERR_INVALID_ARG;
                break;

    }

    x1Timers = xTimerCreate("Timer",       // Just a text name, not used by the kernel.
                          (pdMS_TO_TICKS(MyInterval1)),   // The timer period in ticks.
                           pdTRUE,        // The timers will auto-reload themselves when they expire.
                           ( void * )timerId1,  // Assign each timer a unique id equal to its array index.
                            function // Each timer calls the same callback when it expires.
                                   );

          if( x1Timers == NULL )
          {
              // The timer was not created.
          }
          else
          {
            xTimerStart(x1Timers, 0);

          }
    return ESP_OK;
 }


esp_err_t set_pwm(void){
// Configurar el temporizador para high LEDC
ledc_timer_config_t ledc_timerH = {
        .duty_resolution = LEDC_TIMER_8_BIT, // Resolución de 8 bits
        .freq_hz = 5000, // Frecuencia de 5000 Hz
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timerH);

// Configurar el temporizador para low LEDC
    ledc_timer_config_t ledc_timerL = {
        .duty_resolution = LEDC_TIMER_8_BIT, // Resolución de 8 bits
        .freq_hz = 5000, // Frecuencia de 5000 Hz
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_1
    };
    ledc_timer_config(&ledc_timerL);

 // color rojo canal 1
    ledc_channel_config_t channelconfigR0 = {
        .gpio_num = 2,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigR0);

 // color verde canal 1
    ledc_channel_config_t channelconfigG0 = {
        .gpio_num = 4,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigG0);

 // color azul canal 1
    ledc_channel_config_t channelconfigB0 = {
        .gpio_num = 16,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigB0);

// color rojo canal 2
    ledc_channel_config_t channelconfigR1 = {
        .gpio_num = 17,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_3,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigR1);

 // color verde canal 2
    ledc_channel_config_t channelconfigG1 = {
        .gpio_num = 5,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_4,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigG1);

// color azul canal 2
    ledc_channel_config_t channelconfigB1 = {
        .gpio_num = 23,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_5,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigB1);

// color rojo canal 3
    ledc_channel_config_t channelconfigR2 = {
        .gpio_num = 22,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_6,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigR2);

// color verde canal 3
     ledc_channel_config_t channelconfigG2 = {
        .gpio_num = 19,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_7,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigG2);

// color azul canal 3
    ledc_channel_config_t channelconfigB2 = {
        .gpio_num = 18,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigB2);

// color rojo canal 4
    ledc_channel_config_t channelconfigR3 = {
        .gpio_num = 32,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigR3);

// color verde canal 4
    ledc_channel_config_t channelconfigG3 = {
        .gpio_num = 33,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigG3);

// color azul canal 4
    ledc_channel_config_t channelconfigB3 = {
        .gpio_num = 25,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_3,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigB3);

// color rojo canal 5
ledc_channel_config_t channelconfigR4 = {
        .gpio_num = 26,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_4,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigR4);

// color verde canal 5
    ledc_channel_config_t channelconfigG4 = {
        .gpio_num = 14,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_5,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigG4);

// color azul canal 5
    ledc_channel_config_t channelconfigB4 = {
        .gpio_num = 10,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_6,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
    };
    ledc_channel_config(&channelconfigB4);

    return ESP_OK;
}


static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
    char bda_str[18] = {0};

    switch (event) {
    case ESP_SPP_INIT_EVT:
        if (param->init.status == ESP_SPP_SUCCESS) {
            ESP_LOGI(SPP_TAG, "ESP_SPP_INIT_EVT");
            esp_spp_start_srv(sec_mask, role_slave, 0, SPP_SERVER_NAME);
        } else {
            ESP_LOGE(SPP_TAG, "ESP_SPP_INIT_EVT status:%d", param->init.status);
        }
        break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_DISCOVERY_COMP_EVT");
        break;
    case ESP_SPP_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_OPEN_EVT");
        break;
    case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT status:%d handle:%"PRIu32" close_by_remote:%d", param->close.status,
                 param->close.handle, param->close.async);
        break;
    case ESP_SPP_START_EVT:
        if (param->start.status == ESP_SPP_SUCCESS) {
            ESP_LOGI(SPP_TAG, "ESP_SPP_START_EVT handle:%"PRIu32" sec_id:%d scn:%d", param->start.handle, param->start.sec_id,
                     param->start.scn);
            esp_bt_dev_set_device_name(EXAMPLE_DEVICE_NAME);
            esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
        } else {
            ESP_LOGE(SPP_TAG, "ESP_SPP_START_EVT status:%d", param->start.status);
        }
        break;
    case ESP_SPP_CL_INIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CL_INIT_EVT");
        break;
    case ESP_SPP_DATA_IND_EVT: // entra evento por bt
        int i = 0;// variable para convertir array
        if (i < param->data_ind.len) { //convierto arrays de 2 bits en unnumero entero
            if (param->data_ind.len - i >= 2) {
            // Reconstruir número de 2 bytes
                receivedValue = (param->data_ind.data[i+1] << 8) | param->data_ind.data[i];
                i += 2; // Incrementar el índice en 2 para saltar al siguiente par de bytes
            } else {
                    // Número de 1 byte
                     receivedValue = param->data_ind.data[i];
                    i++; // Incrementar el índice en 1 para saltar al siguiente byte
                }
        }
        if(receivedValue < 256){
            if(juego == 1){
                    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);//rojo
                    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);//rojo
                    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);//rojo
                    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);//rojo
                    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);//rojo
                    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_6);//rojo
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);//rojo
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);//rojo
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);//rojo
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);//rojo
                    juego = 10;
                    printf("%d", receivedValue);
                    printf("rojo\n");
            }
            else if(juego == 2){
                    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, receivedValue);//verde
                    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);//verde
                    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, receivedValue);//verde
                    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4);//verde
                    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, receivedValue);//verde
                    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7);//verde
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, receivedValue);
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);//verde
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, receivedValue);
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5);//verde
                    juego = 20;
                    printf("%d", receivedValue  );
                    printf("verde\n");
            }
            else if(juego == 3){
                    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, receivedValue);//azul
                    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);//azul
                    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5, receivedValue);//azul
                    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5);//azul
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, receivedValue);//azul
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);//azul
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, receivedValue);//azul
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3);//azul
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6, receivedValue);//azul
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6);//azul
                    printf("%d", receivedValue  );
                    printf("azul\n");
                    juego = 1;
            }
            if(juego == 10){juego = 2;}
            if(juego == 20){juego = 3;}
        }      
        else if(receivedValue < 400){
        ApagarTimer();
        inicioescena();
        switch (receivedValue) {
            case 301:
                PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, 180);
                PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5, 180);
                PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 180);
                PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, 180);
                PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_6, 180);
                myInterval = 50;
                llamado = 301;
                intervalo = 200;
                modificointervalo =  100;
                pasos = 0;
                lumenes = 255;
                set_timer(301, myInterval);
                break;

            case 302:
                llamada302();
                break;

            case 304:
                PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 180);
                PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, 180);
                PuertoSalida(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_7, 180);
                PuertoSalida(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 180);
                printf("entramos al 304\n");
                break;

            case 305:
                myInterval = 200;
                llamado = 306;
                intervalo = 200;
                modificointervalo =  100;
                pasos = 0;
                lumenes = 255;
                set_timer(305, myInterval);
                break;

            case 306:
                myInterval = 50;
                llamado = 306;
                intervalo = 200;
                modificointervalo =  100;
                pasos = 0;
                lumenes = 255;
                set_timer(306, myInterval);
                break;

            case 307:
                myInterval = 200;
                llamado = 307;
                intervalo = 200;
                modificointervalo =  100;
                pasos = 0;
                lumenes = 255;
                set_timer(307, myInterval);
                break;

            case 308:
                myInterval = 50;
                llamado = 308;
                intervalo = 200;
                modificointervalo =  100;
                pasos = 0;
                lumenes = 0;
                set_timer(308, myInterval);
                break;
            case 309:
                break;
            default:
                break;
        };
        }else{
        ApagarTimer();
        switch (receivedValue) {
            case 401:
                intervalo = intervalo + modificointervalo;
                set_timer(llamado, intervalo);
                printf("%d", intervalo);
                printf("%d", llamado);
                printf("\n");
                break; 

            case 402:
                if(intervalo > 100){
                intervalo = intervalo - modificointervalo;
                }else if(intervalo > 10){
                    modificointervalo = 10;
                    intervalo = intervalo - modificointervalo;
                }
                set_timer(llamado, intervalo);
                printf("%d", intervalo);
                printf("%d", llamado);
                printf("\n");
                break;
            default:
                break;
        }

        }
       
        break;
    case ESP_SPP_CONG_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CONG_EVT");
        break;
    case ESP_SPP_WRITE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_WRITE_EVT");
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_OPEN_EVT status:%d handle:%"PRIu32", rem_bda:[%s]", param->srv_open.status,
                 param->srv_open.handle, bda2str(param->srv_open.rem_bda, bda_str, sizeof(bda_str)));
        gettimeofday(&time_old, NULL);
        break;
    case ESP_SPP_SRV_STOP_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_STOP_EVT");
        break;
    case ESP_SPP_UNINIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_UNINIT_EVT");
        break;
    default:
        break;
    }
}


void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param){
    char bda_str[18] = {0};

    switch (event) {
    case ESP_BT_GAP_AUTH_CMPL_EVT:{
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
            ESP_LOGI(SPP_TAG, "authentication success: %s bda:[%s]", param->auth_cmpl.device_name,
                     bda2str(param->auth_cmpl.bda, bda_str, sizeof(bda_str)));
        } else {
            ESP_LOGE(SPP_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }
    case ESP_BT_GAP_PIN_REQ_EVT:{
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
        if (param->pin_req.min_16_digit) {
            ESP_LOGI(SPP_TAG, "Input pin code: 0000 0000 0000 0000");
            esp_bt_pin_code_t pin_code = {0};
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
        } else {
            ESP_LOGI(SPP_TAG, "Input pin code: 1234");
            esp_bt_pin_code_t pin_code;
            pin_code[0] = '1';
            pin_code[1] = '2';
            pin_code[2] = '3';
            pin_code[3] = '4';
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
        }
        break;
    }

#if (CONFIG_BT_SSP_ENABLED == true)
    case ESP_BT_GAP_CFM_REQ_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %"PRIu32, param->cfm_req.num_val);
        esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
        break;
    case ESP_BT_GAP_KEY_NOTIF_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%"PRIu32, param->key_notif.passkey);
        break;
    case ESP_BT_GAP_KEY_REQ_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        break;
#endif

    case ESP_BT_GAP_MODE_CHG_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_MODE_CHG_EVT mode:%d bda:[%s]", param->mode_chg.mode,
                 bda2str(param->mode_chg.bda, bda_str, sizeof(bda_str)));
        break;

    default: {
        ESP_LOGI(SPP_TAG, "event: %d", event);
        break;
    }
    }
    return;
}



void app_main(void){
    set_pwm();
    char bda_str[18] = {0};
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
     esp_bluedroid_config_t cfg = {
        // Configura aquí los parámetros de la estructura de configuración según sea necesario
        .ssp_en = true,
        // Otros parámetros...
    };

    // Llama a la función esp_bluedroid_init_with_cfg con la estructura de configuración

    if ((ret = esp_bluedroid_init_with_cfg(&cfg))) {
        ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }































































































    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s gap register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    esp_spp_cfg_t bt_spp_cfg = {
        .mode = esp_spp_mode,
        .enable_l2cap_ertm = esp_spp_enable_l2cap_ertm,
        .tx_buffer_size = 0, /* Only used for ESP_SPP_MODE_VFS mode */
    };
    if ((ret = esp_spp_enhanced_init(&bt_spp_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp init failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

#if (CONFIG_BT_SSP_ENABLED == true)
    /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
#endif

    /*
     * Set default parameters for Legacy Pairing
     * Use variable pin, input pin code when pairing
     */
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
    esp_bt_pin_code_t pin_code;
    esp_bt_gap_set_pin(pin_type, 0, pin_code);

    ESP_LOGI(SPP_TAG, "Own address:[%s]", bda2str((uint8_t *)esp_bt_dev_get_address(), bda_str, sizeof(bda_str)));
}
