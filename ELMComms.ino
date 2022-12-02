#include "BluetoothSerial.h"
#include "ELMduino.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

BluetoothSerial SerialBT;
#define ELM_PORT     SerialBT
#define DEBUG_PORT Serial
#define BUTTON_PIN 21 // GIOP21 pin connected to button

class Display {
private:
    /* Declare LCD object for SPI
    Adafruit_PCD8544(CLK,DIN,D/C,CE,RST); */
    Adafruit_PCD8544 display = Adafruit_PCD8544(18, 23, 4, 15, 2);
    int contrastValue;
public:
    Display() {
        contrastValue = 60;
        display.begin();
        display.setContrast(contrastValue);
        clear_display();
        // delay(1000);
    }

    void display_text(String text, int height) {
        display.setTextSize(1);
        display.setTextColor(BLACK);
        int position_width = (84 - 6 * text.length()) / 2;
        display.setCursor(position_width, height);
        display.println(text);
        display.display();
    }

    void clear_display()
    {
        this->display.clearDisplay();
        this->display.display();
    }

    void clear_buffer() {
        this->display.clearDisplay();
    }
};

class Button {
private:
    int lastState;
    int currentState;
public:
    Button() {
        lastState = HIGH;   
    }

    bool isButtonClicked() {
        currentState = digitalRead(BUTTON_PIN);
        if(lastState == LOW && currentState == HIGH) {
            lastState = currentState;
            return true;
        }
        lastState = currentState;
        return false;
    }
};

Display display;

Button button;

ELM327 myELM327;

bool available_pids[128] = {false}; //maybe 102?

int all_available_non_null_pids[101] = {};

int available_id_size = 0;

double val;
String val_string;

