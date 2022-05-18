#include "OBD2.h"

uint8_t idList[17] = {0x01, 0x03, 0x04, 0x05, 0x07, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x11, 0x14, 0x21, 0x33, 0x42, 0x45, 0x4d };
uint8_t idIndex = 0;
uint32_t time_received = 0;


FlexCAN_T4<CAN, RX_SIZE_256, TX_SIZE_16> myCan;
OBD2sensordata *_db;


void initOBD2(OBD2sensordata &database){
    // passed by reference, so other parts of the  code can access
    _db = &database;


    myCan.begin();
    myCan.setBaudRate(500000);
    myCan.setMaxMB(20);
    myCan.enableFIFO();
    myCan.enableFIFOInterrupt();
    myCan.onReceive(FIFO,receivedOBD2callback);

    // set up FIFO filter (OBD2)
    myCan.setFIFOFilter(REJECT_ALL);
    myCan.setFIFOFilter(0, 0x7E8, STD);
}



void askPID(uint8_t id){ 
    CAN_message_t askPID;
    askPID.id = 0x7DF;

    askPID.buf[0] = 0x02; // message length
    askPID.buf[1] = 0x01; // service 0x01 (current data)
    askPID.buf[2] = id;

    askPID.len = 8;
    myCan.write(askPID);

}
extern uint32_t counter;

void receivedOBD2callback(const CAN_message_t &msg){
    #ifdef DEBUG
 //   Serial.print("Callback reached: ");
    #endif
    uint8_t length = msg.buf[0];
    uint8_t service = msg.buf[1]; // 41: actual data, 43: dtc
    uint8_t pid = msg.buf[2];
    uint8_t A = msg.buf[3];
    uint8_t B = msg.buf[4];

    if (service == 0x41){ // we've got a response
        switch(pid){
            case MONITOR_STATUS_SINCE_DTCS_CLEARED:

                _db->MIL_on =  A & 0b10000000 ;
                _db->DTC_CNT = A & 0b01111111 ;
                break;

            case FUEL_SYSTEM_STATUS:
                _db->Fuel_system_status = A;
                break;

            case CALCULATED_ENGINE_LOAD:
                _db->Calculated_Engine_load = A;
                break;

            case ENGINE_COOLANT_TEMPERATURE:
                _db->Engine_coolant_temperature = A;
                break;

            case LONG_TERM_FUEL_TRIM_BANK_1:
                _db->long_term_fuel_trim = A;
                break;

            case INTAKE_MANIFOLD_ABSOLUTE_PRESSURE:
                _db->intake_manifold_absolute_pressure = A;
                break;

            case ENGINE_RPM:
                _db->engine_rpmA = A;
                _db->engine_rpmB = B;
                break;

            case VEHICLE_SPEED:
                _db->vehicle_speed = A;
                break;

            case TIMING_ADVANCE:
                _db->timing_advance = A;
                break;

            case AIR_INTAKE_TEMPERATURE:
                _db->intake_air_temperature = A;
                break;

            case THROTTLE_POSITION:
                _db->throttle_position = A;
                break;

            case OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM:
                _db->oxygen_sensor_voltage = A;
                _db->oxygen_sensor_long_term_fuel_trim = B;
                break;

            case DISTANCE_TRAVELED_WITH_MIL_ON:
                _db->Distance_Traveled_MIL_on_A = A;
                _db->Distance_Traveled_MIL_on_B = B;
                break;

            case ABSOLULTE_BAROMETRIC_PRESSURE:
                _db->absolute_barometric_presure = A;
                break;

            case CONTROL_MODULE_VOLTAGE:
                _db->control_module_voltage_A = A;
                _db->control_module_voltage_B = B;
                break;

            case RELATIVE_THROTTLE_POSITION:
                _db->relavite_throttle_position = A;
                break;

            case TIME_RUN_WITH_MIL_ON:
                _db->time_run_with_mil_on_A = A;
                _db->time_run_with_mil_on_B = B;
                break;  
        }
    }


    askPID(idList[idIndex]);
    idIndex = (idIndex + 1) % sizeof(idList);
    time_received = millis();
}

