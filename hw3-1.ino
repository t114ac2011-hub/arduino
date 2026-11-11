const int RledPin = 3;  // PWM 腳位
const int GledPin = 6;  // PWM 腳位
const int BledPin = 5;  // PWM 腳位

// 模式狀態：0 = 常亮/靜態模式, 1 = 慢速閃爍, 2 = 中速閃爍, 3 = 極速爆閃
int mode = 0; 

int currentR = 255;
int currentG = 0;
int currentB = 0;

unsigned long previousMillis = 0;
long blinkInterval = 300; // 閃爍間隔時間
bool ledOn = true;

void setup() {
  Serial.begin(9600);
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
  
  randomSeed(analogRead(A1));
  setRGB(currentR, currentG, currentB);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "RANDOM") {
      // 隨機選色：僅更新顏色變數，不修改 mode (維持當前閃爍狀態)
      currentR = random(0, 256);
      currentG = random(0, 256);
      currentB = random(0, 256);
      if (mode == 0) {
        setRGB(currentR, currentG, currentB);
      }

    } else if (input == "STEADY") {
      // 若網頁傳入靜態模式指令
      mode = 0;
      setRGB(currentR, currentG, currentB);

    } else if (input == "BLINK1") {
      mode = 1;
      blinkInterval = 500; // 慢速閃爍 (1 Hz)
    } else if (input == "BLINK2") {
      mode = 2;
      blinkInterval = 200; // 中速閃爍 (2.5 Hz)
    } else if (input == "BLINK3") {
      mode = 3;
      blinkInterval = 100; // 極速爆閃 (5 Hz)

    } else {
      // 解析 RGB 格式 (R,G,B)
      int firstComma = input.indexOf(',');
      int secondComma = input.indexOf(',', firstComma + 1);

      if (firstComma > 0 && secondComma > firstComma) {
        // 調色盤選色：僅更新顏色變數，不修改 mode
        currentR = input.substring(0, firstComma).toInt();
        currentG = input.substring(firstComma + 1, secondComma).toInt();
        currentB = input.substring(secondComma + 1).toInt();
        
        // 如果目前是常亮模式，立刻更新顯示顏色
        if (mode == 0) {
          setRGB(currentR, currentG, currentB);
        }
      }
    }
  }

  // 閃爍控制邏輯 (非阻塞式 millis 寫法)
  if (mode >= 1 && mode <= 3) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      ledOn = !ledOn;
      
      if (ledOn) {
        setRGB(currentR, currentG, currentB);
      } else {
        setRGB(0, 0, 0); // 熄滅
      }
    }
  }
}

// 共陽極控制邏輯
void setRGB(int r, int g, int b) {
  analogWrite(RledPin, 255 - r);
  analogWrite(GledPin, 255 - g);
  analogWrite(BledPin, 255 - b);
}