#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"  
#include "esp_err.h"  

// Configuración modular
#define CONFIG_LED_GPIO_PIN         2
#define CONFIG_BLINK_PERIOD_MS      500
#define CONFIG_TASK_STACK_SIZE      3072
#define CONFIG_TASK_PRIORITY        5
#define CONFIG_BUTTON_GPIO_PIN      0  


static const char* TAG = "LED Controller";


static volatile uint8_t g_led_state = 0;
static volatile uint16_t g_blink_period = CONFIG_BLINK_PERIOD_MS;


void initialize_hardware(void);
void led_control_task(void *pvParameter);
void button_monitor_task(void *pvParameter);  

void app_main(void) 
{
    
    ESP_ERROR_CHECK(initialize_hardware());
    
    
    BaseType_t ret = xTaskCreate(
        led_control_task,
        "LED Control",
        CONFIG_TASK_STACK_SIZE,
        NULL,
        CONFIG_TASK_PRIORITY,
        NULL
    );
    
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Error creating LED task!");
        return;
    }

    
}

void initialize_hardware(void) 
{
    // Configuración del LED como se ve se puede poner pull down y pull up , entra en cambio de modo
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << CONFIG_LED_GPIO_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));


    ESP_LOGI(TAG, "Hardware initialized successfully");
}

void led_control_task(void *pvParameter)
{
    ESP_LOGI(TAG, "LED task started. Blink period: %dms", g_blink_period);
    
    while (1) 
    {
        g_led_state = !g_led_state;
        
        
        ESP_ERROR_CHECK(gpio_set_level(CONFIG_LED_GPIO_PIN, g_led_state));
        
        
        ESP_LOGD(TAG, "LED state changed to: %s", g_led_state ? "ON" : "OFF");
        
        
        vTaskDelay(pdMS_TO_TICKS(g_blink_period));
    }
}


void button_monitor_task(void *pvParameter)
{
    ESP_LOGI(TAG, "Button monitor task started");
    
    while (1) 
    {
        
        if (!gpio_get_level(CONFIG_BUTTON_GPIO_PIN)) 
        {
            g_blink_period = (g_blink_period == 500) ? 1000 : 500;
            ESP_LOGI(TAG, "Button pressed. New blink period: %dms", g_blink_period);
            
            
            vTaskDelay(pdMS_TO_TICKS(300));
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}