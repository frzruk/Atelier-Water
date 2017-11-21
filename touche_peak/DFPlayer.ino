void playFiles(int mV, int mP, int maxValLT, int maxValGT, int maxPosGT, int maxPosLT, int mp3File, int mp3Length)
{
  if (((mV < maxValLT) && (mV > maxValGT)) && ((mP > maxPosGT) && (mP < maxPosLT)))
  {
    checkVal = false;
    myDFPlayer.play(mp3File);
    delay(mp3Length);
    checkVal = true;
  }
}

void mp3setup ()
{
    mySoftwareSerial.begin(9600); // Start the serial connection to the software on the MP3 Controller
  Serial.begin(115200); // Start the serial connection for the Arduino to COM on the PC
  Serial.println();
  Serial.println(F("Initializing DFPlayer... may take a couple of secs"));

  // Check if the mp3 controller is connected and output an error message to serial monitor
  if(!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online"));
  myDFPlayer.enableLoop();
  myDFPlayer.volume(30);  // Set the mp3 controller volume
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
