#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>

WiFiClientSecure client;

#define token "Токен вашего бота"

//Объявляем пользователей, вводим ID Телеграмма
#define user1 "ID Телеграм Пользователя 1"
#define user2 "ID Телеграм Пользователя 2"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
UniversalTelegramBot bot(token, client);
unsigned long lastTime;

void setup() {
  
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
  WiFi.begin("Ваш SSID", "Пароль Вайфай");
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);
  //Информируем каждого пользователя о подключении к Сети
  bot.sendMessage(user1, "Розетки в Сети");
  bot.sendMessage(user2, "Розетки в Сети");
}

void loop() {
  
  if(millis() - lastTime > 1000)
  {
    int numMessages = bot.getUpdates(bot.last_message_received+1);

    while(numMessages)
    {
      handleNewMessages(numMessages);
      numMessages = bot.getUpdates(bot.last_message_received+1);
    }

    lastTime = millis();
  }
}

void handleNewMessages(int numMessages) {
  for(int i = 0; i < numMessages; i++) {

    String text = bot.messages[i].text;
    String id = bot.messages[i].chat_id;
    
    //Добавляем функцию switchState для каждого пользователя
    switchState(id, text, user1);
    switchState(id, text, user2);
  }
}

void switchState (String id, String text, String user) {
  if(id == user)
    {
      if(text == "/lampon") {
        digitalWrite(2, LOW);
        bot.sendMessage(id, "Ряд лампы ВКЛ");
      }
      if(text == "/lampoff") {
        digitalWrite(2, HIGH);
        bot.sendMessage(id, "Ряд лампы ВЫКЛ");
      }
      if(text == "/chargeon") {
        digitalWrite(0, LOW);
        bot.sendMessage(id, "Ряд зарядки ВКЛ");
      }
      if(text == "/chargeoff") {
        digitalWrite(0, HIGH);
        bot.sendMessage(id, "Ряд зарядки ВЫКЛ");
      }
      if(text == "/allon") {
        digitalWrite(0, LOW);
        digitalWrite(2, LOW);
        bot.sendMessage(id, "Все ВКЛ");
      }
      if(text == "/alloff") {
        digitalWrite(0, HIGH);
        digitalWrite(2, HIGH);
        bot.sendMessage(id, "Все ВЫКЛ");
      }
    }
}
