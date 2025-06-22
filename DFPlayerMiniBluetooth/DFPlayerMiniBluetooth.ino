/**
  * Version: DFPlayerMiniBluetooth 0.2
  * Project instructions: https://www.infotronikblog.com/2025/04/dfplayer-mini-bluetooth-2025.html
  * Project repository:  https://github.com/Peyutron/DFPlayerMiniBluetooth
  * DFPlayer Mini BT PlayStore https://play.google.com/store/apps/details?id=com.infotronikblog.dfplayerminibt
  * web: https://www.infotronikblog.com
  * Creator: Carlos MC
  *
  * External Library:
  * - DFRobotDFPlayerMini https://github.com/DFRobot/DFRobotDFPlayerMini/tree/master
  * ESP32 Library:
  * - BluetoothSerial https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/src/BluetoothSerial.h
  * Board -> DOIT ESP32 DEVKIT V1
  * - Flash frecuency 80Mhz
  
  * ┌────────────────────────┐          ┌───────────────────────┐  
  * │        ESP32           │          │     DFPlayer Mini     │
  * ├────────────────────────┤          ├───────────────────────┤  
  * │                        │          │                       │  
  * │  GPIO16 (TX)  ───R1kΩ──────▶  RX  │                       │ 
  * │  GPIO17 (RX)  ◀────────┬────  TX  │                       │
  * │                        │          │                       │  
  * │  GPIO19       ◀────────┘──── BUSY │                       │
  * │                        │          │                       │  
  * │  5V          ─────────────▶  VCC  │                       │
  * │  GND         ─────────────▶  GND  │                       │
  * └────────────────────────┘          └───────────────────────┘  

**/


#include <BluetoothSerial.h>
#include "DFRobotDFPlayerMini.h"

BluetoothSerial SerialBT;
DFRobotDFPlayerMini myDFPlayer;

// Pines UART
#define RXD2 16
#define TXD2 17
#define BUSY 19

void printDetail(uint8_t type, int value);

// Variables para almacenar estado
int currentVolume = 15;
int currentEQ = 0;
int currentDevice = 2; // 1-USB, 2-SD, 3-Sleep, 4-Serial
int currentTrack = 0; 
int currentFolder = 1;
int isLoopActivated = -1;
bool busyActivated = false;
bool next_song = true;
bool play_state = false;
unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 500;  // interval at which to blink (milliseconds)



