#include <Arduino.h>

#include <Servo.h>  // Inclui a biblioteca Servo para controlar servos
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Configurações do display LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço I2C do LCD

// Configurações do sensor DHT
#define DHTPIN 4  // Pino do sensor DHT
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pinos dos botões
#define BTN_UP 8
#define BTN_DOWN 7
#define BTN_SELECT 3
#define BTN_BACK 2

// Pinos do relé e LEDs
#define RELE_PIN 10
#define LED_VERMELHO 11
#define LED_AMARELO 12
bool releState = false;
bool ledVermelhoState = false;
bool ledAmareloState = false;
bool backlightState = true;

// Servo
Servo meuServo;   // Cria um objeto Servo para controlar o servo motor
int pos;          // Variável para armazenar a posição do servo motor
int potPin = A0;  // Pino analógico onde o potenciômetro está conectado
int val;          // Variável para armazenar o valor lido do potenciômetro

// Menu
int menuIndex = 0;
const char *menuOptions[] = {
  "1. Pot./Servo",
  "2. Temp./Umidade",
  "3. Rele On/Off",
  "4. Msg no LCD",
  "5. LED Vermelho",
  "6. LED Amarelo",
  "7. Backlight"
};
const int menuSize = sizeof(menuOptions) / sizeof(menuOptions[0]);

// Funções
void showMenu();
void executeOption(int index);


void setup() {
  // Inicializa o monitor serial
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  dht.begin();

  pinMode(RELE_PIN, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  digitalWrite(RELE_PIN, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AMARELO, LOW);

  // Configuração dos botões
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  meuServo.attach(6);  // Associa o servo motor ao pino digital 6 do Arduino
  meuServo.write(0);   // Define a posição inicial do servo motor para 0 graus

  showMenu();
}

void loop() {
  if (!digitalRead(BTN_UP)) {
    menuIndex = (menuIndex - 1 + menuSize) % menuSize;  // Navegar para cima
    delay(200);                                         // Debounce
    showMenu();
  }

  if (!digitalRead(BTN_DOWN)) {
    menuIndex = (menuIndex + 1) % menuSize;  // Navegar para baixo
    delay(200);                              // Debounce
    showMenu();
  }

  if (!digitalRead(BTN_SELECT)) {
    delay(200);  // Debounce
    executeOption(menuIndex);
    showMenu();
  }
}

void showMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-> ");
  lcd.print(menuOptions[menuIndex]);  // Mostra a opção selecionada
                                      //if (menuIndex + 1 < menuSize) { // Mostra a próxima opção, se existir
  lcd.setCursor(0, 1);
  //lcd.print("   ");
  lcd.print(menuOptions[(menuIndex + 1) % menuSize]);
  //}
}

void executeOption(int index) {
  lcd.clear();
  switch (index) {
    case 0:  // Potenciômetro e Servo
      while (true) {
        val = analogRead(potPin);         // Lê o valor do potenciômetro (0 a 1023)
        val = constrain(val, 0, 1023);    // Garante que o valor fique entre 0 e 1023
        pos = map(val, 0, 1023, 0, 180);  // Mapeia o valor do potenciômetro para a faixa de 0 a 180 graus
        meuServo.write(pos);              // Define a posição do servo motor
        Serial.print("Pot: ");
        Serial.print(val);
        Serial.print(" / Pos: ");
        Serial.println(pos);

        lcd.setCursor(0, 0);  // Define o cursor na primeira linha
        lcd.print("Potenc.: ");
        lcd.print("       ");
        lcd.setCursor(9, 0);
        lcd.print(val);       // Exibe o texto formatado
        lcd.setCursor(0, 1);  // Define o cursor na segunda linha
        lcd.print("Pos. Servo: ");
        lcd.print("    ");
        lcd.setCursor(12, 1);
        lcd.print(pos);  // Exibe o texto formatado

        delay(15);  // Aguarda um curto intervalo para estabilizar o movimento do servo

        if (!digitalRead(BTN_BACK)) break;  // Voltar ao menu
      }
      break;

    case 1:  // Temperatura e Umidade
      while (true) {
        float temp = dht.readTemperature();
        float hum = dht.readHumidity();
        if (isnan(temp) || isnan(hum)) {
          Serial.println(F("Failed to read from DHT sensor!")); //Por padrão, strings literais são armazenadas na RAM, mas usando F(), a string é mantida na memória de programa (flash), que geralmente é mais abundante que a RAM no Arduino.
          return;
        }
        lcd.setCursor(0, 0);
        lcd.print("Temp.: ");
        lcd.print("         ");
        lcd.setCursor(7, 0);
        lcd.print(temp);
        lcd.print(" C");
        lcd.setCursor(0, 1);
        lcd.print("Umid.: ");
        lcd.print("         ");
        lcd.setCursor(7, 1);
        lcd.print(hum);
        lcd.print(" %");
        Serial.print("Umidade: ");
        Serial.print(hum);
        Serial.print(" / Temperatura: ");
        Serial.print(temp);
        Serial.println("°C");
        if (!digitalRead(BTN_BACK)) break;  // Voltar ao menu
      }
      break;

    case 2:  // Relé
      releState = !releState;
      digitalWrite(RELE_PIN, releState ? HIGH : LOW);
      lcd.print(releState ? "Rele Ligado" : "Rele Desligado");
      delay(1000);
      break;

    case 3:  // Mensagem no LCD
      lcd.print("Digite no serial");
      Serial.println("Digite para enviar ao LCD: ");

      while (true) {
        if (Serial.available()) {
          // Lê o texto enviado pelo monitor serial
          String texto = Serial.readStringUntil('\n');  // Lê até encontrar uma nova linha
          texto.trim();                                 // Remove espaços ou quebras de linha desnecessárias

          // Limpa o display LCD
          lcd.clear();

          // Exibe o texto no LCD
          lcd.setCursor(0, 0);
          if (texto.length() <= 16) {
            // Texto cabe em uma linha
            lcd.print(texto);
          } else {
            // Divide o texto em duas linhas
            lcd.print(texto.substring(0, 16));  // Primeiros 16 caracteres
            lcd.setCursor(0, 1);
            lcd.print(texto.substring(16, 32));  // Próximos 16 caracteres
          }

          // Mensagem no monitor serial para indicar que o texto foi exibido
          Serial.println("Texto enviado para o LCD!");
        }
        if (!digitalRead(BTN_BACK)) break;  // Voltar ao menu
      }
      break;

    case 4:  // LED Vermelho
      ledVermelhoState = !ledVermelhoState;
      digitalWrite(LED_VERMELHO, ledVermelhoState ? HIGH : LOW);
      lcd.print(ledVermelhoState ? "LED Vermelho On" : "LED Vermelho Off");
      delay(1000);
      break;

    case 5:  // LED Amarelo
      ledAmareloState = !ledAmareloState;
      digitalWrite(LED_AMARELO, ledAmareloState ? HIGH : LOW);
      lcd.print(ledAmareloState ? "LED Amarelo On" : "LED Amarelo Off");
      delay(1000);
      break;

    case 6:  // Backlight
      if (!backlightState) {
        Serial.println("Backlight On");
      } else Serial.println("Backlight Off");
      backlightState = !backlightState;
      backlightState ? lcd.backlight() : lcd.noBacklight();
      lcd.print(backlightState ? "Backlight On" : "Backlight Off");
      delay(1000);
      break;
  }
}