void OBD2events(){
    // process can callbacks
    myCan.events();

    // restart asking if not response 
    if (millis()-time_received > 1000){
        #ifdef DEBUG
        Serial.println("Failed to receive response, restarting");
        #endif

        askPID(MONITOR_STATUS_SINCE_DTCS_CLEARED);
        time_received = millis();
    }
}


void printOBD2ALL(OBD2sensordata database){
    Serial.print("MIL_ON: "); Serial.println(database.MIL_on);
    Serial.print("DTC COUNT: "); Serial.println(database.DTC_CNT);
    Serial.println();

    Serial.print("FUEL SYSTEM STATUS: ");
    switch (database.Fuel_system_status){
        case 0:
            Serial.println("motor OFF");
            break;
        case 1:
            Serial.println("Open loop due to insufficient engine temperature");
            break;
        case 2:
            Serial.println("Closed loop, using oxygen sensor feedback to determine fuel mix");
            break;
        case 4:
            Serial.println("Open loop due to engine load OR fuel cut due to deceleration");
            break;
        case 8:
            Serial.println("Open loop due to system failure");
            break;
        case 16:
            Serial.println("Closed loop, using at least one oxygen sensor but there is a fault in the feedback system");
        default:
            Serial.println("Invalid response");
    }
    
    Serial.println();
    
    Serial.print("Calculated Engine Load: "); Serial.print(database.Calculated_Engine_load/2.55, 2); Serial.println("%");

    Serial.println();

    Serial.print("coolant Temp: "); Serial.print(float(database.Engine_coolant_temperature) - 40.0,2); Serial.println("ºC");
    Serial.print("Air Temp: "); Serial.print(float(database.intake_air_temperature) - 40.0,2); Serial.println("ºC");
    
    Serial.println();

    Serial.print("Long term fuel trim: "); Serial.print(database.long_term_fuel_trim/1.28-100, 2); Serial.println("%");
    Serial.print("Intake manifold absolute pressure: "); Serial.print(database.intake_manifold_absolute_pressure); Serial.println("kPa");
    Serial.print("Absolute barometric presure: "); Serial.print(database.absolute_barometric_presure); Serial.println("kPa");

    Serial.println();
    Serial.print("Engine RPM: "); Serial.print((database.engine_rpmA*256+database.engine_rpmB)/4.0,2); Serial.println("rpm");
    Serial.print("Vehicle Speed: "); Serial.print(database.vehicle_speed); Serial.println("km/h");

    Serial.println();

    Serial.print("Timing advance: "); Serial.print(database.timing_advance/2.0-64, 2); Serial.println("º before TDC");
    Serial.print("Throttle position: "); Serial.print(database.throttle_position/2.55, 2); Serial.println("%");
    Serial.print("Relative throttle position: "); Serial.print(database.relavite_throttle_position/2.55, 2); Serial.println("%");

    Serial.println();


    Serial.print("Oxygen sensor voltage: "); Serial.print(database.oxygen_sensor_voltage/200.0, 2); Serial.println("V");
    Serial.print("Oxygen sensor long term fuel trim: "); Serial.print(database.oxygen_sensor_long_term_fuel_trim/1.28 - 100, 2); Serial.println("%");

    Serial.println();

    Serial.print("Control module voltage: "); Serial.print((256*database.control_module_voltage_A + database.control_module_voltage_B) / 1000.0, 2); Serial.println("V");
    Serial.print("Distance Traveled MIL: "); Serial.print(256*database.Distance_Traveled_MIL_on_A + database.control_module_voltage_B); Serial.println("km");
    Serial.print("Time Traveled MIL: "); Serial.print(256*database.time_run_with_mil_on_A + database.time_run_with_mil_on_B); Serial.println("minutes");
}
