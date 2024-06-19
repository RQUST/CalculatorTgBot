#ifndef CALCULATOR_BOT_H
#define CALCULATOR_BOT_H

#include <tgbot/tgbot.h>
#include <iostream>
#include <string_view>

#include "calculator/calculator/calculator.h"
#include "data_base/data_base.h"

class CalculatorBot {
 private:
  TgBot::Bot bot_;
  Database db_;

 public:
  explicit CalculatorBot(std::string_view api_token) : bot_(api_token.data()), db_() {
    bot_.getEvents().onCommand("start",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnStart(message_ptr->chat->id); });

    bot_.getEvents().onCommand("help",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnHelp(message_ptr->chat->id); });

    bot_.getEvents().onCommand("hello",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnHello(message_ptr->chat->id); });

    bot_.getEvents().onCommand("history",
                               [this](TgBot::Message::Ptr message_ptr) { ActionOnHistory(message_ptr->chat->id); });

    bot_.getEvents().onAnyMessage([this](TgBot::Message::Ptr message_ptr) { ActionOnAnyMessage(message_ptr); });
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

  void ActionOnHistory(int64_t chat_id) {
    try {
      auto logs = db_.GetLastTenLogs(chat_id);
      std::string history_message = "Last 10 calculations:\n";
      for (const auto& [timestamp, expression] : logs) {
        history_message += timestamp + " - " + expression + "\n";
      }
      if (logs.empty()) {
        history_message += "No calculations found.\n";
      }
      bot_.getApi().sendMessage(chat_id, history_message);
    } catch (const std::exception& e) {
      bot_.getApi().sendMessage(chat_id, "Error retrieving history: " + std::string(e.what()));
    }
  }

  void ActionOnAnyMessage(TgBot::Message::Ptr message_ptr) {
    const std::string& text = message_ptr->text;
    // Check if the message is a command
    if (text[0] == '/') {
      // Command is already handled, do nothing
      return;
    } else {
      ActionOnCalculate(message_ptr);
    }
  }

  void ActionOnCalculate(TgBot::Message::Ptr message_ptr) {
    try {
      const std::string& text = message_ptr->text;
      int result = CalculateExpression(text);
      bot_.getApi().sendMessage(message_ptr->chat->id, "Result: " + std::to_string(result));
      db_.InsertLog(message_ptr->chat->id, text);  // Сохраняем запрос в базу данных
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
