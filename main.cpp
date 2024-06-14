#include <tgbot/Bot.h>

void Start(TgBot::Message::Ptr message_ptr) {
  const auto chat_id = message_ptr->chat->id;
}

int main() {
  TgBot::Bot bot("7411232470:AAHTDVw80FPIfhqyMSy34nc0_mcLc9meuQA");

  bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message_ptr) {
    const auto chat_id = message_ptr->chat->id;
    bot.getApi().sendMessage(chat_id, "Bot started");
  });

  bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message_ptr) {
    const auto chat_id = message_ptr->chat->id;
    bot.getApi().sendMessage(chat_id, "Help message");
  });

 bot.getEvents().onCommand("hello", [&bot](TgBot::Message::Ptr message_ptr) {
    const auto chat_id = message_ptr->chat->id;
    bot.getApi().sendMessage(chat_id, "Hello, " + message_ptr->chat->username);
  });
}