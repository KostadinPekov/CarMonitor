#include "BluetoothSerial.h"
#include "ELMduino.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

BluetoothSerial SerialBT;
#define ELM_PORT     SerialBT
#define DEBUG_PORT Serial
#define BUTTON_PIN 21 // GIOP21 pin connected to button

typedef void (*voidFncPtr)();// Create a type to point to a function.

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
        clearDisplay();
        // delay(1000);
    }

    void displayText(String text, int height) {
        display.setTextSize(1);
        display.setTextColor(BLACK);
        int positionWidth = (84 - 6 * text.length()) / 2;
        display.setCursor(positionWidth, height);
        display.println(text);
        display.display();
    }

    void clearDisplay()
    {
        this->display.clearDisplay();
        this->display.display();
    }

    void clearAndDisplayText(String text, int height) {
      clearDisplay();
      displayText(text, height);
    }

    void clearBuffer() {
        this->display.clearDisplay();
    }
};

class Button {
private:
    int lastState;
    int currentState;
public:
    Button() {
      pinMode(BUTTON_PIN, INPUT_PULLUP);
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

class PidController {
private:
    bool availabilityMap[128] = {false}; //maybe 102?
    int availablePids[101] = {};
    int availableIdSize = 0;
    int currentPid = 0;

    double currentVal;
    String currentValString;
    voidFncPtr CallBackFunctions[102] = {
  NULL,    //0
  NULL,    //1
  NULL,    //2
  NULL,    //3
  [&]() {    //4
    currentVal = myELM327.engineLoad();
    currentValString = "engineLoad";
  },
  [&]() {    //5
    currentVal = myELM327.engineCoolantTemp();
    currentValString = "engineCoolantTemp";
  },
  [&]() {    //6
    currentVal = myELM327.shortTermFuelTrimBank_1();
    currentValString = "shortTermFuelTrimBank_1";
  },
  [&]() {    //7
    currentVal = myELM327.longTermFuelTrimBank_1();
    currentValString = "longTermFuelTrimBank_1";
  },
  [&]() {    //8
    currentVal = myELM327.shortTermFuelTrimBank_2();
    currentValString = "shortTermFuelTrimBank_2";
  },
  [&]() {    //9
    currentVal = myELM327.longTermFuelTrimBank_2();
    currentValString = "longTermFuelTrimBank_2";
  },
  [&]() {    //10
    currentVal = myELM327.fuelPressure();
    currentValString = "fuelPressure";
  },
  [&]() {    //11
    currentVal = myELM327.manifoldPressure();
    currentValString = "manifoldPressure";
  },
  [&]() {    //12
    currentVal = myELM327.rpm();
    currentValString = "rpm";
  },
  [&]() {    //13
    currentVal = myELM327.kph();
    currentValString = "kph";
  },
  [&]() {    //14
    currentVal = myELM327.timingAdvance();
    currentValString = "timingAdvance";
  },
  [&]() {    //15
    currentVal = myELM327.intakeAirTemp();
    currentValString = "intakeAirTemp";
  },
  [&]() {    //16
    currentVal = myELM327.mafRate();
    currentValString = "mafRate";
  },
  [&]() {    //17
    currentVal = myELM327.throttle();
    currentValString = "throttle";
  },
  [&]() {    //18
    currentVal = myELM327.commandedSecAirStatus();
    currentValString = "commandedSecAirStatus";
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
  [&]() {    //28
    currentVal = myELM327.obdStandards();
    currentValString = "obdStandards";
  },
  NULL,    //29
  [&]() {    //30
    currentVal = myELM327.auxInputStatus();
    currentValString = "auxInputStatus";
  },
  [&]() {    //31
    currentVal = myELM327.runTime();
    currentValString = "runTime";
  },
  [&]() {    //32
    currentVal = myELM327.supportedPIDs_21_40();
    currentValString = "supportedPIDs_21_40";
  },
  [&]() {    //33
    currentVal = myELM327.distTravelWithMIL();
    currentValString = "distTravelWithMIL";
  },
  [&]() {    //34
    currentVal = myELM327.fuelRailPressure();
    currentValString = "fuelRailPressure";
  },
  [&]() {    //35
    currentVal = myELM327.fuelRailGuagePressure();
    currentValString = "fuelRailGuagePressure";
  },
  NULL,    //36
  NULL,    //37
  NULL,    //38
  NULL,    //39
  NULL,    //40
  NULL,    //41
  NULL,    //42
  NULL,    //43
  [&]() {    //44
    currentVal = myELM327.commandedEGR();
    currentValString = "commandedEGR";
  },
  [&]() {    //45
    currentVal = myELM327.egrError();
    currentValString = "egrError";
  },
  [&]() {    //46
    currentVal = myELM327.commandedEvapPurge();
    currentValString = "commandedEvapPurge";
  },
  [&]() {    //47
    currentVal = myELM327.fuelLevel();
    currentValString = "fuelLevel";
  },
  [&]() {    //48
    currentVal = myELM327.warmUpsSinceCodesCleared();
    currentValString = "warmUpsSinceCodesCleared";
  },
  [&]() {    //49
    currentVal = myELM327.distSinceCodesCleared();
    currentValString = "distSinceCodesCleared";
  },
  [&]() {    //50
    currentVal = myELM327.evapSysVapPressure();
    currentValString = "evapSysVapPressure";
  },
  [&]() {    //51
    currentVal = myELM327.absBaroPressure();
    currentValString = "absBaroPressure";
  },
  NULL,    //52
  NULL,    //53
  NULL,    //54
  NULL,    //55
  NULL,    //56
  NULL,    //57
  NULL,    //58
  NULL,    //59
  [&]() {    //60
    currentVal = myELM327.catTempB1S1();
    currentValString = "catTempB1S1";
  },
  [&]() {    //61
    currentVal = myELM327.catTempB2S1();
    currentValString = "catTempB2S1";
  },
  [&]() {    //62
    currentVal = myELM327.catTempB1S2();
    currentValString = "catTempB1S2";
  },
  [&]() {    //63
    currentVal = myELM327.catTempB2S2();
    currentValString = "catTempB2S2";
  },
  [&]() {    //64
    currentVal = myELM327.supportedPIDs_41_60();
    currentValString = "supportedPIDs_41_60";
  },
  [&]() {    //65
    currentVal = myELM327.monitorDriveCycleStatus();
    currentValString = "monitorDriveCycleStatus";
  },
  [&]() {    //66
    currentVal = myELM327.ctrlModVoltage();
    currentValString = "ctrlModVoltage";
  },
  [&]() {    //67
    currentVal = myELM327.absLoad();
    currentValString = "absLoad";
  },
  [&]() {    //68
    currentVal = myELM327.commandedAirFuelRatio();
    currentValString = "commandedAirFuelRatio";
  },
  [&]() {    //69
    currentVal = myELM327.relativeThrottle();
    currentValString = "relativeThrottle";
  },
  [&]() {    //70
    currentVal = myELM327.ambientAirTemp();
    currentValString = "ambientAirTemp";
  },
  [&]() {    //71
    currentVal = myELM327.absThrottlePosB();
    currentValString = "absThrottlePosB";
  },
  [&]() {    //72
    currentVal = myELM327.absThrottlePosC();
    currentValString = "absThrottlePosC";
  },
  [&]() {    //73
    currentVal = myELM327.absThrottlePosD();
    currentValString = "absThrottlePosD";
  },
  [&]() {    //74
    currentVal = myELM327.absThrottlePosE();
    currentValString = "absThrottlePosE";
  },
  [&]() {    //75
    currentVal = myELM327.absThrottlePosF();
    currentValString = "absThrottlePosF";
  },
  [&]() {    //76
    currentVal = myELM327.commandedThrottleActuator();
    currentValString = "commandedThrottleActuator";
  },
  [&]() {    //77
    currentVal = myELM327.timeRunWithMIL();
    currentValString = "timeRunWithMIL";
  },
  [&]() {    //78
    currentVal = myELM327.timeSinceCodesCleared();
    currentValString = "timeSinceCodesCleared";
  },
  NULL,    //79
  [&]() {    //80
    currentVal = myELM327.maxMafRate();
    currentValString = "maxMafRate";
  },
  [&]() {    //81
    currentVal = myELM327.fuelType();
    currentValString = "fuelType";
  },
  [&]() {    //82
    currentVal = myELM327.ethonolPercent();
    currentValString = "ethonolPercent";
  },
  [&]() {    //83
    currentVal = myELM327.absEvapSysVapPressure();
    currentValString = "absEvapSysVapPressure";
  },
  [&]() {    //84
    currentVal = myELM327.evapSysVapPressure2();
    currentValString = "evapSysVapPressure2";
  },
  NULL,    //85
  NULL,    //86
  NULL,    //87
  NULL,    //88
  [&]() {    //89
    currentVal = myELM327.absFuelRailPressure();
    currentValString = "absFuelRailPressure";
  },
  [&]() {    //90
    currentVal = myELM327.relativePedalPos();
    currentValString = "relativePedalPos";
  },
  [&]() {    //91
    currentVal = myELM327.hybridBatLife();
    currentValString = "hybridBatLife";
  },
  [&]() {    //92
    currentVal = myELM327.oilTemp();
    currentValString = "oilTemp";
  },
  [&]() {    //93
    currentVal = myELM327.fuelInjectTiming();
    currentValString = "fuelInjectTiming";
  },
  [&]() {    //94
    currentVal = myELM327.fuelRate();
    currentValString = "fuelRate";
  },
  [&]() {    //95
    currentVal = myELM327.emissionRqmts();
    currentValString = "emissionRqmts";
  },
  [&]() {    //96
    currentVal = myELM327.supportedPIDs_61_80();
    currentValString = "supportedPIDs_61_80";
  },
  [&]() {    //97
    currentVal = myELM327.demandedTorque();
    currentValString = "demandedTorque";
  },
  [&]() {    //98
    currentVal = myELM327.torque();
    currentValString = "torque";
  },
  [&]() {    //99
    currentVal = myELM327.referenceTorque();
    currentValString = "referenceTorque";
  },
  NULL,    //100
  [&]() {    //101
    currentVal = myELM327.auxSupported();
    currentValString = "auxSupported";
  },
};

public:
    void initializePids() {
        Serial.println("Initialize start");
        uint32_t pids1 = 0;
        do {
            pids1 = myELM327.supportedPIDs_1_20();
            if (myELM327.nbdevice.DisplayUpdatedValue_rx_state == ELM_SUCCESS) {
                break;
            } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
                break;
                myELM327.printError();    
            }
        }
        while (!isError(myELM327.nb_rx_state));
    
        uint32_t pids2 = 0;
        do {
            pids2 = myELM327.supportedPIDs_21_40();
            if (myELM327.nb_rx_state == ELM_SUCCESS) {
                break;
            } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
                break;
                myELM327.printError();    
            }
        }
        while (!isError(myELM327.nb_rx_state));
    
        uint32_t pids3 = 0;
        do {
            pids3 = myELM327.supportedPIDs_41_60();
            if (myELM327.nb_rx_state == ELM_SUCCESS) {
                break;
            } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
                break;
                myELM327.printError();    
            }
        }
        while (!isError(myELM327.nb_rx_state));
    
        uint32_t pids4 = 0;
        do {
            pids4 = myELM327.supportedPIDs_61_80();
            if (myELM327.nb_rx_state == ELM_SUCCESS) {
                break;
            } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
                break;
                myELM327.printError();    
            }
        }
        while (!isError(myELM327.nb_rx_state));
    
    
        int i = 0;
        for (int j = 31; j >= 0; j--) {
            if (bitRead(pids1,j) == 1) {
                availabilityMap[i] = true;
            }        
            i++;
        }
    //    for (int j = 0; j < 31; i++) {
    //        if(isBitSet(pids2,j)) {
    //            availabilityMap[i] = true;
    //        }
    //        i++;
    //    }
    //    for (int j = 0; j < 31; i++) {
    //        if(isBitSet(pids3,j)) {
    //            availabilityMap[i] = true;
    //        }
    //        i++;
    //    }
    //    for (int j = 0; j < 31; i++) {
    //        if(isBitSet(pids4,j)) {
    //            availabilityMap[i] = true;
    //        }
    //        i++;
    //    }
    
