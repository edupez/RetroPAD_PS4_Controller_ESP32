#include <Bluepad32.h>
ControllerPtr myController;
// Estados anteriores (para detectar mudança)
bool lastA = false, lastB = false, lastC = false, lastStart = false;
bool lastUp = false, lastDown = false, lastLeft = false, lastRight = false;
// Definição dos pinos para o controle Mega Drive (3 botões)
#define PIN_SELECT 23
#define PIN_UP 33
#define PIN_DOWN 25
#define PIN_LEFT 27
#define PIN_RIGHT 32
#define PIN_A 22
#define PIN_B 26
#define PIN_C 18
#define PIN_START 17
#define LEDLED 14 // led

void setupPins() {
  pinMode(PIN_SELECT, OUTPUT);
  pinMode(PIN_UP, OUTPUT);
  pinMode(PIN_DOWN, OUTPUT);
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_START, OUTPUT);
  pinMode(LEDLED, OUTPUT);

  digitalWrite(PIN_SELECT, LOW);
  digitalWrite(PIN_UP, LOW);
  digitalWrite(PIN_DOWN, LOW);
  digitalWrite(PIN_LEFT, LOW);
  digitalWrite(PIN_RIGHT, LOW);
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_START, LOW);
  digitalWrite(LEDLED, LOW);
}

void onConnectedController(ControllerPtr ctl) {
  myController = ctl;
  Serial.println("Controle conectado!");
}

void onDisconnectedController(ControllerPtr ctl) {
  if (myController == ctl)
    myController = nullptr;
  Serial.println("Controle desconectado!");
}

void setup() {
  Serial.begin(115200);
  setupPins();

  BP32.setup(onConnectedController, onDisconnectedController);
  BP32.enableNewBluetoothConnections(true);

  Serial.println("Pronto para parear! Aperte SHARE + PS no controle PS4.");
}

void loop() {
  BP32.update();

  if (myController && myController->isConnected()) {

    int ax = myController->axisX();
    int ay = myController->axisY();
    int hat = myController->dpad();

    // D-PAD digital
    bool up_dpad = (hat == 1 || hat == 5 || hat == 9);
    bool right_dpad = (hat == 4 || hat == 5 || hat == 6);
    bool down_dpad = (hat == 2 || hat == 6 || hat == 10);
    bool left_dpad = (hat == 8 || hat == 9 || hat == 10);

    // ANALÓGICO (opcional)
    bool up_analog = ay < -200;
    bool down_analog = ay > 200;
    bool left_analog = ax < -200;
    bool right_analog = ax > 200;

    // Usa D-PAD + ANALÓGICO juntos:
    bool up = up_dpad || up_analog;
    bool down = down_dpad || down_analog;
    bool left = left_dpad || left_analog;
    bool right = right_dpad || right_analog;

    bool btnA = myController->b();
    bool btnB = myController->x();
    bool btnC = myController->a();
    bool btnD = myController->y();
    bool btnStart = (myController->miscButtons() & 0x04);
    bool btnPS = (myController->miscButtons() & 0x01);
    bool btnHome = (myController->miscButtons() & 0x02);
    bool btnTouchpad = (myController->miscButtons() & 0x08);
    
    bool btnL1 = myController->l1();
    bool btnL2 = myController->l2();
    bool btnR1 = myController->r1();
    bool btnR2 = myController->r2();


    // Atualiza 
    digitalWrite(PIN_UP, up ? HIGH : LOW);
    digitalWrite(PIN_DOWN, down ? HIGH : LOW);
    digitalWrite(PIN_LEFT, left ? HIGH : LOW);
    digitalWrite(PIN_RIGHT, right ? HIGH : LOW);
    digitalWrite(PIN_A, btnA || btnR1 ? HIGH : LOW);
    digitalWrite(PIN_B, btnB || btnL1 ? HIGH : LOW);
    digitalWrite(PIN_C, btnC || btnR2 || btnL2 ? HIGH : LOW);
    digitalWrite(PIN_START, btnHome || btnPS ? HIGH : LOW);
    digitalWrite(PIN_SELECT, btnStart ? HIGH : LOW);

    bool qualquerBotao = btnR1 || btnR2 || btnL1 || btnL2 || btnTouchpad || up || down || left || right || btnA || btnB || btnC || btnD || btnStart || btnHome || btnPS;

    if (qualquerBotao) {
      digitalWrite(LEDLED, LOW);
    } else {
      digitalWrite(LEDLED, HIGH);
    }


  } else {
    digitalWrite(LEDLED, HIGH);
    delay(80);
    digitalWrite(LEDLED, LOW);
    delay(80);
  }

  delay(3);
}
