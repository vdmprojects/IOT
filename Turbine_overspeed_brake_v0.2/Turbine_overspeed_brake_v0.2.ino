
const float MAXRPM (1300);
const float RPM_mult(1);
const float maxFreq (MAXRPM*RPM_mult);
const float softThreshold = maxFreq * 0.95;
const float hysteresis = maxFreq * 0.05;
const float brakeRange = maxFreq - softThreshold;



const int pulsePin = 8; // Input signal connected to Pin 8 of Arduino
const int brake1Pin = 13;
const int brake2Pin = 3;
const int controlPin1 = 4;
const int controlPin2 = 5;

const int loadRand = 50;

int pulseHigh; // Integer variable to capture High time of the incoming pulse
int pulseLow; // Integer variable to capture Low time of the incoming pulse

float pulseTotal; // Float variable to capture Total time of the incoming pulse
float frequency; // Calculated Frequency
float last_frequency = (1000);
float freqTrend (0);
float RPM = 0 ;


// variables for modeling the turbine for testing. remark out for production.
float tSpeed = 1000;
float tLoad = 1000;
float tJet=1000;
float brakeLoad = 0;




void setup()
{
    pinMode(pulsePin,INPUT);
    pinMode(brake1Pin,OUTPUT);
    pinMode(brake2Pin,OUTPUT);
    pinMode(controlPin1,INPUT);
    pinMode(controlPin2,INPUT);

    Serial.begin(115200);
}


void loop(){

    //GET_RPM(); uncomment for production

    MODEL_TURBINE(); // code for modeling the turbine for testing. remark out for production.

    TURBINE_CONTROL();
    
    last_frequency = frequency;

    float turbineRPM = frequency * RPM_mult;

}