void setup()
{

  Serial.begin(115200);
  
  // Inicializar UART para el DFPlayer
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // Inicializar la conexión Bluetooth
  SerialBT.begin("ESP32_MP3"); // Nombre del dispositivo Bluetooth

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  pinMode(BUSY, INPUT_PULLUP);

  if (!myDFPlayer.begin(Serial2, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(20);  //Set volume value (0~30).
  // myDFPlayer.volumeUp(); //Volume Up
  // myDFPlayer.volumeDown(); //Volume Down
  
  myDFPlayer.enableLoop();

  //----Set different EQ----
   myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  // myDFPlayer.EQ(DFPLAYER_EQ_POP);
  // myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  // myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  // myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  // myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
  
  //----Mp3 control----
//  myDFPlayer.sleep();     //sleep
//  myDFPlayer.reset();     //Reset the module
//  myDFPlayer.enableDAC();  //Enable On-chip DAC
//  myDFPlayer.disableDAC();  //Disable On-chip DAC
//  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15
  
/*  //----Mp3 play----
  myDFPlayer.next();  //Play next mp3
  delay(1000);
  myDFPlayer.previous();  //Play previous mp3
  delay(1000);
  myDFPlayer.play(1);  //Play the first mp3
  delay(1000);
  myDFPlayer.loop(1);  //Loop the first mp3
  delay(1000);
  myDFPlayer.pause();  //pause the mp3
  delay(1000);
  myDFPlayer.start();  //start the mp3 from the pause
  delay(1000);
  myDFPlayer.enableLoop(); //enable loop.
  delay(1000);
  myDFPlayer.disableLoop(); //disable loop.
  delay(1000);
  myDFPlayer.enableLoopAll(); //loop all mp3 files.
  delay(1000);
  myDFPlayer.disableLoopAll(); //stop loop all mp3 files.
  delay(1000);
  myDFPlayer.playMp3Folder(4); //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
  delay(1000);
  myDFPlayer.playFolder(15, 4);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
  delay(1000);
  myDFPlayer.playLargeFolder(2, 999); //play specific mp3 in SD:/02/004.mp3; Folder Name(1~10); File Name(1~1000)
  delay(1000);
  myDFPlayer.loopFolder(5); //loop all mp3 files in folder SD:/05.
  delay(1000);
  myDFPlayer.randomAll(); //Random play all the mp3.
  delay(1000);
  myDFPlayer.advertise(3); //advertise specific mp3 in SD:/ADVERT/0003.mp3; File Name(0~65535)
  delay(1000);
  myDFPlayer.stopAdvertise(); //stop advertise
  delay(1000);
*/
  //----Read imformation----
  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.println(myDFPlayer.readVolume()); //read current volume
  Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
  Serial.println(myDFPlayer.readFileCountsInFolder(0)); //read file counts in folder SD:/03
}

void loop()
{
  busyActivated = digitalRead(BUSY);
  unsigned long currentMillis = millis();    
  if (currentMillis - previousMillis >= interval) 
    {
      previousMillis = currentMillis;
      if(busyActivated == HIGH && next_song && play_state)
      {
        SerialBT.println(processCommand("pn"));
        delay(10);
      }
    }

  if (SerialBT.available()) 
  {
    uint8_t buffer[128];
    // Lee hasta encontrar el delimitador '\n' 
    int bytesRecibidos = SerialBT.readBytesUntil('\n', buffer, sizeof(buffer));
    
    String cmd = String((char*)buffer).substring(0, bytesRecibidos);
    cmd.trim(); // Limpia saltos de línea y espacios
    
    String command = ClearString(cmd);
    if (command != NULL && !command.equals(""))
    {

      // Serial.println("btReceived: " + command);
      SerialBT.println(processCommand(command));
      // Serial.println("btSend: " + processCommand(command));
    }
  }
}


String processCommand(String cmd) 
{


  // Comandos de Reproducción
  if (cmd.startsWith("pp")) // PLAY
  {
    Serial.println("PLAY");

    play_state = true;
    currentTrack = cmd.substring(2).toInt();
    myDFPlayer.play(currentTrack);
    return "pp" + String(currentTrack);
  
  } 
  else if (cmd.startsWith("px")) // PAUSE
  {
    play_state = false;
    myDFPlayer.pause();
    return "px";
  } 
  else if (cmd.startsWith("py")) // Resume
  {
    play_state = true;
    myDFPlayer.start();
    return "py";
  }
  else if (cmd.startsWith("ps")) // STOP
  {
    play_state = false;
    myDFPlayer.stop();
    return "ps";
  } 
  else if (cmd.startsWith("pn")) // NEXT
  {
    myDFPlayer.next();
    delay(100);
    // currentTrack = WaitForModuleResponse(4);
    currentTrack += 1;
    return "pn" + (String)currentTrack;
  } 
  else if (cmd.startsWith("pb")) // PREVIOUS
  { 
    myDFPlayer.previous();
    // currentTrack = WaitForModuleResponse(4);
    if (currentTrack >= 0) currentTrack -= 1;
    return "pb" + (String)currentTrack;;

  // Equalización
  } 
  else if (cmd.startsWith("e")) // EQUALIZER
  {
    currentEQ = cmd.substring(1).toInt();
    myDFPlayer.EQ(currentEQ);
    delay(50);
    return "e" + String(currentEQ);

  // Volumen
  } 
  else if (cmd.startsWith("v")) // VOLUME
  {
    currentVolume = cmd.substring(1).toInt();
    myDFPlayer.volume(currentVolume);
    return "v" + String(currentVolume);

  // Configuración
  } 
  else if (cmd.startsWith("d")) // CURRENT DEVICE
  {
    currentDevice = cmd.substring(1).toInt();
    myDFPlayer.outputDevice(currentDevice);
    return "d" + String(currentDevice);  
  }
  // Consultas de Estado 
  else if (cmd.startsWith("r")) // READ
  {
    String datas = "";
    Serial.println(cmd.substring(1));
    switch (cmd.substring(1).toInt())
    {
      case 0: // readState
        return "r0" + (String)WaitForModuleResponse(0);
      case 1: // readVolume
        return "r1" + (String)WaitForModuleResponse(1);
      case 2: // readEQ
        return "r2" + (String)WaitForModuleResponse(2);      
      case 3: // readFileCounts()); //read all file counts in SD card
        return "r3" + (String)WaitForModuleResponse(3);
      case 4: // readCurrentFileNumber()); //read current play file number
        return "r4" + (String)WaitForModuleResponse(4);
      case 5: // readFileCountsInFolder(3)); //read file counts in folder 
        return "r5" + (String)WaitForModuleResponse(5);
      default:
        return "read r error";
    }    
  }  
  else if (cmd.startsWith("o")) // OPTIONS
  {
    delay(100); // Necesary for response
    String datas = "";
    Serial.print("Options: ");
    Serial.println(cmd.substring(1));
    switch (cmd.substring(1).toInt())
    {
      case 0: // Enable DAC
        myDFPlayer.enableDAC();  // Enable On-chip DAC
        return "o0" ;
      case 1: // Disable DAC
        myDFPlayer.disableDAC(); // Disable On-chip DAC
        return "o1";
      case 2: // Sleep
        myDFPlayer.sleep();     // sleep
        return "o2";
      case 3: // Reset module
        myDFPlayer.reset();     // Reset the module 
        return "o3";
      case 4: // outputSetting(bool enable, uint8_t gain) - not used
        // myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15
        return "o4";
      default:
        return "read o error";
    }    
  } 
    else if (cmd.startsWith("l")) // LOOP
  {
    isLoopActivated = cmd.substring(1).toInt();
    if (isLoopActivated == 0) myDFPlayer.enableLoop();
    else if (isLoopActivated == 1) myDFPlayer.disableLoop();
    else if (isLoopActivated == 2) myDFPlayer.enableLoopAll();
    else if (isLoopActivated == 3) myDFPlayer.disableLoopAll();
    
    return "l" + (String)isLoopActivated;  
  }
    else if (cmd.startsWith("u")) // RANDOM
  {
    myDFPlayer.randomAll();
    return "u";
  }
  else return "Bad arduino command";
  
}

// Cambios en la equalización: 
void eq_master(int eq_value)
{
  myDFPlayer.EQ(eq_value);
  delay(50);
}

int WaitForModuleResponse(uint8_t option)
{ 
  int counter = 0;
  int response = -1;
  while (response == -1)
  {
    delay(100);
    switch (option) 
    {
      case 0:
        response = myDFPlayer.readState();
        break;
      case 1:
        response = myDFPlayer.readVolume();
        break;
      case 2:
        response = myDFPlayer.readEQ();
        break;
      case 3:
        response = myDFPlayer.readFileCounts(2);
        break;
      case 4:
        response = myDFPlayer.readCurrentFileNumber(2);
        break;
      case 5:
        response = myDFPlayer.readFileCountsInFolder(currentFolder);
        break;
    }
    counter++;
    if (counter == 5)response = -1;
  }
  return response;
}

String ClearString(String input) {
  String output;
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    if (c >= 32 && c <= 126) { // Filtra ASCII imprimible
      output += c;
    }
  }
  return output;
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

