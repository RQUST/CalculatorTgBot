#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <memory>
#include <stdexcept>

class IExpression {
 public:
  virtual int Calculate() const = 0;

  virtual ~IExpression() = default;
};

class Constant final : public IExpression {
  int value_;

 public:
  explicit Constant(int value) : value_(value) {
  }

  int Calculate() const final {
    return value_;
  }
};

class IUnaryOperation : public IExpression {
  std::unique_ptr<IExpression> operand_;

 public:
  explicit IUnaryOperation(std::unique_ptr<IExpression> operand) : operand_(std::move(operand)) {
  }

  int Calculate() const final {
    const auto x = operand_->Calculate();
    return Operation(x);
  }

  virtual int Operation(int x) const = 0;
};

class Square final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;

  int Operation(int x) const final {
    return x * x;
  }
};

class AbsoluteValue final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;

  int Operation(int x) const final {
    return x < 0 ? -x : x;
  }
};

class Plus final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;

  int Operation(int x) const final {
    return +x;
  }
};

class Minus final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;

  int Operation(int x) const final {
    return -x;
  }
};

class IBinaryOperation : public IExpression {
  std::unique_ptr<IExpression> left_;
  std::unique_ptr<IExpression> right_;

 public:
  IBinaryOperation(std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right)
      : left_(std::move(left)), right_(std::move(right)) {
  }

  int Calculate() const final {
    const auto x = left_->Calculate();
    const auto y = right_->Calculate();
    return Operation(x, y);
  }

  virtual int Operation(int x, int y) const = 0;
};

class Subtract final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x - y;
  }
};

class Multiply final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x * y;
  }
};

class Sum final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x + y;
  }
};


class Divide final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    if (y == 0) {
      throw std::runtime_error("Division by zero");
    }
    return x / y;
  }
};

class Residual final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    if (y == 0) {
      throw std::runtime_error("Residual by zero");
    }
    return x % y;
  }
};
 
class Minimum final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return std::min(x, y);
  }
};

class Maximum final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return std::max(x, y);
  }
};
#endif  // EXPRESSIONS_H