typedef void (*voidFncPtr)();// Create a type to point to a function.
voidFncPtr    CallBackFunctions[102] = {
NULL,    //0
NULL,    //1
NULL,    //2
NULL,    //3
[]() {    //4
    val = myELM327.engineLoad();
    val_string = "engineLoad";
},
[]() {    //5
    val = myELM327.engineCoolantTemp();
    val_string = "engineCoolantTemp";
},
[]() {    //6
    val = myELM327.shortTermFuelTrimBank_1();
    val_string = "shortTermFuelTrimBank_1";
},
[]() {    //7
    val = myELM327.longTermFuelTrimBank_1();
    val_string = "longTermFuelTrimBank_1";
},
[]() {    //8
    val = myELM327.shortTermFuelTrimBank_2();
    val_string = "shortTermFuelTrimBank_2";
},
[]() {    //9
    val = myELM327.longTermFuelTrimBank_2();
    val_string = "longTermFuelTrimBank_2";
},
[]() {    //10
    val = myELM327.fuelPressure();
    val_string = "fuelPressure";
},
[]() {    //11
    val = myELM327.manifoldPressure();
    val_string = "manifoldPressure";
},
[]() {    //12
    val = myELM327.rpm();
    val_string = "rpm";
},
[]() {    //13
    val = myELM327.kph();
    val_string = "kph";
},
[]() {    //14
    val = myELM327.timingAdvance();
    val_string = "timingAdvance";
},
[]() {    //15
    val = myELM327.intakeAirTemp();
    val_string = "intakeAirTemp";
},
[]() {    //16
    val = myELM327.mafRate();
    val_string = "mafRate";
},
[]() {    //17
    val = myELM327.throttle();
    val_string = "throttle";
},
[]() {    //18
    val = myELM327.commandedSecAirStatus();
    val_string = "commandedSecAirStatus";
},
NULL,    //19
NULL,    //20
NULL,    //21
NULL,    //22
NULL,    //23
NULL,    //24
NULL,    //25
NULL,    //26
NULL,    //27
[]() {    //28
    val = myELM327.obdStandards();
    val_string = "obdStandards";
},
NULL,    //29
[]() {    //30
    val = myELM327.auxInputStatus();
    val_string = "auxInputStatus";
},
[]() {    //31
    val = myELM327.runTime();
    val_string = "runTime";
},
[]() {    //32
    val = myELM327.supportedPIDs_21_40();
    val_string = "supportedPIDs_21_40";
},
[]() {    //33
    val = myELM327.distTravelWithMIL();
    val_string = "distTravelWithMIL";
},
[]() {    //34
    val = myELM327.fuelRailPressure();
    val_string = "fuelRailPressure";
},
[]() {    //35
    val = myELM327.fuelRailGuagePressure();
    val_string = "fuelRailGuagePressure";
},
NULL,    //36
NULL,    //37
NULL,    //38
NULL,    //39
NULL,    //40
NULL,    //41
NULL,    //42
NULL,    //43
[]() {    //44
    val = myELM327.commandedEGR();
    val_string = "commandedEGR";
},
[]() {    //45
    val = myELM327.egrError();
    val_string = "egrError";
},
[]() {    //46
    val = myELM327.commandedEvapPurge();
    val_string = "commandedEvapPurge";
},
[]() {    //47
    val = myELM327.fuelLevel();
    val_string = "fuelLevel";
},
[]() {    //48
    val = myELM327.warmUpsSinceCodesCleared();
    val_string = "warmUpsSinceCodesCleared";
},
[]() {    //49
    val = myELM327.distSinceCodesCleared();
    val_string = "distSinceCodesCleared";
},
[]() {    //50
    val = myELM327.evapSysVapPressure();
    val_string = "evapSysVapPressure";
},
[]() {    //51
    val = myELM327.absBaroPressure();
    val_string = "absBaroPressure";
},
NULL,    //52
NULL,    //53
NULL,    //54
NULL,    //55
NULL,    //56
NULL,    //57
NULL,    //58
NULL,    //59
[]() {    //60
    val = myELM327.catTempB1S1();
    val_string = "catTempB1S1";
},
[]() {    //61
    val = myELM327.catTempB2S1();
    val_string = "catTempB2S1";
},
[]() {    //62
    val = myELM327.catTempB1S2();
    val_string = "catTempB1S2";
},
[]() {    //63
    val = myELM327.catTempB2S2();
    val_string = "catTempB2S2";
},
[]() {    //64
    val = myELM327.supportedPIDs_41_60();
    val_string = "supportedPIDs_41_60";
},
[]() {    //65
    val = myELM327.monitorDriveCycleStatus();
    val_string = "monitorDriveCycleStatus";
},
[]() {    //66
    val = myELM327.ctrlModVoltage();
    val_string = "ctrlModVoltage";
},
[]() {    //67
    val = myELM327.absLoad();
    val_string = "absLoad";
},
[]() {    //68
    val = myELM327.commandedAirFuelRatio();
    val_string = "commandedAirFuelRatio";
},
[]() {    //69
    val = myELM327.relativeThrottle();
    val_string = "relativeThrottle";
},
[]() {    //70
    val = myELM327.ambientAirTemp();
    val_string = "ambientAirTemp";
},
[]() {    //71
    val = myELM327.absThrottlePosB();
    val_string = "absThrottlePosB";
},
[]() {    //72
    val = myELM327.absThrottlePosC();
    val_string = "absThrottlePosC";
},
[]() {    //73
    val = myELM327.absThrottlePosD();
    val_string = "absThrottlePosD";
},
[]() {    //74
    val = myELM327.absThrottlePosE();
    val_string = "absThrottlePosE";
},
[]() {    //75
    val = myELM327.absThrottlePosF();
    val_string = "absThrottlePosF";
},
[]() {    //76
    val = myELM327.commandedThrottleActuator();
    val_string = "commandedThrottleActuator";
},
[]() {    //77
    val = myELM327.timeRunWithMIL();
    val_string = "timeRunWithMIL";
},
[]() {    //78
    val = myELM327.timeSinceCodesCleared();
    val_string = "timeSinceCodesCleared";
},
NULL,    //79
[]() {    //80
    val = myELM327.maxMafRate();
    val_string = "maxMafRate";
},
[]() {    //81
    val = myELM327.fuelType();
    val_string = "fuelType";
},
[]() {    //82
    val = myELM327.ethonolPercent();
    val_string = "ethonolPercent";
},
[]() {    //83
    val = myELM327.absEvapSysVapPressure();
    val_string = "absEvapSysVapPressure";
},
[]() {    //84
    val = myELM327.evapSysVapPressure2();
    val_string = "evapSysVapPressure2";
},
NULL,    //85
NULL,    //86
NULL,    //87
NULL,    //88
[]() {    //89
    val = myELM327.absFuelRailPressure();
    val_string = "absFuelRailPressure";
},
[]() {    //90
    val = myELM327.relativePedalPos();
    val_string = "relativePedalPos";
},
[]() {    //91
    val = myELM327.hybridBatLife();
    val_string = "hybridBatLife";
},
[]() {    //92
    val = myELM327.oilTemp();
    val_string = "oilTemp";
},
[]() {    //93
    val = myELM327.fuelInjectTiming();
    val_string = "fuelInjectTiming";
},
[]() {    //94
    val = myELM327.fuelRate();
    val_string = "fuelRate";
},
[]() {    //95
    val = myELM327.emissionRqmts();
    val_string = "emissionRqmts";
},
[]() {    //96
    val = myELM327.supportedPIDs_61_80();
    val_string = "supportedPIDs_61_80";
},
[]() {    //97
    val = myELM327.demandedTorque();
    val_string = "demandedTorque";
},
[]() {    //98
    val = myELM327.torque();
    val_string = "torque";
},
[]() {    //99
    val = myELM327.referenceTorque();
    val_string = "referenceTorque";
},
NULL,    //100
[]() {    //101
    val = myELM327.auxSupported();
    val_string = "auxSupported";
},
};

