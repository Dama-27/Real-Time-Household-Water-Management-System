#include "firebase.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"

extern const uint8_t firebase_cert_pem_start[] asm("_binary_firebase_cert_pem_start");
extern const uint8_t firebase_cert_pem_end[]   asm("_binary_firebase_cert_pem_end");

static const char *TAG = "FIREBASE";

// Firebase settings (replace these with your project settings)
// #define FIREBASE_HOST "https://check-e1cd8-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define FIREBASE_HOST "https://smart-house-water-management-default-rtdb.firebaseio.com/"

void firebase_send_json(const char *path, cJSON *root) {
    char url[256];
    snprintf(url, sizeof(url), "%s%s.json", FIREBASE_HOST, path);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_PATCH, // Use PUT to update or set data
        .cert_pem = (const char *)firebase_cert_pem_start,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    
    char *data = cJSON_PrintUnformatted(root);  // Convert cJSON object to string
    esp_http_client_set_post_field(client, data, strlen(data));

    esp_err_t err = esp_http_client_perform(client);
    
    if (err == ESP_OK) {
        ESP_LOGI("FIREBASE", "Data sent successfully");
    } else {
        ESP_LOGE("FIREBASE", "Failed to send data: %s", esp_err_to_name(err));
    }

    free(data);
    esp_http_client_cleanup(client);
}

// Function to send multiple values to Firebase
void firebase_send_data( float inflow, float outflow, 
                        int tds, float temperature, int total_in, int total_out, int water_level, 
                        const char* water_quality) {

    // Path is now the root, as no specific path is provided
    char path[] = "/";  // Firebase root path
    cJSON *root = cJSON_CreateObject();

    // Adding all the required fields to the root object
    // cJSON_AddBoolToObject(root, "InValve", in_valve);
    cJSON_AddNumberToObject(root, "Inflow", inflow);
    // cJSON_AddBoolToObject(root, "MotorSwitch", motor_switch);
    // cJSON_AddBoolToObject(root, "OutValve", out_valve);
    cJSON_AddNumberToObject(root, "Outflow", outflow);
    cJSON_AddNumberToObject(root, "TDS", tds);
    cJSON_AddNumberToObject(root, "Temperature", temperature);
    cJSON_AddNumberToObject(root, "TotalIn", total_in);
    cJSON_AddNumberToObject(root, "TotalOut", total_out);
    cJSON_AddNumberToObject(root, "WaterLevel", water_level);
    cJSON_AddStringToObject(root, "WaterQuality", water_quality);
    // cJSON_AddStringToObject(root, "test", test);
    // cJSON_AddStringToObject(root, "toggleState", toggle_state);
    // cJSON_AddNumberToObject(root, "value", value);

    // Call the Firebase send function
    firebase_send_json(path, root);

    cJSON_Delete(root);  // Clean up the JSON object
}

// Function to get the button state from Firebase
bool firebase_get_button_state(const char *button_path) {
    char url[256];
    snprintf(url, sizeof(url), "%s/%s.json", FIREBASE_HOST, button_path);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .cert_pem = (const char *)firebase_cert_pem_start,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return false;
    }

    esp_http_client_fetch_headers(client);

    int content_length = esp_http_client_get_content_length(client);
    if (content_length <= 0) {
        ESP_LOGW(TAG, "No content");
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return false;
    }

    char *response = malloc(content_length + 1);
    if (!response) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return false;
    }

    int data_read = esp_http_client_read_response(client, response, content_length);
    if (data_read >= 0) {
        response[data_read] = '\0';
        ESP_LOGI(TAG, "Read %d bytes: %s", data_read, response);

        if (strcmp(response, "true") == 0) {
            free(response);
            esp_http_client_close(client);
            esp_http_client_cleanup(client);
            return true;
        } else if (strcmp(response, "false") == 0) {
            free(response);
            esp_http_client_close(client);
            esp_http_client_cleanup(client);
            return false;
        }
    }

    free(response);
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    return false;
}