        for (int j = 0; j < 31; j++)
        {
            if (availabilityMap[j] && (CallBackFunctions[j] != NULL)) {
                availablePids[availableIdSize] = j;
                availableIdSize++;
            }
        }
    }

    void readCurrentPid(size_t currentPid) {
      CallBackFunctions[availablePids[currentPid]]();
    }

    void nextPid() {
      if (currentPid == availableIdSize - 1) {
            currentPid = 0;
        }
        currentPid++;
    }

    const double getCurrentVal() {
      return currentVal;
    }

    const String getCurrentValString() {
      return currentValString;
    }
};

class DeviceController {
private:
    const Display display;
    const Button buttonNext;
    const ELM327 myELM327;
    const PidController pidController;
public:
    void initializeDevice(String deviceName, String devicePass) {
      display.displayText("|Connecting|", 20);
#if LED_BUILTIN
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
#endif
    
        DEBUG_PORT.begin(1200);
        SerialBT.setPin(devicePass);
        ELM_PORT.begin("ArduHUD", true);
    
        if (!ELM_PORT.connect(deviceName))
        {
            DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
            display.clearAndDisplayText("|FAILED 1|" , 20);
            while (1);
        }
    
        if (!myELM327.begin(ELM_PORT, false, 5000, '0'))
        {
            Serial.println("Couldn't connect to OBD scanner - Phase 2");
            display.clearAndDisplayText("|Failed 2|", 20);
            while (1);
        }
    
        Serial.println("Connected to ELM327");
        display.clearAndDisplayText("|Connected|", 20);
        pidController.initializePids();
    }

    void DisplayUpdatedValue() {
        display.clearBuffer();
        pidController.readCurrentPid();
        if (myELM327.nb_rx_state == ELM_SUCCESS) {
            display.displayText(currentValString,12); 
            display.displayText(String(currentVal), 20);
        } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
            myELM327.printError();    
        }
    }

    bool switchScheduled() {
      return buttonNext.isButtonClicked();
    }

    void checkForButtonUpdatesAndUpdate() {
      if (switchScheduled()) {
            pidController.nextPid();
            delay(250);
        }
    }
};

bool isBitSet(uint32_t value, uint8_t bitIndex) {
    return (value & (1 << bitIndex)) ? true : false;
}

bool isError(int8_t state) {
    return state != ELM_GETTING_MSG && state != ELM_SUCCESS;
}

DeviceController device;
void setup()
{
    device.initializeDevice();
}

void loop()
{
    device.checkForButtonUpdatesAndUpdate();
    device.DisplayUpdatedValue(); 
}
