const int buttonPin = 2;  // 按鈕接 D2
const int ledPin = 3;     // LED 燈接 D3 (承接第四題)

int buttonState = 0;      // 當前按鈕狀態
bool buttonPressed = false; // 防連點標記

int state = 0;            // 狀態計數器：0=衡亮, 1=慢閃, 2=中閃, 3=快閃

unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // 初始狀態：衡亮
}

void loop() {
  // 1. 按鈕狀態檢測與狀態切換
  buttonState = digitalRead(buttonPin);

  // 當按下按鈕且之前未觸發時，狀態 +1
  if (buttonState == HIGH && !buttonPressed) {
    state++;
    if (state > 3) {
      state = 0; // 按到第4下回到 0 (衡亮)
    }
    buttonPressed = true;
  }
  
  // 放開按鈕後重置觸發標記
  if (buttonState == LOW && buttonPressed) {
    buttonPressed = false;
  }

  // 2. 依據當前狀態控制 LED 輸出
  unsigned long currentMillis = millis();

  if (state == 0) {
    // 狀態 0: 衡亮
    digitalWrite(ledPin, HIGH);

  } else if (state == 1) {
    // 狀態 1: 慢閃爍 (間隔 500ms)
    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }

  } else if (state == 2) {
    // 狀態 2: 不快不慢閃爍 (間隔 200ms)
    if (currentMillis - previousMillis >= 200) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }

  } else if (state == 3) {
    // 狀態 3: 快閃爍 (間隔 80ms)
    if (currentMillis - previousMillis >= 80) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
  }
}