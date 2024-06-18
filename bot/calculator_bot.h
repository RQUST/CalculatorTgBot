#ifndef CALCULATOR_BOT_H
#define CALCULATOR_BOT_H

#include <tgbot/tgbot.h>
#include <iostream>
#include <string_view>

class CalculatorBot {
 private:
  TgBot::Bot bot_;

 public:
  explicit CalculatorBot(std::string_view api_token) : bot_(api_token.data()) {
    bot_.getEvents().onCommand("start",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnStart(message_ptr->chat->id); });

    bot_.getEvents().onCommand("help",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnHelp(message_ptr->chat->id); });
  }

  void ActionOnStart(int64_t chat_id) {
    bot_.getApi().sendMessage(chat_id, "Bot started!");
  }

  void ActionOnHelp(int64_t chat_id) {
    bot_.getApi().sendMessage(chat_id, "No Help");
  }

  void Run() {
    bot_.getApi().deleteWebhook();
    auto long_poll = TgBot::TgLongPoll(bot_);
    while (true) {
      try {
        long_poll.start();
      } catch (const TgBot::TgException& err) {
        std::cerr << "TgBot error: " << err.what() << '\n';
        continue;
      }
    }
  }
};

#endif // CALCULATOR_BOT_H
