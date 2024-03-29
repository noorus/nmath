#ifndef BANDIT_CONTEXT_H
#define BANDIT_CONTEXT_H

#include <algorithm>
#include <deque>
#include <functional>
#include <list>
#include <string>
#include <bandit/test_run_error.h>

namespace bandit {
  namespace context {
    struct interface {
      virtual ~interface() {}
      virtual const std::string& name() = 0;
      virtual void execution_is_starting() = 0;
      virtual void register_before_each(std::function<void()> func) = 0;
      virtual void register_after_each(std::function<void()> func) = 0;
      virtual void run_before_eaches() = 0;
      virtual void run_after_eaches() = 0;
      virtual bool hard_skip() = 0;
    };

    struct bandit : public interface {
      bandit(const std::string& desc, bool hard_skip_a)
          : desc_(desc), hard_skip_(hard_skip_a), is_executing_(false) {}

      const std::string& name() override {
        return desc_;
      }

      void execution_is_starting() override {
        is_executing_ = true;
      }

      void register_before_each(std::function<void()> func) override {
        throw_if_executing("before_each");
        before_eaches_.push_back(func);
      }

      void register_after_each(std::function<void()> func) override {
        throw_if_executing("after_each");
        after_eaches_.push_back(func);
      }

      void run_before_eaches() override {
        run_all(before_eaches_);
      }

      void run_after_eaches() override {
        run_all(after_eaches_);
      }

      bool hard_skip() override {
        return hard_skip_;
      }

    private:
      void throw_if_executing(std::string&& method) const {
        if (is_executing_) {
          throw detail::test_run_error("'" + method + "' was called after 'describe' or 'it'");
        }
      }

      void run_all(const std::list<std::function<void()>>& funcs) {
        for (auto f : funcs) {
          f();
        }
      }

      std::string desc_;
      bool hard_skip_;
      bool is_executing_;
      std::list<std::function<void()>> before_eaches_;
      std::list<std::function<void()>> after_eaches_;
    };

    struct stack_t : public std::deque<context::interface*> {
      void throw_if_empty(std::string&& method) const {
        if (empty()) {
          throw detail::test_run_error("'" + method + "' was called without surrounding 'describe'");
        }
      }
    };
  }
}
#endif
