#include <Servo.h>
#include <LiquidCrystal.h>

#define margem 30
#define velocidade 3

LiquidCrystal lcd(12,11,7,6,5,4);
//led dbg
int pld0 = 12, pld1 = 11;//0 verde, 1 vermelho

//potenciometro
int ppot = A2;
float potread;
int potmap;

//botao
int pbot = 8;

int ligar = 0; 

//variaveis do servo
int ps0 = 9;
int ps1 = 10;
Servo s0;
Servo s1;
int s0read;
int s1read;

//angulo de deslocamento
int ang;
//ldr
int pldr0 = A0;
int pldr1 = A1;
float ldr0=0, ldr1=0;

void setup() {
  lcd.begin(16,2);
  lcd.print("Pressione o botao amarelo.");
  
  pinMode(pbot, INPUT);
  
  
  //configuracao dos servos
  pinMode(ps0,OUTPUT);
  pinMode(ps1,OUTPUT);
  
  s0.attach(ps0);
  s0.write(90);

  s1.attach(ps1);
  s1.write(20);
  
  //
  Serial.begin(9600);
}

void loop() {
    s0read = s0.read();
    s1read = s1.read();
  if(digitalRead(pbot) == 1){
    if(ligar == 0){
      //liga
      ligar = 1;
      Serial.print("Status ligar if: ");
      Serial.println(ligar);
    }else if(ligar == 1){
      //desliga
      ligar = 0;
      s0.write(90);
      s1.write(20);
      Serial.print("Status ligar if: ");
      Serial.println(ligar);
    }
    delay(500);// delay adicionado para evitar que ligar fique em 1
  }
  
      Serial.print("Status ligar: ");
      Serial.println(ligar);
  if(ligar == 1){
    //leitura do pot
    potread = analogRead(ppot);
    potmap = map(potread,0,1023,0,180);//mapeando para graus
    //leitura do ldr
    ldr0 = analogRead(pldr0);
    ldr1 = analogRead(pldr1);
    

    //movimento motor s0
    if(ldr0 > (ldr1 + margem)){
      //esquerda decresce
      ang = s0read - velocidade;
      s0.write(ang);
    }else if(ldr1 > (ldr0 + margem)){
      //direita cresce
      ang = s0read + velocidade;
      s0.write(ang);
    }

    //movimento motor s1
    if(potmap <= 20){
      s1.write(potmap);
    }
    
    //saidas terminal
    Serial.print("leitura ldr0: ");
    Serial.println(ldr0);
    Serial.print("leitura ldr1: ");
    Serial.println(ldr1);
    Serial.print("Pos S0: ");
    Serial.println(s0read);
    Serial.print("Pos S1: ");
    Serial.println(s1read);
    delay(100);//200ms
    Serial.println(" ");

    
    
    
  }
    Serial.print("Pos S1: ");
    Serial.println(s1read);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("X:");
    lcd.print(s0read);
    lcd.write(B11011111);//caractere º
    lcd.print(" S0:");
    lcd.print(ldr0);
    lcd.setCursor(0,1);
    lcd.print("Y:");
    lcd.print(s1read);
    lcd.write(B11011111);
    lcd.print(" S1:");
    lcd.print(ldr1);
    
    
}