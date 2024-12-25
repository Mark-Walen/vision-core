//
// Created by Mark-Walen on 2024/12/24.
//

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>

namespace vlue::exceptions {
    class Exception : public std::runtime_error {
    public:
        explicit Exception(const std::string& msg_)
            : std::runtime_error(build_what(msg_)), msg(msg_) {}
        ~Exception() noexcept override;

        Exception(const Exception&) = default;

        std::string msg;

    private:
        static std::string build_what(const std::string& msg) {
            return msg;
        }
    };

    class BadFile final : public Exception {
    public:
        explicit BadFile(const std::string& filename)
            : Exception(std::string("Bad File") + ": " + filename) {}
        BadFile(const BadFile&) = default;
        ~BadFile() noexcept override;
    };

    class ParserException : public Exception {
    public:
        explicit ParserException(const std::string& msg_)
            : Exception(msg_) {}
        ParserException(const ParserException&) = default;
        ~ParserException() noexcept override;
    };
};

#endif //EXCEPTIONS_H
