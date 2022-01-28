#include <Servo.h>
#include <LiquidCrystal.h>

#define margem 30
#define velocidade 3
#define ligar 0
#define tempo_de_tarefa 5000//ms
#define intervalo_tarefa 10000//ms

//os motores so devem sem mover quando chamado pela funcao principal

LiquidCrystal lcd(12,11,7,6,5,4);
// variavel para controle do momento de execução de tarefas
unsigned long tempo;
unsigned long tempo_anterior;
unsigned long decorrido;

//botoes e variaveis de sinalizacao
int pbot_status_motor_ldr = 8;
int pbot_status_higro = 2;
int pbot_iniciar = 3;
int funcao_A = 0; 
int funcao_A1 = 0;
int funcao_B = 0; 
int funcao_principal = 0; 


//variaveis do higrometro
int phigro_D = 13;
int phigro_A = A5;
float leitura_hig;

//variaveis do servo
int ps0 = 9;
int ps1 = 10;
Servo s0; //servo 0
Servo s1; //servo 1
int s0read;
int s1read;

//variaveis do sensor de luminosidade (ldr)
int pldr0 = A0;
int pldr1 = A1;
int pldrC = A2;
int pldrB = A3;
int ldr0=0, ldr1=0, ldrC, ldrB;

//angulo de deslocamento
int ang0, ang1;

//funcoes

//Interpretação dos estados dos botoes
void leitura_botoes(){
   //Esta funcao altera as variaveis que indicam qual tarefa esta sendo executada
   //
   // Funcao A: inicia e para a tarefa de posicionamento do painel e exibe as leituras dos sensores de luz no lcd
   // Funcao A1: mesma que A mas exibe a posição dos motores
   // Funcao B: inicia e para a tarefa de leitura do sensor de umidade, e exibe a leitura no lcd
   //
   
  if(digitalRead(pbot_status_motor_ldr) == 1 && digitalRead(pbot_status_higro) == 1 && digitalRead(pbot_iniciar) == 0){
    if(funcao_A1 == 0){
      funcao_A1 = 1;
      funcao_A = 0;
      funcao_B = 0;
      Serial.println("funcao A1");
    }else if(funcao_A1 == 1){
      funcao_A1 = 0;
    }

    delay(500);
  }
  else if(digitalRead(pbot_status_motor_ldr) == 1 && digitalRead(pbot_status_higro) == 0 && digitalRead(pbot_iniciar) == 0){
    if(funcao_A == 0){
      //liga
      funcao_A = 1;
      funcao_A1 = 0;
      funcao_B = 0;
      Serial.println("funcao A");
    }else if(funcao_A == 1){
      //desliga
      
      funcao_A = 0;
      
    }
    delay(500);// delay adicionado para evitar que ligar fique em 1
  }
  else if(digitalRead(pbot_status_higro) == 1 && digitalRead(pbot_status_motor_ldr) == 0 && digitalRead(pbot_iniciar) == 0){
    if(funcao_B == 0){
      //liga
      funcao_A = 0;
      funcao_A1 = 0;
      funcao_B = 1;
      Serial.println("funcao B");
    }else if(funcao_B == 1){
      //desliga
      funcao_B = 0;
    }
    delay(500);// delay adicionado para evitar que ligar fique em 1
  }
  else if(digitalRead(pbot_iniciar) == 1 && digitalRead(pbot_status_motor_ldr) == 0 && digitalRead(pbot_status_higro) == 0){
    if(funcao_principal == 0){
      //liga
      funcao_A = 0;
      funcao_A1 = 0;
      funcao_B = 0;
      funcao_principal = 1;
      tempo_anterior = tempo;
      Serial.println("funcao C");
    }else if(funcao_principal == 1){
      //desliga
      funcao_principal = 0;
      s0.write(90);
      s1.write(20);
    }
    delay(500);// delay adicionado para evitar que ligar fique em 1
  }
}



void dados_ldr(){
  lcd.clear();
  
  lcd.setCursor(0,0);

  lcd.print(" S0:");
  lcd.print(ldr0);
  lcd.print(" S1:");
  lcd.print(ldr1);
  
  lcd.setCursor(0,1);
  lcd.print(" SC:");
  lcd.print(ldrC);
  lcd.print(" SB:");
  lcd.print(ldrB);
}
void dados_motor(){
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("X:");
  lcd.print(s0read);
  lcd.write(B11011111);//caractere º
  
  lcd.setCursor(0,1);
  lcd.print("Y:");
  lcd.print(s1read);
  lcd.write(B11011111);
}

