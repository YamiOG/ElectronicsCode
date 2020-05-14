#include <Keyboard.h>

byte rows[] = {4, 12, 6};
const int rowCount = sizeof(rows) / sizeof(rows[0]);

byte cols[] = {2, 0};
const int colCount = sizeof(cols) / sizeof(cols[0]);

byte keys[colCount][rowCount];
bool prevState[colCount][rowCount];

//Key Functions
int key0[] = {KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_LEFT_SHIFT, 'a'};
int key1[] = {KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_LEFT_SHIFT, 'b'};
int key2[] = {KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_LEFT_SHIFT, 'c'};
int key3[] = {KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_LEFT_SHIFT, 'd'};
int key4[] = {KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_LEFT_SHIFT, 'e'};
int key5[] = {KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_LEFT_SHIFT, 'g'};

int key0Count = sizeof(key0) / sizeof(key0[0]);
int key1Count = sizeof(key1) / sizeof(key0[1]);
int key2Count = sizeof(key2) / sizeof(key0[2]);
int key3Count = sizeof(key3) / sizeof(key0[3]);
int key4Count = sizeof(key4) / sizeof(key0[4]);
int key5Count = sizeof(key5) / sizeof(key0[5]);

void KeyboardOutput(int colNum, int rowNum, bool enable)
{
  switch (colNum)
  {
    case 0:
      if (rowNum == 0) {
        if(enable) {
          for(int i = 0; i < key0Count; i++){ 
            Keyboard.press(key0[i]);
          }
        }
        else{
          for(int i = 0; i < key0Count; i++){ 
            Keyboard.release(key0[i]);
          }
        }
      }
      if (rowNum == 1) {
        if(enable) {
          for(int i = 0; i < key2Count; i++){ 
            Keyboard.press(key2[i]);
          }
        }
        else{
          for(int i = 0; i < key2Count; i++){ 
            Keyboard.release(key2[i]);
          }
        }
      }
      if (rowNum == 2) {
        if(enable) {
          for(int i = 0; i < key4Count; i++){ 
            Keyboard.press(key4[i]);
          }
        }
        else{
          for(int i = 0; i < key4Count; i++){ 
            Keyboard.release(key4[i]);
          }
        }
      }
      break;
    case 1:
      if (rowNum == 0) {
        if(enable) {
          for(int i = 0; i < key1Count; i++){ 
            Keyboard.press(key1[i]);
          }
        }
        else{
          for(int i = 0; i < key1Count; i++){ 
            Keyboard.release(key1[i]);
          }
        }
      }
      if (rowNum == 1) {
        if(enable) {
          for(int i = 0; i < key3Count; i++){ 
            Keyboard.press(key3[i]);
          }
        }
        else{
          for(int i = 0; i < key3Count; i++){ 
            Keyboard.release(key3[i]);
          }
        }
      }
      if (rowNum == 2) {
        if(enable) {
          for(int i = 0; i < key5Count; i++){ 
            Keyboard.press(key5[i]);
          }
        }
        else{
          for(int i = 0; i < key5Count; i++){ 
            Keyboard.release(key5[i]);
          }
        }
      }
      break;
  }
  //delay(100);
}

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  for (int x = 0; x < rowCount; x++) {
    pinMode(rows[x], INPUT);
  }

  for (int x = 0; x < colCount; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }

}

void readMatrix() {
  // iterate the columns
  for (int colIndex = 0; colIndex < colCount; colIndex++) {
    // col: set to output to low
    byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
    digitalWrite(curCol, LOW);

    // row: interate through the rows
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
      byte rowCol = rows[rowIndex];
      pinMode(rowCol, INPUT_PULLUP);
      keys[colIndex][rowIndex] = digitalRead(rowCol);
      if (digitalRead(rowCol) != 1 && prevState[colIndex][rowIndex] == false) {
        KeyboardOutput(colIndex, rowIndex, true);
        prevState[colIndex][rowIndex] = true;
      }
      if (digitalRead(rowCol) != 0 && prevState[colIndex][rowIndex]) {
        KeyboardOutput(colIndex, rowIndex, false);
        prevState[colIndex][rowIndex] = false;
      }
      pinMode(rowCol, INPUT);
    }
    // disable the column
    pinMode(curCol, INPUT);
  }
}

void loop() {
Serial.println(key0Count);
  readMatrix();
}
