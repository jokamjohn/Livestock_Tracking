
 /*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

 #include <Process.h>
 
bool A = 0;
bool B = 0;
bool C = 0;

int Ai = 2;
int Bi = 3;
int Ci = 4;

bool barnState = 0;
bool dipState = 0;

int barnPin = 5;
int dipPin = 6;

int location_barn = A0;
int location_dip = A1;

bool locationStatus_barn = 0;
bool locationStatus_dip = 0;

unsigned long current_uid;
unsigned long previous_uid = 0;

unsigned long epoch; // UNIX timestamp for sensor readings

void setup() {
    Bridge.begin();
    Serial.begin(9600); // Initialize serial communications with the PC
    setClock();
    
    pinMode(Ai, INPUT);
    pinMode(Bi, INPUT);
    pinMode(Ci, INPUT);
    
    pinMode(barnPin, INPUT);
    pinMode(dipPin, INPUT);
    
    pinMode(location_barn, INPUT);
    pinMode(location_dip, INPUT);
}

void loop() {
  
  A = digitalRead(Ai);
  B = digitalRead(Bi);
  C = digitalRead(Ci);

  barnState = digitalRead(barnPin);
  dipState = digitalRead(dipPin);

  locationStatus_barn = digitalRead(location_barn);
  locationStatus_dip = digitalRead(location_dip);
  
  if (digitalRead(barnPin)){
    
  if(A){
    
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
    
    Serial.print("Captured tag, UID: ");
    Serial.println(current_uid);
    Serial.println("LOCATION : COW AT BARN");
    Serial.println("");
  
     /*if (current_uid != previous_uid){
    Serial.print("Captured tag, UID: ");
    Serial.println(current_uid);
    Serial.println("LOCATION : COW AT BARN");
    Serial.println("");
    previous_uid = current_uid;
    
     }*/
     delay(800);

      epoch = timeInEpoch();
    
    String time = "\"time\":\"" + String(epoch) + "000" + "\"";

    String url = "https://alex-farm.firebaseio.com/barnArea.json";

    String location = "\"BARN\"";

    String readerOne = "\"Reader 1\"";

    String data = "\"location\":" + String(location) + ", \"capturedTag\":" + String(current_uid) + ", \"Reader\":" + String(readerOne);

    String jsonData = "{" + time + "," + data + "}";
    
    Serial.println(jsonData);

    Process p;
    p.runShellCommand("curl -k -X POST " + url + " -d '" + jsonData + "'");
    while(p.running());
   
   
}else if(digitalRead(dipPin)){
  
  if(A){
    
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
    
    Serial.print("Captured tag, UID: ");
    Serial.println(current_uid);
    Serial.println("LOCATION: COW AT DIP");
    Serial.println("");
  /*
     if (current_uid != previous_uid){
    Serial.println(current_uid);
    Serial.println("");
    Serial.println("LOCATION: COW AT DIP");
    previous_uid = current_uid;
   
  }*/
   delay(800);

   epoch = timeInEpoch();
    String time = "\"time\":\"" + String(epoch) + "000" + "\"";

    String url = "https://alex-farm.firebaseio.com/dipArea.json";

   String location = "\"DIP\"";

    String readerTwo = "\"Reader 2\"";

    String data = "\"location\":" + String(location) + ", \"capturedTag\":" + String(current_uid) + ", \"Reader\":" + String(readerTwo);

    String jsonData = "{" + time + "," + data + "}";
    
    Serial.println(jsonData);

    Process p;
    p.runShellCommand("curl -k -X POST " + url + " -d '" + jsonData + "'");
    while(p.running());
       
} 
}

// Synchronize clock using NTP
void setClock() {  
  Process p;
  
  Serial.println("Setting clock.");
  
  // Sync clock with NTP
  p.runShellCommand("ntpd -nqp 0.openwrt.pool.ntp.org");
  
  // Block until clock sync is completed - 1433666585 is unix time for 7.6.2015 08:43:05 GMT
  while(timeInEpoch() < 1433666585);
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
   
 