//funcoes A
void funcao_botao_amarelo(){

    s0read = s0.read();
    s1read = s1.read();
      //leitura do ldr
    ldr0 = analogRead(pldr0);
    ldr1 = analogRead(pldr1);
    ldrC = analogRead(pldrC);
    ldrB = analogRead(pldrB);
    if(funcao_A == 0 && funcao_A1 == 0){
        //movimento motor servo_0
      if(ldr0 > (ldr1 + margem)){
        //esquerda decresce
        ang0 = s0read - velocidade;
        s0.write(ang0);
      }else if(ldr1 > (ldr0 + margem)){
        //direita cresce
        ang0 = s0read + velocidade;
        s0.write(ang0);
      }
  
      //movimento motor servo_1
      if(ldrC > (ldrB + margem)){
        //pra cima
        ang1 = s1read - velocidade;
        s1.write(ang1);
      }else if(ldrB > (ldrC + margem)){
        ang1 = s1read + velocidade;
        if(ang1 <= 40){
          s1.write(ang1);
        }
      }
    }
    
    if(funcao_A == 1){
      dados_ldr();
    }else if(funcao_A1 == 1){
      dados_motor();
    }

    //saidas terminal
    Serial.print("leitura ldr0: ");
    Serial.println(ldr0);
    Serial.print("leitura ldr1: ");
    Serial.println(ldr1);
    Serial.print("leitura ldrC: ");
    Serial.println(ldrC);
    Serial.print("leitura ldrB: ");
    Serial.println(ldrB);
    Serial.print("Pos S0: ");
    Serial.println(s0read);
    Serial.print("Pos S1: ");
    Serial.println(s1read);
    delay(100);//200ms
    Serial.println(" ");
    
}

//funcoes B
void funcao_botao_vermelho(){
  leitura_hig = analogRead(phigro_A);
  //220 = agua ; 1023 = ar ; umidade_aceitavel < 620
  Serial.print("Saida analogica do higrometro: ");
  Serial.println(leitura_hig);
  
  if(funcao_B == 1){
    lcd.clear();
  
    lcd.setCursor(0,0);
    lcd.print("Umidade: ");
    lcd.print(leitura_hig);
    lcd.setCursor(0,1);
    if(leitura_hig < 620 && leitura_hig > 400){
      lcd.print("Hidratada");
    }else if(leitura_hig > 620){
      lcd.print("Desidratada");
    }else if(leitura_hig < 400){
      lcd.print("Execesso");
    }
  }
  
}

void resumo(){
  //resume todos os dados dos sensores no lcd
  lcd.clear();
  
  lcd.setCursor(0,0);

  lcd.print("0:");
  lcd.print(ldr0);
  lcd.print("1:");
  lcd.print(ldr1);
  lcd.print("C:");
  lcd.print(ldrC);
  lcd.print("B:");
  lcd.print(ldrB);
  
  lcd.setCursor(0,1);
 // lcd.print("x:");
  //lcd.print(s0read);
  //lcd.print("y:");
  //lcd.print(s0read);
  //lcd.print(" ");
  if(leitura_hig < 620 && leitura_hig > 400){
      lcd.print("OK");
    }else if(leitura_hig > 620){
      lcd.print("Seco");
    }else if(leitura_hig < 400){
      lcd.print("Exc");
    }
}

void setup() {
  lcd.begin(16,2);
  lcd.print("Pressione o botao amarelo.");
  
  pinMode(pbot_status_motor_ldr, INPUT);
  
  
  //configuracao dos servos
  pinMode(ps0,OUTPUT);
  pinMode(ps1,OUTPUT);
  
  s0.attach(ps0);
  s0.write(90);

  s1.attach(ps1);
  s1.write(20);

  s0read = s0.read();
  s1read = s1.read();
  
  //
  Serial.begin(9600);
}

void loop() {

  leitura_botoes();

  Serial.print("Tempo decorrido: ");
  tempo = millis();
  Serial.println(tempo);
  
  if(funcao_A == 1){
//funcao do botao amarelo
    funcao_botao_amarelo();//remover movimento dos motores
  }else if(funcao_A1 == 1){
    funcao_botao_amarelo();
  }
  else if(funcao_B == 1){
//funcao do botao vermelho
    funcao_botao_vermelho();
  }
  if(funcao_principal == 1){//executa a cada 1 minuto
      decorrido = tempo - tempo_anterior;
      if(funcao_A == 0 && funcao_B == 0 && funcao_A1 == 0){
        
        lcd.clear();
  
        lcd.setCursor(0,0);
      
        lcd.print("Intervalo");
        lcd.setCursor(0,1);
        lcd.print(decorrido/1000);
        lcd.print("/");
        lcd.print(intervalo_tarefa/1000);
      }

     //funcao do botao preto
     if(decorrido >= intervalo_tarefa){
      tempo_anterior = tempo;
      tempo = millis();
      decorrido = tempo - tempo_anterior;
      while(decorrido <= 5000){
        funcao_botao_amarelo();
        funcao_botao_vermelho();
        if(funcao_A == 0 && funcao_B == 0 && funcao_A1 == 0)resumo();
        tempo = millis();
        decorrido = tempo - tempo_anterior;
      }
       
     }
     
  }
  
  lcd.clear();

}