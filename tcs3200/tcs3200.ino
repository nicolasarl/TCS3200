//Frequency control
const int s0 = 50;
const int s1 = 51;
//Filter control
const int s2 = 52;
const int s3 = 53;

const int out = A0;


//Declarations

enum Estado {
  lect_raw, procesado, error, pantalla, serial};
Estado state = lect_raw;

float color[]= {
  0,0,0,0};//R,G,C,B
unsigned long duracion = 0;

void setup(){
  //Pins setup 
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  //OUTPUT pins initialization
  digitalWrite(s0, LOW);// 2% frequency scaling: 10-12kHz Limited because of Arduino's clock
  digitalWrite(s1, HIGH);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  
  Serial.begin(9600);
  Serial.flush();

}

void loop(){

  switch(state){

  case lect_raw:

    for(int i = 0; i < 2; i++){

      digitalWrite(s2, i);//Red: 00, Blue: 01; Clear: 10; Green: 11
      for(int j = 0; j < 2; j++){
        digitalWrite(s3, j);
        delay(1);//in miliseconds
        duracion= pulseIn(out,digitalRead(out) == HIGH ? LOW : HIGH);
        color[2*i+j] = 1000000/duracion;
        Serial.print("Color ");
        Serial.print(2*i+j);
        Serial.print(": ");
        Serial.println(color[2*i+j]);
      }
    }
    if (color[0] == 0 || color[1] == 0 || color[2] == 0 || color[3] == 0){
      state = error;      
    }
    else{
      state = procesado;
    }

    break;
  case procesado:
  
    state = pantalla;
    break;
  case pantalla:
    state = serial;
    break;
  case serial:
  
    Serial.print("Frecuencia fotodiodo rojo en HZ:");
    Serial.println(color[0],DEC);
    Serial.print("Frecuencia fotodiodo blue en Hz:");
    Serial.println(color[1],DEC);
    Serial.print("Frecuencia fotodiodo clear en HZ:");
    Serial.println(color[2],DEC);
    Serial.print("Frecuencia fotodiodo green en Hz:");
    Serial.println(color[3], DEC);
    Serial.println();
    delay(5000);
    state = lect_raw;
    break;
  case error:
  
    Serial.println("Se ha producido un error en la funcion pulseIn");
    state = lect_raw;
    break;
  }

}

