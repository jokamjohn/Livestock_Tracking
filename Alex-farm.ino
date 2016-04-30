#include <Process.h>;
long linuxBaud = 250000;
bool A = 0;
bool B = 0;
bool C = 0;

int Ai = 2;
int Bi = 3;
int Ci = 4;

unsigned long current_uid;
unsigned long previous_uid = 0;

void setup()
{
 Bridge.begin(); // Initialize the Bridge
 Serial.begin(115200); // open serial connection via USB-Serial
 Serial.print("start"); 
Serial.begin(9600); // Initialize serial communications with the PC
    
    pinMode(Ai, INPUT);
    pinMode(Bi, INPUT);
    pinMode(Ci, INPUT);
}

void loop()
{
 A = digitalRead(Ai);
  B = digitalRead(Bi);
  C = digitalRead(Ci);
  
  if(A){
    
    //Serial.println("A IS HIGH");
 if(C){
    current_uid = 4294955080;
    if (B){
      current_uid = 28489;
      }
    }
    else{
      
      current_uid = 4294953522;
      }
  }else {
    if (C){
      current_uid = 29001;
      }
      if (B){
        if (C){
          current_uid = 27209;
          }
         else {
         current_uid = 28233;
          } 
        }
    }
  
  
  
    if (current_uid != previous_uid){
    
    Serial.println(current_uid);
    
    previous_uid = current_uid;
    delay(800);
 

 epoch = timeInEpoch();
    String time = "\"time\":\"" + String(epoch) + "000" + "\"";

    String metrics = "\"area\":" + String(h) + ", \"capturedTag\":" + String(current_uid) + ", \"soil\":" + String(moistureLevel);

    String jsonData = "{" + time + "," + metrics + "}";
    
    Serial.println(jsonData);

    Process p;
    p.runShellCommand("curl -k -X POST " + url + " -d '" + jsonData + "'");
    while(p.running());
 
 while (p.available()) {
 int result = p.parseInt(); 
 Serial.println(result); 
 }
 delay(2000); 
}

}

//Returns a UNIX timestamp 
unsigned long timeInEpoch() {
  Process time;                   
  char epochCharArray[12] = "";

  // Get UNIX timestamp
  time.begin("date");
  time.addParameter("+%s");
  time.run();
  
  // When execution is completed, store in charArray
  while (time.available() > 0) {
    time.readString().toCharArray(epochCharArray, 12);
  }
  
  // Return long with timestamp
  return atol(epochCharArray);
}

  
