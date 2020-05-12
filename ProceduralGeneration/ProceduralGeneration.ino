#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SSD1306_LCDHEIGHT 64

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int simState = 0;
int settingsState = 0;

const int mW = 32;
const int mH = 16;

typedef struct{
  bool m[mW][mH];
}Map;

typedef struct{
  int state, prev, pin;
}Button;

Button b1, b2, b3;

float iChance = 0;
int bLimit = 0;
int dLimit = 0;
int nSteps = 0;

int seed;

Map gMap;

void setup() {
  Serial.begin(9600);

  //Display Setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);  

  b1.pin = 5;
  b2.pin = 6;
  b3.pin = 9;

  //Pin Setup
  pinMode(b1.pin, INPUT_PULLUP);
  pinMode(b2.pin, INPUT_PULLUP);
  pinMode(b3.pin, INPUT_PULLUP);
}

void MenuHandler(){
  CheckButtons();
  
  display.clearDisplay();
  
  if(IsPressed(b1) || IsPressed(b2) || IsPressed(b3)){
    simState++;
  }

  display.setCursor(0,0);
  display.println("Procedural Generation");
  display.setCursor(90,16);
  display.println("(+) ->");
  display.setCursor(90,32);
  display.println("(-) ->");
  display.setCursor(90,48);
  display.println("(x) ->");
  display.display();
}

void ResetState(Button &b){
  if(b.state == 1 && b.prev == 1){
    b.prev = 0;
  }
}

bool IsPressed(Button &b){
  if(b.state != 1 && b.prev == 0){
     b.prev = 1;
    return true; 
  }
  return false;
}

void CheckButtons(){
  b1.state = digitalRead(b1.pin);
  ResetState(b1);
  b2.state = digitalRead(b2.pin);
  ResetState(b2);
  b3.state = digitalRead(b3.pin);
  ResetState(b3);
}

int countNeighbors(Map map, int x, int y){
    int count = 0;
    for(int i=-1; i<2; i++){
        for(int j=-1; j<2; j++){
            int neighbor_x = x+i;
            int neighbor_y = y+j;
            //If we're looking at the middle point
            if(i == 0 && j == 0){
                //Do nothing, we don't want to add ourselves in!
            }
            //In case the index we're looking at it off the edge of the map
            else if(neighbor_x < 0 || neighbor_y < 0 || neighbor_x >= mW || neighbor_y >= mH){
                count = count + 1;
            }
            //Otherwise, a normal check of the neighbour
            else if(map.m[neighbor_x][neighbor_y]){
                count = count + 1;
            }
        }
    }
    return count;
}

void SimulateStep(){
    Map oldMap = gMap;
    for(int x=0; x<mW; x++){
        for(int y=0; y<mH; y++){
            int nbs = countNeighbors(oldMap, x, y);
            //The new value is based on our simulation rules
            //First, if a cell is alive but has too few neighbours, kill it.
            if(oldMap.m[x][y]){
                if(nbs < dLimit){
                    gMap.m[x][y] = false;
                }
                else{
                    gMap.m[x][y] = true;
                }
            } //Otherwise, if the cell is dead now, check if it has the right number of neighbours to be 'born'
            else{
                if(nbs > bLimit){
                    gMap.m[x][y] = true;
                }
                else{
                    gMap.m[x][y] = false;
                }
            }
        }
    }
}

void GenerateMap(){
  srand(millis());
  seed = rand();
  for(int i = 0; i < mW; i++){
    for(int j = 0; j < mH; j++){
      if(((rand() % 100) + 1) < (iChance * 100)){
        gMap.m[i][j] = true;
      }
      else{
        gMap.m[i][j] = false;
      }

      float percent =((float)(i*mH)+(j+1))/(((float)(mH*mW))/100.0f);
      display.clearDisplay();
      display.setCursor(22,20);
      display.println("Generating Map");
      
      display.setCursor(43,30);
      display.print(percent);
      display.println("%");
      
      display.display();
    }
  }

  for(int i = 0; i < nSteps; i++){
    SimulateStep();
  }
}

void SettingsInHandler(){
  CheckButtons();

  display.clearDisplay();
  
  if(settingsState == 0){
      if(IsPressed(b1)){
        iChance += 0.1f;
        if(iChance > 0.9f){
          iChance = 0.9f;
        }
      }
      if(IsPressed(b2)){
        iChance -= 0.1f;
        if(iChance < 0){
          iChance = 0;
        }
      }
      if(IsPressed(b3)){
        settingsState++;
      }
      display.setCursor(5,20);
      display.println((String)"Inital Chance: "+iChance);
  }
  else if(settingsState == 1){
      if(IsPressed(b1)){
        bLimit++;
      }
      if(IsPressed(b2)){
        bLimit--;
      }
      if(IsPressed(b3)){
        settingsState++;
      }
      display.setCursor(15,20);
      display.println((String)"Birth Limit: "+bLimit);
  }
  else if(settingsState == 2){
      if(IsPressed(b1)){
        dLimit++;
      }
      if(IsPressed(b2)){
        dLimit--;
      }
      if(IsPressed(b3)){
        settingsState++;
      }
      display.setCursor(15,20);
      display.println((String)"Death Limit: "+dLimit);
  }
  else if(settingsState == 3){
      if(IsPressed(b1)){
        nSteps++;
      }
      if(IsPressed(b2)){
        nSteps--;
      }
      if(IsPressed(b3)){
        settingsState++;
      }
      display.setCursor(10,20);
      display.println((String)"Number of Steps: "+ nSteps);
  }
  else if(settingsState == 4){
      simState++;
      settingsState = 0;
      GenerateMap();
  }

  display.display();
}

void ViewHandler(){
  display.clearDisplay();
  for(int i = 0; i < mW; i++){
    for(int j = 0; j < mH; j++){
      if(gMap.m[i][j] == true){
        display.drawRect(i*(128/mW), j*(64/mH), 128/mW, 64/mH, SSD1306_WHITE);
      }
    }
  }
  
  CheckButtons();
  if(IsPressed(b1) || IsPressed(b2) || IsPressed(b3)){
    simState++;
  }
  
  display.display();
}

void SeedViewer(){
  display.clearDisplay();
  display.setCursor(45,0);
  display.println("Seed");
  display.setCursor(10,25);
  display.setTextSize(1);
  display.print(seed);
  display.print(".");
  display.print(iChance*100);
  display.print(".");
  display.print(bLimit);
  display.print(".");
  display.print(dLimit);
  display.print(".");
  display.println (nSteps);
  display.display();
  
  CheckButtons();
  if(IsPressed(b1) || IsPressed(b2) || IsPressed(b3)){
    simState = 0;
    iChance = 0;
    bLimit = 0;
    dLimit = 0;
    nSteps = 0;
  }
}

void loop() {
  if(simState == 0){
    //Menu
    MenuHandler();
  }
  else if(simState == 1){
    //Setup
    SettingsInHandler();
  }
  else if(simState == 2){
    //View
    ViewHandler();
  }
  else if(simState == 3){
    SeedViewer();
  }
}
