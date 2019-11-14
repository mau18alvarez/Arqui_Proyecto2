#include <Servo.h>
#include <infrarrojo.h>

//Defining variables for servo motors
#define pinServoArriba 6
#define pinServoAbajo 5

//-Set Angle for servos
//-Servo Arriba
#define servoFinal 43
#define servoInit 140
#define servoMidRGB 97

//-Servo abajo
#define servoRojo 100
#define servoVerde 75
#define servoAzul 50

unsigned long R, G, B, max;
double fr, fg, fb;

//Defining variables for RGB sensor
#define SEL_RED            \
    digitalWrite(S2, LOW); \
    digitalWrite(S3, LOW);
#define SEL_GREEN           \
    digitalWrite(S2, HIGH); \
    digitalWrite(S3, HIGH);
#define SEL_BLUE           \
    digitalWrite(S2, LOW); \
    digitalWrite(S3, HIGH);
#define SEL_CLEAR           \
    digitalWrite(S2, HIGH); \
    digitalWrite(S3, LOW);

//Variables infrarrojo
infrarrojo estado(7);
int VALOR; // Variable que recibe el dato
bool lec_infr = false;

//Defining pin-out for RGB sensor
int S0 = 10;     //color gris
int S1 = 11;     //color morado
int S2 = 12;     //color naranja
int S3 = 13;     //color azul
int OE = 8;      //color blanco
int RGB_OUT = 9; //color cafe

//Defining variables for ESPWIFI
int PIN1_ESP = 4;
int PIN2_ESP = 3;
int VAL_ESP = 2;

Servo servoArriba, servoAbajo;




void setup()
{

    //Set up for Servo Motors
    servoArriba.attach(pinServoArriba);
    servoAbajo.attach(pinServoAbajo);
    
    //Set up for WIFI-ESP
    pinMode(PIN1_ESP, OUTPUT);
    pinMode(PIN2_ESP, OUTPUT);
    pinMode(VAL_ESP, OUTPUT);
    digitalWrite(PIN1_ESP, LOW);    
    digitalWrite(PIN2_ESP, LOW);
    digitalWrite(VAL_ESP, LOW);
    
    //Set up for RGB
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(RGB_OUT, INPUT);
    digitalWrite(S0, HIGH);
    digitalWrite(S1, HIGH);
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    digitalWrite(OE, LOW); //always on

    Serial.begin(9600);
}


void loop()
{

    //Manejo de infrarrojo
    VALOR = estado.lectura(VALOR);

    if (!VALOR && !lec_infr)
    {
        lec_infr = true;
        //Aqui se pone lo que quiere que haga
        Serial.println("obstaculo");
        //Setear posicion inicial
    servoArriba.write(servoInit);
    delay(1000);

    servoArriba.write(servoMidRGB);
    delay(2000);

    //RGB SENSOR--------------------------------------------------------------------------------------------------------------------

    SEL_RED;
    R = get_RGB_reading();
    SEL_GREEN;
    G = get_RGB_reading();
    SEL_BLUE;
    B = get_RGB_reading();

    fr = 1.0 / (R * 1e-6);
    fg = 1.0 / (G * 1e-6);
    fb = 1.0 / (B * 1e-6);

    max = max(fr, max(fg, fb));
    //Serial.print("max ");Serial.println(max);

    /* Serial.print("r: "); Serial.print(fr); Serial.println();
    Serial.print("g: "); Serial.print(fg); Serial.println();
    Serial.print("b: "); Serial.print(fb); Serial.println();*/

    if (fr > fg & fr > fb)
    {
        Serial.print("Rojo"), Serial.println();
        servoAbajo.write(servoRojo);
        digitalWrite(PIN1_ESP, LOW), digitalWrite(PIN2_ESP, HIGH), digitalWrite(VAL_ESP, HIGH);
        delay(1000);
        digitalWrite(VAL_ESP, LOW);
        }

    if (fg > fr & fg > fb)
    {
        Serial.print("Verde");
        Serial.println();
        servoAbajo.write(servoVerde);
        digitalWrite(PIN1_ESP, HIGH), digitalWrite(PIN2_ESP, LOW), digitalWrite(VAL_ESP, HIGH);
        delay(1000);
        digitalWrite(VAL_ESP, LOW);
    }

    if (fb > fr & fb > fg)
    {
        Serial.print("Azul");
        Serial.println();
        servoAbajo.write(servoAzul);
        digitalWrite(PIN1_ESP, LOW), digitalWrite(PIN2_ESP, LOW), digitalWrite(VAL_ESP, HIGH);
        delay(1000);
        digitalWrite(VAL_ESP, LOW);
    }

    //Serial.print("r: "); Serial.print(map(fr, 25000, 250000, 0, 255)); Serial.println();
    //Serial.print("g: "); Serial.print(map(fg, 25000, 250000, 0, 255)); Serial.println();
    //Serial.print("b: "); Serial.print(map(fb, 33333, 333333, 0, 255)); Serial.println();
    //RGB SENSOR--------------------------------------------------------------------------------------------------------------------

    servoArriba.write(servoFinal);
    lec_infr = false;
    delay(1000);
    }
    else if (VALOR && lec_infr)
    {
        lec_infr = false;
        //Aqui se pone lo que quiere que haga
        Serial.println("libre");
        digitalWrite(PIN1_ESP, HIGH), digitalWrite(PIN2_ESP, HIGH), digitalWrite(VAL_ESP, HIGH);
        delay(1000);
        digitalWrite(VAL_ESP, LOW);
    }
    delay(100);

    
}

////////////////////////////////////////////////////////////////
unsigned long get_RGB_reading()
{
    unsigned long val;
    noInterrupts();
    val = pulseIn(RGB_OUT, HIGH);
    interrupts();
    return val;
}
