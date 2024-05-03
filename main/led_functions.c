#include "led_functions.h"

#define RMT_LED_STRIP_RESOLUTION_HZ (10000000) // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM            (14)
#define EXAMPLE_LED_NUMBERS               (15)
#define EXAMPLE_CHASE_SPEED_MS           (100)

uint32_t red = 0;
uint32_t green = 0;
uint32_t blue = 0;
uint16_t hue = 0;
uint16_t start_rgb = 0;


rmt_channel_handle_t led_chan = NULL;
rmt_tx_channel_config_t tx_chan_config = {
    .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
    .gpio_num = RMT_LED_STRIP_GPIO_NUM,
    .mem_block_symbols = 64, // increase the block size can make the LED less flickering
    .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
    .trans_queue_depth = 4, // set the number of transactions that can be pending in the background
};

rmt_encoder_handle_t led_encoder = NULL;
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };

rmt_transmit_config_t tx_config = {
        .loop_count = 0, // no transfer loop
    };

static const char *TURN_ON_OFF_LEDSTRIP = "TURN ON/OFF LEDSTRIP";
static const char *CHANGE_COLOR = "CHANGE COLOR";
static const char *TAG = "TAG";


static uint8_t led_strip_pixels[EXAMPLE_LED_NUMBERS * 3];

uint8_t cor_atual;

void ledstrip_init()
{
    ESP_LOGI(TAG, "Create RMT TX channel");
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));
    ESP_LOGI(TAG, "Install led strip encoder");
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));
    ESP_LOGI(TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(led_chan));
    ESP_LOGI(TAG, "Start LED rainbow chase");
}

void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

void turn_on_off_ledstrip(uint8_t status)
{


    if(status==TURN_ON)
    {
        hue = cor_atual;

        led_strip_hsv2rgb(hue, SATURACAO, INTENSIDADE, &red, &green, &blue);
        
        for (int j = 0; j < EXAMPLE_LED_NUMBERS; j++) 
        {
            led_strip_pixels[j * 3 + 0] = green;
            led_strip_pixels[j * 3 + 1] = red;
            led_strip_pixels[j * 3 + 2] = blue;
        }
        // Flush RGB values to LEDs
        ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
        vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
    }
    else
    {
        memset(led_strip_pixels, 0, sizeof(led_strip_pixels));
        ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
        vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
    }
}


uint8_t change_color(uint8_t color)
{

    hue = color;
    cor_atual = color;

    led_strip_hsv2rgb(hue, SATURACAO, INTENSIDADE, &red, &green, &blue);

    for (int j = 0; j < EXAMPLE_LED_NUMBERS; j++) 
    {
        led_strip_pixels[j * 3 + 0] = green;
        led_strip_pixels[j * 3 + 1] = red;
        led_strip_pixels[j * 3 + 2] = blue;
    }
    // Flush RGB values to LEDs
    ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
    return cor_atual;
}


