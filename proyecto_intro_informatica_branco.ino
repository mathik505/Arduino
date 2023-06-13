#include <Servo.h>
#include <LiquidCrystal_I2C.h>

//Pines
int FotRes = A0;// Pin de fotoresistor
int Trig = 7;// Pin de Utrasensor 
int Echo = 6;//Pinde Ultrasensor
int PinH = A1;// Pin deservo motor


//Variables
int luminosidad;
int tiempo;
int distancia;
int humedad;
int VS = 34000.0;//Velocidad sonido
Servo motor;
LiquidCrystal_I2C Lsd(0x27, 20, 4);
String estado;
String detectado;

void setup(){
  //Pantalla led
  lcd.init();
  lcd.backlight();
  
  //Analogicos
  pinMode(FotRes, INPUT);
  pinMode(PinH,INPUT);
  
  //Digitales
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  motor.attach(SM);
  
  //Conexion Serial
  Serial.begin(9600);
}

void loop(){
  //Guardar el valor del fotoresistor
  luminicidad = analogRead(FotRes);//Lee entrada analogica
  Serial.print("luminicidad: ");
  Serial.println(luminicidad);
  //Guardar el valor del ultrasonido
  iniciarTrigger();//Emitimos una onda sonica
  tiempo = pulseIn(Echo, HIGH);//Recibimos onda sonica
  distancia = ( tiempo * 0.000001 * VS )/ 2;//Distancia = (velocidad * tiempo)/2
  Serial.print("distancia: ");
  Serial.println(distancia);
  //Guardar el valor de humedad
  humedad = map(analogRead(PinH), 0, 1023, 100, 0);//Lee entrada analogica ; map transforma rango de valores a un rango requerido
  Serial.print("humedad: ");
  Serial.println(humedad);
  //Validar intensidad de luz
  if(luminicidad < 30){
    motor.write(0);//Cierra el toldo
    lcd.setCursor(0,0);
    estado = "Night";
    lcd.print(estado);
  } else {
    lcd.setCursor(0,0);
    estado = "Day  ";
    lcd.print(estado);
    //motor.write(180);//Abre el toldo ; para verificar si no hya nada cerca
    if(distancia < 10){
      motor.write(0);//Cierra el toldo
      lcd.setCursor(8,0);
      detectado = "DETECTED";
      lcd.print(detectado);
    } else {
      motor.write(180);//Abre el toldo
      lcd.setCursor(8,0);
      detectado = "NOTHING ";
      lcd.print(detectado);
    }
  }
  //Pantalla led
    if(humedad < 40){
      lcd.setCursor(0,1);
      lcd.print("H2O = ");
      lcd.print(humedad);
      lcd.print("% ");
      lcd.setCursor(9,1);
      lcd.print("  Water");
    } else{
      lcd.setCursor(0,1);
      lcd.print("H2O = ");
      lcd.print(humedad);
      lcd.print("% ");
    }
    
  delay(1000);//tiempo de espera
}

void iniciarTrigger(){
  // Ponemos el Triiger en estado bajo y esperamos 2 ms
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  // Comenzamos poniendo el pin Trigger en estado bajo
  digitalWrite(Trig, LOW);
}
