const int buttonPin = 2;  // the number of the pushbutton pin
const int RledPin = 3;    // the number of the R pin (PWM)
const int GledPin = 6;    // the number of the G pin (PWM) -> 改接至 D6
const int BledPin = 5;    // the number of the B pin (PWM)

// variables will change:
int buttonState = 0;      // variable for reading the pushbutton status
int ledcolor = 0;
bool ButtonPressed = false;

void setup() {
  // initialize the LED pin as an output:
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // 初始化先關閉 LED（共陽極：255 為完全熄滅）
  analogWrite(RledPin, 255);
  analogWrite(GledPin, 255);
  analogWrite(BledPin, 255);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // 按鈕按下判斷（搭配 ButtonPressed 避免連續跳色）
  if (buttonState == HIGH && !ButtonPressed) {
    ledcolor = ledcolor + 1;
    ButtonPressed = true;
  }
  if (buttonState == LOW && ButtonPressed) {
    ButtonPressed = false;
  }

  // --- 根據 ledcolor 切換不同相似色的漸層效果 ---

  if (ledcolor == 0) {
    // 全關 (LED OFF)
    analogWrite(RledPin, 255);
    analogWrite(GledPin, 255);
    analogWrite(BledPin, 255);

  } else if (ledcolor == 1) {
    // 綠 ⇄ 藍 漸變 (Green <-> Blue)
    // 1. 從綠色漸變成藍色
    for (int i = 0; i <= 255; i++) {
      if (digitalRead(buttonPin) == HIGH) break; 
      analogWrite(RledPin, 255);        
      analogWrite(GledPin, i);          
      analogWrite(BledPin, 255 - i);   
      delay(5);
    }
    // 2. 從藍色漸變回綠色
    for (int i = 0; i <= 255; i++) {
      if (digitalRead(buttonPin) == HIGH) break;
      analogWrite(RledPin, 255);        // R 關閉
      analogWrite(GledPin, 255 - i);    // G 逐漸變亮
      analogWrite(BledPin, i);          // B 逐漸變暗
      delay(5);
    }

  } else if (ledcolor == 2) {
    // 紅 ⇄ 紫 漸變 (Red <-> Purple)
    // 1. 從紅色漸變成紫色
    for (int i = 0; i <= 255; i++) {
      if (digitalRead(buttonPin) == HIGH) break;
      analogWrite(RledPin, 0);          // R 全亮
      analogWrite(GledPin, 255);        // G 關閉
      analogWrite(BledPin, 255 - i);    // B 逐漸加入，形成紫色
      delay(5);
    }
    // 2. 從紫色漸變回紅色
    for (int i = 0; i <= 255; i++) {
      if (digitalRead(buttonPin) == HIGH) break;
      analogWrite(RledPin, 0);          // R 全亮
      analogWrite(GledPin, 255);        // G 關閉
      analogWrite(BledPin, i);          // B 逐漸減少，回到紅色
      delay(5);
    }

  } else if (ledcolor == 3) {
    // 黃 ⇄ 橙 漸變 (Yellow <-> Orange)
    // 1. 從黃色漸變成橙色
    for (int i = 0; i <= 155; i++) {
      if (digitalRead(buttonPin) == HIGH) break;
      analogWrite(RledPin, 0);          
      analogWrite(GledPin, i);          
      analogWrite(BledPin, 255);        
      delay(8);
    }
    // 2. 從橙色漸變回黃色
    for (int i = 155; i >= 0; i--) {
      if (digitalRead(buttonPin) == HIGH) break;
      analogWrite(RledPin, 0);          
      analogWrite(GledPin, i);         
      analogWrite(BledPin, 255);    
      delay(8);
    }

  } else if (ledcolor == 4) {
    ledcolor = 0;
  }
}