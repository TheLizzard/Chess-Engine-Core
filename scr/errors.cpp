#pragma once
#include <exception>


using namespace std;


class IndexError: public std::exception{
    public:
        const char* what() const noexcept{
            return "IndexError";
        }
};


class IterationError: public std::exception{
    public:
        const char* what() const noexcept{
            return "IterationError";
        }
};


class ValueError: public std::exception{
    public:
        const char* what() const noexcept{
            return "ValueError";
        }
};


class ListSizeChangedError: public std::exception{
    public:
        const char* what() const noexcept{
            return "ListSizeChangedError";
        }
};


class CapturedKingError: public std::exception{
    public:
        const char* what() const noexcept{
            return "CapturedKingError";
        }
};


class MoveNeedsPromotionError: public std::exception{
    public:
        const char* what() const noexcept{
            return "MoveNeedsPromotionError";
        }
};


class IllegalMoveError: public std::exception{
    public:
        const char* what() const noexcept{
            return "IllegalMoveError";
        }
};

