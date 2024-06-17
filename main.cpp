#include <tgbot/tgbot.h>

int main() {
  TgBot::Bot bot("6834213547:AAGsRMnIu-9ZcuYc5l6RqkAwKFLAxtfhCT8");
  auto& api = bot.getApi();

  bot.getEvents().onCommand("start", [&api](TgBot::Message::Ptr message_ptr) {
    const auto chat_id = message_ptr->chat->id;
    api.sendMessage(chat_id, "Bot started");
  });

  bot.getEvents().onCommand("help", [&api](TgBot::Message::Ptr message_ptr) {
    const auto chat_id = message_ptr->chat->id;
    api.sendMessage(chat_id, "Help message");
  });

  bot.getEvents().onCommand("hello", [&api](TgBot::Message::Ptr message_ptr) {
    const auto chat_id = message_ptr->chat->id;
    api.sendMessage(chat_id, "Hello, " + message_ptr->chat->username);
  });

  bot.getEvents().onCommand("cpp", [&api](TgBot::Message::Ptr message) {
    TgBot::InputFile::Ptr photo = TgBot::InputFile::fromFile("ISO_C++_Logo.svg.png", "image/png");
    api.sendPhoto(message->chat->id, photo);
  });

  bot.getEvents().onAnyMessage( [&api](TgBot::Message::Ptr message_ptr) {
    const auto chat_id = message_ptr->chat->id;
    api.sendMessage(chat_id, message_ptr->text);
  });

  bot.getEvents().onUnknownCommand([&api](TgBot::Message::Ptr message_ptr) {
    const auto chat_id = message_ptr->chat->id;
    api.sendMessage(chat_id, "Unknown Command, " + message_ptr->text);
  });

  api.deleteWebhook();
  auto long_poll = TgBot::TgLongPoll(bot);
  while (true) {
    try {
      long_poll.start();
    } catch (const TgBot::TgException& err) {
      std::cerr << "TgBot error" << err.what() << '\n';
      continue;
    }
  }
}