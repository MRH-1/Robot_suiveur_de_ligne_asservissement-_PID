#include <SimpleTimer.h>
//6.5;0.5
//code asservissement p ,pi ,,pid
unsigned long tickR=0;unsigned long tickl=0;
float nbtourparsecR;float nbtourparsecl;
int previous=0;int pred=0;
#define enableL 11
#define enableR 10
#define MRav 9
#define MRar 8
#define MLav 12
#define MLar 13
 float t1=0;
 float t2=0;
int c ;
int l ;
int r ;
int mr ;
int ml ;
int v = 150;
long somL=0;
long somR=0;
unsigned long freqcodeuseR=0;
unsigned long freqcodeuseL =0;
 float secR=0;
 float secL=0;
unsigned  long tickcodeusel=0;
unsonst int nbtickpartourl=330; // nbr de ticks par tour
const int nbtickpartourR=1670; 
float reducteur=0.07;igned  long tickcodeuseR=0;
int commandevoulu=0.33;
int cmdr = 70;int cmdl=0 ;                  // Commande du moteur
const int freqech =100;  // Fréquence d'exécution de l'asservissement
c
float erreurR=0;float erreurL=0;
int erreurpredL=0;
int erreurpredR=0;
int deltaL=0;                     //835-->90
                                  //557-->60
int deltaR=0;

float  kpr=900;float kpl=0.002482;float kir=0;float kil=0.00494;
float kdR=0;float kdL=0;
void setup() {
Serial.begin(9600);
pinMode(enableL, OUTPUT);pinMode(enableR, OUTPUT);pinMode(MRav, OUTPUT);
pinMode(MRar, OUTPUT);pinMode(MLav, OUTPUT);pinMode(MLar, OUTPUT);
attachInterrupt(digitalPinToInterrupt(3),compteurl, RISING);
attachInterrupt(digitalPinToInterrupt(2),compteurR, RISING); 
 pinMode (A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
pinMode(2,INPUT);pinMode(3,INPUT);   
}
/*void avancer() {
  digitalWrite (MRav, HIGH);
  digitalWrite (MRar, LOW);
  analogWrite (enableR, v);

  digitalWrite (MLav, HIGH);
  digitalWrite (MLar, LOW);
  analogWrite (enableL, v + 30);
}*/ 
void arreter() {
  digitalWrite (MRav, HIGH);
  digitalWrite (MRar, LOW);
  analogWrite (enableR, 0);

  digitalWrite (MLav, HIGH);
  digitalWrite (MLar, LOW);
  analogWrite (enableL, 0);
}
void readsensor() {
  ml = digitalRead(A2);
 // Serial.print(ml);
  mr = digitalRead(A3);
    //Serial.print(mr);
  l = digitalRead(A1);
   // Serial.print(l);
  r = digitalRead(A4);
    //Serial.print(r);
  c = digitalRead(A0);
    //Serial.print(c);
}
void leftadjust() {
    digitalWrite (MLav, LOW);
    digitalWrite (MLar, HIGH);
    analogWrite (enableL, 80);

    digitalWrite (MRav, HIGH);
    digitalWrite (MRar, LOW);
    analogWrite (enableR, 80);
 

}


void rightadjust() {
    digitalWrite (MLav, HIGH);
    digitalWrite (MLar, LOW);
    analogWrite (enableL, 80)
    ;

    digitalWrite (MRav, LOW);
    digitalWrite (MRar, HIGH);
    analogWrite (enableR ,80);

  }
  void suiveur() {
  if (!l && !r &&  mr && ml )avancer();
  else if (l&&!r) {
    gauche();
    
      
  }

  else if (!l&&r) {
    
  
     droite();
     
 }
  }
void asservissement(){
  if( millis()-previous>10)
    {asservissementR(); 
     asservissementL();
   previous=millis();}}
void loop()  {
  digitalWrite (MLav, HIGH);
  digitalWrite (MLar, LOW);
  analogWrite (enableL, cmdl);
    if( millis()-previous>10)
    {//asservissementR(); 
     asservissementL();
   previous=millis();}

   // affichage du nbr de tours par seconde chaque instant
if( millis()-pred>1000)
    { secL=tickcodeusel-secL;
    // Serial.println(secR/1670); 
     Serial.println(secL/330); 
     Serial.println("--------------------------------------------------------------------");
      secR=tickcodeuseR;  secL=tickcodeusel;
     pred=millis();}
 
 

}
 
void compteurl(){
    tickcodeusel++;  
}
void compteurR(){
    tickcodeuseR++; 
}

/*-----------------asservissement proportionel p------------------------------------ */
void asservissementR(){ 
 //calcu du nbre de tick de la derniere periode d echaontiollnage
 tickR=tickcodeuseR-tickR;
  // Calcul de l'erreur
     freqcodeuseR = freqech*tickR;
     nbtourparsecR = freqcodeuseR/nbtickpartourR;
    // Serial.println(nbtourparsecR);
 /*/(float)rapport_reducteur;*/
    float erreurR =commandevoulu-nbtourparsecR  ;
    deltaR=erreurR-erreurpredR;
   
    somR=somR+erreurR;

  //determination de la cmd
cmdr=kpr*erreurR+kir*somR+kdR*deltaR;

// normalisation de la cmd
if(cmdr<0) cmdr =0;
if (cmdr>255) cmdr =255;
//Serial.print("cmdr=");//Serial.println(cmdr);
erreurpredR=erreurR;
tickR=tickcodeuseR;

     }
     void asservissementL(){ 
 //calcu du nbre de tick de la derniere periode d echaontiollnage
 //tickl=tickcodeusel-tickl;
  // Calcul de l'erreur
   // freqcodeuseL = freqech*tickl;
    // nbtourparsecl = freqcodeuseL/nbtickpartourl;
     //Serial.println(nbtourparsecl);
     /*/(float)rapport_reducteur;*/
   erreurL =commandevoulu-tickcodeusel;
    somL=somL+erreurL;
    deltaL=erreurL-erreurpredL;
  //determination de la cmd
cmdl=kpl*erreurL+kil*somL;
// normalisation de la cmd
if(cmdl<0) cmdl=0;
if (cmdl>255) cmdl =255;
//Serial.print("cmdl=");Serial.println(cmdl);
tickl=tickcodeusel;
erreurpredL=erreurL;
tickcodeusel=0;
     }
   void avancer() {
    asservissement();
   
  digitalWrite (MRav, HIGH);
  digitalWrite (MRar, LOW);
  analogWrite (enableR, cmdr);

  digitalWrite (MLav, HIGH);
  digitalWrite (MLar, LOW);
  analogWrite (enableL, cmdl);
}

 void droite() {
 do{Serial.println(tickcodeuseR);
asservissement();
 t1=tickcodeusel-t1;
  digitalWrite (MLav, HIGH);
    digitalWrite (MLar, LOW);
    analogWrite (enableL,90);
    digitalWrite (MRav, LOW);
    digitalWrite (MRar, HIGH);
    analogWrite (enableR ,90);
   }while(t1<((1670/4))-75);

  }

void gauche() {
  do{asservissement();
   t2=tickcodeuseR-t2;
    digitalWrite (MLav, LOW);
    digitalWrite (MLar, HIGH);
    analogWrite (enableL, 90);

    digitalWrite (MRav, HIGH);
    digitalWrite (MRar, LOW);
    analogWrite (enableR, 90);}
    while(t2<((1659/4)-72));
      
     Serial.println(tickcodeusel);
}
