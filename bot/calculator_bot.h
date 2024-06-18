#ifndef CALCULATOR_BOT_H
#define CALCULATOR_BOT_H

#include <tgbot/tgbot.h>
#include <iostream>
#include <string_view>

#include "calculator/calculator/calculator.h"

class CalculatorBot {
 private:
  TgBot::Bot bot_;

 public:
  explicit CalculatorBot(std::string_view api_token) : bot_(api_token.data()) {
    bot_.getEvents().onCommand("start",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnStart(message_ptr->chat->id); });

    bot_.getEvents().onCommand("help",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnHelp(message_ptr->chat->id); });

    bot_.getEvents().onCommand("hello",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnHello(message_ptr->chat->id); });

    bot_.getEvents().onAnyMessage([this](TgBot::Message::Ptr message_ptr) { ActionOnCalculate(message_ptr); });
  }

  void ActionOnStart(int64_t chat_id) {
    bot_.getApi().sendMessage(chat_id, "Bot started!");
  }

  void ActionOnHelp(int64_t chat_id) {
    bot_.getApi().sendMessage(chat_id, "No Help");
  }

  void ActionOnHello(int64_t chat_id) {
    bot_.getApi().sendMessage(chat_id, "Hello");
  }

  void ActionOnCalculate(TgBot::Message::Ptr message_ptr) {
    try {
      const std::string& text = message_ptr->text;
      int result = CalculateExpression(text);
      bot_.getApi().sendMessage(message_ptr->chat->id, "Result: " + std::to_string(result));
    } catch (const std::exception& e) {
      bot_.getApi().sendMessage(message_ptr->chat->id, "Error: " + std::string(e.what()));
    }
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

#endif  // CALCULATOR_BOT_H
