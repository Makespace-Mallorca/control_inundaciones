#include <virtuabotixRTC.h>

virtuabotixRTC myRTC(6, 7, 8);    // CLK -> 6, DAT -> 7, RST -> 8

const int detectorAgua = 3;       // sensor agua (humedo -> 0´1V -> DO LED encendido | seco -> 4´3V -> DO LED apagado)
const int rearme = 2;             // pulsador (pulsado -> HIGH -> cerrado | sin pulsar -> LOW -> abierto)
const int releAbrir = 4;          // rele 1 (HIGH -> 5V -> led apagado -> NO cerrado NC abierto | LOW -> 0´036V -> led encendido -> NO abierto NC cerrado)
const int releCerrar = 5;         // rele 2 (HIGH -> 5V -> led apagado -> NO cerrado NC abierto | LOW -> 0´036V -> led encendido -> NO abierto NC cerrado)

int valvulaAoC = 1;               // variable donde se guarda si la válvula está abierta o cerrada (0 = válvula cerrada | 1 = válvula abierta)

void setup() 
{
  Serial.begin(9600);
  // myRTC.setDS1302Time(00, 50, 11, 7, 14, 3, 2021); //  Descomentar para poner el reloj en hora SS, MM, HH, DW, DD, MM, YYYY
  
  pinMode( detectorAgua, INPUT);
  pinMode( rearme, INPUT);
  pinMode( releAbrir, OUTPUT);
  pinMode( releCerrar, OUTPUT);

  digitalWrite(releAbrir, HIGH);    //Inicializa el relé que abre la válvula (HIGH -> relé en reposo)
  digitalWrite(releCerrar, HIGH);   //Inicializa el relé que cierra la válvula (HIGH -> relé en reposo)
} 

void loop() 
{   
  if (digitalRead(rearme) == LOW)               // si el pulsador de rearme no está presionado comprueba si hay inundación
  {
    myRTC.updateTime();                         // Carga los datos de la fecha y la hora del RTC
    
    /*Serial.print(myRTC.dayofmonth);           // Imprime en el puerto serie la fecha y la hora actuales
    Serial.print("/");
    Serial.print(myRTC.month);
    Serial.print("/");
    Serial.print(myRTC.year);
    Serial.print(" ");
    Serial.print(myRTC.hours);                
    Serial.print(":");
    Serial.print(myRTC.minutes);
    Serial.print(":");
    Serial.println(myRTC.seconds);
    delay(1000);*/
    
    if ((myRTC.dayofmonth == 1)&&(myRTC.hours == 4)&&(myRTC.minutes == 0)&&(myRTC.seconds == 0)&&(valvulaAoC == 1))  // Cierra y abre la válvula para evitar que calcifique con los años asegurándose antes que la válvula no está cerrada por una inundación
      {
        digitalWrite (releCerrar, LOW);
        delay(9000);
        digitalWrite (releCerrar, HIGH);
        delay(1000);
        digitalWrite (releAbrir, LOW);
        delay(9000);
        digitalWrite (releAbrir, HIGH);
      }
    
    if ((digitalRead(detectorAgua) == LOW) && (valvulaAoC == 1))         // si el sensor de agua esta mojado (LOW) y la valvula esta abierta (1) cierra la válvula
        { 
         digitalWrite(releCerrar, LOW);
         delay (9000);
         digitalWrite(releCerrar, HIGH);
         valvulaAoC = 0;                                                 // Cambia el estado a válvula cerrada (0)
        }
   
  }
  else                                                                   //si el pulsador de rearme a sido presionado y la válvula esta cerrada abre la válvula
    {
      if (valvulaAoC == 0)
      digitalWrite(releAbrir, LOW);
      delay (9000);
      digitalWrite(releAbrir, HIGH);
      valvulaAoC = 1;
    }
}
