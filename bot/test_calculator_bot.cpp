#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <tgbot/tgbot.h>
#include "bot/calculator_bot.h"

// Mock class for TgBot::Api
class MockApi {
public:
    void sendMessage(int64_t chat_id, const std::string& text) {
        last_chat_id_ = chat_id;
        last_message_ = text;
    }

    int64_t last_chat_id_;
    std::string last_message_;
};

// Mock class for TgBot::Bot
class MockBot : public TgBot::Bot {
public:
    explicit MockBot(const std::string& token) : TgBot::Bot(token), api_() {}

    MockApi& getApi() {
        return api_;
    }

private:
    MockApi api_;
};

TEST_CASE("CalculatorBot Start Command", "[CalculatorBot]") {
    MockBot bot("dummy_token");
    CalculatorBot calc_bot("dummy_token");

    TgBot::Message::Ptr message = std::make_shared<TgBot::Message>();
    message->chat = std::make_shared<TgBot::Chat>();
    message->chat->id = 123456789;
    message->text = "/start";

    calc_bot.ActionOnStart(message->chat->id);

    REQUIRE(bot.getApi().last_chat_id_ == 123456789);
    REQUIRE(bot.getApi().last_message_ == "Bot started!");
}

TEST_CASE("CalculatorBot Help Command", "[CalculatorBot]") {
    MockBot bot("dummy_token");
    CalculatorBot calc_bot("dummy_token");

    TgBot::Message::Ptr message = std::make_shared<TgBot::Message>();
    message->chat = std::make_shared<TgBot::Chat>();
    message->chat->id = 123456789;
    message->text = "/help";

    calc_bot.ActionOnHelp(message->chat->id);

    REQUIRE(bot.getApi().last_chat_id_ == 123456789);
    REQUIRE(bot.getApi().last_message_ == "No Help");
}

TEST_CASE("CalculatorBot Hello Command", "[CalculatorBot]") {
    MockBot bot("dummy_token");
    CalculatorBot calc_bot("dummy_token");

    TgBot::Message::Ptr message = std::make_shared<TgBot::Message>();
    message->chat = std::make_shared<TgBot::Chat>();
    message->chat->id = 123456789;
    message->text = "/hello";

    calc_bot.ActionOnHello(message->chat->id);

    REQUIRE(bot.getApi().last_chat_id_ == 123456789);
    REQUIRE(bot.getApi().last_message_ == "Hello");
}

TEST_CASE("CalculatorBot Calculate Expression", "[CalculatorBot]") {
    MockBot bot("dummy_token");
    CalculatorBot calc_bot("dummy_token");

    TgBot::Message::Ptr message = std::make_shared<TgBot::Message>();
    message->chat = std::make_shared<TgBot::Chat>();
    message->chat->id = 123456789;
    message->text = "3 + 4";

    // Mock the CalculateExpression function
    auto calculate_expr = [](const std::string&) { return 7; };

    // Replace ActionOnCalculate with direct call and assert
    calc_bot.ActionOnCalculate(message);

    REQUIRE(bot.getApi().last_chat_id_ == 123456789);
    REQUIRE(bot.getApi().last_message_ == "Result: 7");
}

TEST_CASE("CalculatorBot Handle Invalid Expression", "[CalculatorBot]") {
    MockBot bot("dummy_token");
    CalculatorBot calc_bot("dummy_token");

    TgBot::Message::Ptr message = std::make_shared<TgBot::Message>();
    message->chat = std::make_shared<TgBot::Chat>();
    message->chat->id = 123456789;
    message->text = "invalid expression";

    // Mock the CalculateExpression function to throw an exception
    auto calculate_expr = [](const std::string&) -> int { throw std::runtime_error("Invalid expression"); };

    // Replace ActionOnCalculate with direct call and assert
    calc_bot.ActionOnCalculate(message);

    REQUIRE(bot.getApi().last_chat_id_ == 123456789);
    REQUIRE(bot.getApi().last_message_ == "Error: Invalid expression");
}
