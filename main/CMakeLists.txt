idf_component_register(SRCS "wifi_connect.c" "firebase.c" "Real-Time-Household-Water-Management-System.c" 
"ultrasonic_control.c" "flow_sensor.c" "relay_control.c" "solenoid_control.c" "temperature_sensor.c" "tds_sensor.c"
                    INCLUDE_DIRS "." 
                    EMBED_TXTFILES "firebase_cert.pem")
