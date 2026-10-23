// 腳位設定
const int buttonPin = 2;     // 按鈕連接到 D2[cite: 8, 10, 12]
const int RLedPin = 9;       // RGB 紅色連接到 D9[cite: 8, 10, 12]
const int GLedPin = 10;      // RGB 綠色連接到 D10[cite: 8, 10]
const int BLedPin = 11;      // RGB 藍色連接到 D11[cite: 8, 10]

// 心情與計時變數
int moodScore = 50;          // 心情初始分數 (0 ~ 100)
int buttonState = 0;         // 目前按鈕狀態
bool buttonPressed = false;  // 按鈕是否被按下的標記

unsigned long lastPetTime = 0;       // 上次觸碰娃娃的時間
unsigned long lastDecayTime = 0;     // 上次扣分的時間
const int untouchedInterval = 3000;  // 超過 3 秒未觸碰開始扣分
const int decayRate = 1000;          // 每 1 秒扣一次分數

// 紀錄當前 RGB LED 的顏色數值（用於漸變計算）
int currentR = 255, currentG = 255, currentB = 0; // 初始為黃光 (50分)

void setup() {
    pinMode(buttonPin, INPUT);
    pinMode(RLedPin, OUTPUT);
    pinMode(GLedPin, OUTPUT);
    pinMode(BLedPin, OUTPUT);

    // 初始燈光設定為黃光
    analogWrite(RLedPin, currentR);
    analogWrite(GLedPin, currentG);
    analogWrite(BLedPin, currentB);

    unsigned long currentTime = millis();
    lastPetTime = currentTime;
    lastDecayTime = currentTime;
}

void loop() {
    unsigned long currentTime = millis();

    // 1. 偵測按壓娃娃 (按鈕)[cite: 11]
    buttonState = digitalRead(buttonPin);

    // 觸發按壓 (High 狀態)
    if (buttonState == HIGH && !buttonPressed) {
        moodScore = moodScore + 15;  // 撫摸一次增加 15 分
        if (moodScore > 100) {
            moodScore = 100;         // 滿分為 100
        }
        lastPetTime = currentTime;   // 重置最後撫摸時間
        buttonPressed = true;
    }

    if (buttonState == LOW) {
        buttonPressed = false;
    }

    // 2. 判斷長時間未觸碰 (超過 3 秒未按壓，會讓娃娃傷心並扣分)[cite: 11]
    if (currentTime - lastPetTime > untouchedInterval) {
        if (currentTime - lastDecayTime > decayRate) {
            moodScore = moodScore - 5;  // 每秒扣 5 分
            if (moodScore < 0) {
                moodScore = 0;          // 最低為 0 分
            }
            lastDecayTime = currentTime;
        }
    }

    // 3. 根據心情分數漸變更新 RGB LED 顯示[cite: 11]
    updateMoodLED();
}

// 根據分數平滑漸變至目標顏色
void updateMoodLED() {
    int targetR = 0, targetG = 0, targetB = 0;

    if (moodScore >= 70) {
        // 心情開心 (Happy)：目標為綠光 (0, 255, 0)
        targetR = 0;
        targetG = 255;
        targetB = 0;
    } 
    else if (moodScore >= 30) {
        // 心情普通 (Neutral)：目標為黃光 (255, 255, 0)
        targetR = 255;
        targetG = 255;
        targetB = 0;
    } 
    else {
        // 心情傷心 (Sad)：目標為藍光 (0, 0, 255)
        targetR = 0;
        targetG = 0;
        targetB = 255;
    }

    // 執行平滑漸變 (Fade to Target Color)
    fadeToColor(targetR, targetG, targetB);
}

// 實現顏色漸變變化的自訂函式
void fadeToColor(int targetR, int targetG, int targetB) {
    // 如果當前顏色已經等於目標顏色，就不需要漸變
    if (currentR == targetR && currentG == targetG && currentB == targetB) {
        return;
    }

    // 每次呼叫向目標顏色微調一步 (+1 或 -1)
    if (currentR < targetR) currentR++;
    else if (currentR > targetR) currentR--;

    if (currentG < targetG) currentG++;
    else if (currentG > targetG) currentG--;

    if (currentB < targetB) currentB++;
    else if (currentB > targetB) currentB--;

    // 寫入燈光顏色
    analogWrite(RLedPin, currentR);
    analogWrite(GLedPin, currentG);
    analogWrite(BLedPin, currentB);

    // 稍微延遲（控制漸變速度，數字越大漸變越慢）
    delay(5); 
}