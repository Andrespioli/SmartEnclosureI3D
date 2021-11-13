//-------------------------------------------------- CONFIGURACION SENSOR DHT11 ---------------------------------------------------------------------------------
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
#define RELAYPIN 12
#define TEMP_ACT 30.0

DHT dht(DHTPIN, DHTTYPE);

//-------------------------------------------------- CONFIGURACION DE DISPLAY 16X2 ------------------------------------------------------------------------------
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//Creamos el objeto lcd con la dirección 0x3F, 16 columnas y 2 filas
LiquidCrystal_I2C lcd(0x3F,16,2); 

//-------------------------------------------------- CONFIGURACION DE TECLADO MATRICIAL -------------------------------------------------------------------------
#include <Keypad.h>
const byte rowsCount = 4;
const byte columsCount = 4;
const byte rowPins[rowsCount] = { 11, 10, 9, 8 };
const byte columnPins[columsCount] = { 7, 6, 5, 4 };
char keys[rowsCount][columsCount] = {
   { '1','2','3', 'A' },
   { '4','5','6', 'B' },
   { '7','8','9', 'C' },
   { '#','0','*', 'D' }
};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

char tecla;
char variable_c;
char modoManual[4]={'1','2','3'};
char modoAutom[4] ={'4','5','6'};
char frenoEmer[4]={'7','8','9'};
 
char piv[4];
char a_convertir[3];
int i=0;
int y=0;
//-------------------------------------------------------------------- VARIOS ----------------------------------------------------------------------------------------
#define LED_ENC 3

//--------------------------------------------------------------------   Funciones  ----------------------------------------------------------------------------------
//void Automatic(void){

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  lcd.init();//Inicializamos el display
  lcd.backlight();//Encendemos la luz de fondo.
  pinMode(RELAYPIN,OUTPUT);
  digitalWrite(RELAYPIN,LOW);
  pinMode(LED_ENC, OUTPUT);
  digitalWrite(LED_ENC , HIGH);
  dht.begin();
}
 
void loop() {
  lcd.setCursor(0, 0);// Posicionamos el curson en  columna 0, fila 0
    tecla = keypad.getKey(); //Guardamos un char en la variable tecla
    lcd.setCursor(0,0);
    lcd.print("Ingrese el modo:");
  if(tecla){ //Si tecla es diff a null
    lcd.setCursor(i,1);
    piv[i] = tecla;
    lcd.print(tecla);
    i++;
    }
  if(i == 3){
     lcd.setCursor(0,1);
   
    if(! strcmp(piv, modoManual) ){
          lcd.print("Modo manual.");
           delay(1000); 
           lcd.clear();
           lcd.setCursor(0,1);
           lcd.print("Ingrese temperatura:");
           variable_c = keypad.getKey();
           
           int parametro;
           if( variable_c){
            lcd.setCursor(y,1);
            a_convertir[y] = variable_c;
            lcd.print(variable_c);
            y++;
           }
           if( y == 2)
           parametro = atoi(a_convertir);           
           lcd.clear();
           lcd.setCursor(1,1);
           lcd.print(parametro);
           do{
                  delay(500);
                  float t = dht.readTemperature();
                   if ( isnan(t)){
                      Serial.println("Fallo la lectura del sensor");
                      return;
                      }
                      lcd.setCursor(0,0);
                      lcd.print("Temperatura");
                      lcd.setCursor(1,1);
                      lcd.print(t);
                      lcd.setCursor(7,1);
                      lcd.print("*C");
                      lcd.setCursor(14,1);
                      lcd.print("*");
                      delay(500);
                      lcd.setCursor(14,1);
                      lcd.print(" ");
                      Serial.print("Temperatura: ");
                      Serial.print(t);
                      Serial.print(" *C\n ");
                      digitalWrite(RELAYPIN, t > parametro ? HIGH : LOW);
                      tecla = keypad.getKey();
                      }while( tecla!='A');
           }
           //-------------------------------------- MODO AUTOMATICO ------------------------------------------------------------------------------------------------
           else if(! strcmp(piv, modoAutom) ){ //Acá funciona con parámetros preestablecidos por el usuario en el código.
                lcd.print("Modo Autom.");
                delay(2000);
                lcd.clear();
                //char piv = keypad.getKey()
                do{
                  delay(500);
                  float t = dht.readTemperature();
                   if ( isnan(t)){
                      Serial.println("Fallo la lectura del sensor");
                      return;
                      }
                      lcd.setCursor(0,0);
                      lcd.print("Temperatura");
                      lcd.setCursor(1,1);
                      lcd.print(t);
                      lcd.setCursor(7,1);
                      lcd.print("*C");
                      lcd.setCursor(14,1);
                      lcd.print("*");
                      delay(500);
                      lcd.setCursor(14,1);
                      lcd.print(" ");
                      Serial.print("Temperatura: ");
                      Serial.print(t);
                      Serial.print(" *C\n ");
                      digitalWrite(RELAYPIN, t > TEMP_ACT ? HIGH : LOW);
                      tecla = keypad.getKey();
                      }while( tecla!='A');
            }else if(!strcmp(piv, frenoEmer)){
                lcd.print("Freno emergencia.");
                delay(2000);
                lcd.clear();
            }else{
                lcd.print("Modo incorrecto.");
                lcd.print("Vuelva a intentar.");
                delay(2000);
                lcd.clear();
                }
 i=0;
 }
}