bool is_bit_set(uint32_t value, uint8_t bit_index) {
    return (value & (1 << bit_index)) ? true : false;
}

bool is_error(int8_t state) {
    return state != ELM_GETTING_MSG && state != ELM_SUCCESS;
}

void initialize_pids() {
    Serial.println("Initialize start");
    uint32_t pids_1 = 0;
    do {
        pids_1 = myELM327.supportedPIDs_1_20();
        if (myELM327.nb_rx_state == ELM_SUCCESS) {
            break;
        } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
            break;
            myELM327.printError();    
        }
    }
    while (!is_error(myELM327.nb_rx_state));

    uint32_t pids_2 = 0;
    do {
        pids_2 = myELM327.supportedPIDs_21_40();
        if (myELM327.nb_rx_state == ELM_SUCCESS) {
            break;
        } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
            break;
            myELM327.printError();    
        }
    }
    while (!is_error(myELM327.nb_rx_state));

    uint32_t pids_3 = 0;
    do {
        pids_3 = myELM327.supportedPIDs_41_60();
        if (myELM327.nb_rx_state == ELM_SUCCESS) {
            break;
        } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
            break;
            myELM327.printError();    
        }
    }
    while (!is_error(myELM327.nb_rx_state));

    uint32_t pids_4 = 0;
    do {
        pids_4 = myELM327.supportedPIDs_61_80();
        if (myELM327.nb_rx_state == ELM_SUCCESS) {
            break;
        } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
            break;
            myELM327.printError();    
        }
    }
    while (!is_error(myELM327.nb_rx_state));


    int i = 0;
    for (int j = 31; j >= 0; j--) {
        if (bitRead(pids_1,j) == 1) {
            available_pids[i] = true;
        }        
        i++;
    }
//    for (int j = 0; j < 31; i++) {
//        if(is_bit_set(pids_2,j)) {
//            available_pids[i] = true;
//        }
//        i++;
//    }
//    for (int j = 0; j < 31; i++) {
//        if(is_bit_set(pids_3,j)) {
//            available_pids[i] = true;
//        }
//        i++;
//    }
//    for (int j = 0; j < 31; i++) {
//        if(is_bit_set(pids_4,j)) {
//            available_pids[i] = true;
//        }
//        i++;
//    }

    for (int j = 0; j < 31; j++)
    {
        if (available_pids[j] && (CallBackFunctions[j] != NULL)) {
            all_available_non_null_pids[available_id_size] = j;
            available_id_size++;
        }
    }
}

void readValue(double val, String val_string) {
    display.clear_buffer();
    if (myELM327.nb_rx_state == ELM_SUCCESS) {
        display.display_text(val_string,12); 
        display.display_text(String(val), 20);
    } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
        myELM327.printError();    
    }
}

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    display.display_text("|Connecting|", 20);
#if LED_BUILTIN
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
#endif

    DEBUG_PORT.begin(1200);
    SerialBT.setPin("1234");
    ELM_PORT.begin("ArduHUD", true);

    if (!ELM_PORT.connect("vLinker MC-Android"))
    {
        DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
        display.clear_display();
        display.display_text("|FAILED 1|" , 20);
        while (1);
    }

    if (!myELM327.begin(ELM_PORT, false, 5000, '0'))
    {
        Serial.println("Couldn't connect to OBD scanner - Phase 2");
        display.clear_display();
        display.display_text("|Failed 2|", 20);
        while (1);
    }

    Serial.println("Connected to ELM327");
    display.clear_display();
    display.display_text("|Connected|", 20);
    initialize_pids();
}

int counter = 0;
void loop()
{
    CallBackFunctions[all_available_non_null_pids[counter]]();
    readValue(val, val_string);

    bool isButtonClicked = button.isButtonClicked();
    if(isButtonClicked)
    {
        if (counter == available_id_size - 1) {
            counter = 0;
        }
        counter++;
        delay(250);
    }
}
