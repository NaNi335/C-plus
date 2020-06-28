#include <iostream>

using namespace std;

class safe_long_long_t {

private:
    long long value;

public:
    long long get_value() {
        return value;
    }


    safe_long_long_t(long long _) {
        value = _;
    }

    safe_long_long_t() {
        value = 0;
    }

    safe_long_long_t(safe_long_long_t const &_) {
        value = _.value;
    }

    void operator=(safe_long_long_t _) {
        value = _.value;
    }

    safe_long_long_t operator++() {
        return safe_long_long_t(++value);
    }

    safe_long_long_t operator++(int) {
        return safe_long_long_t(value++);
    }

    safe_long_long_t operator--() {
        return safe_long_long_t(--value);
    }

    safe_long_long_t operator--(int) {
        return safe_long_long_t(value--);
    }

    void operator+=(const safe_long_long_t& _) {
        value += _.value;
    }

    void operator-=(const safe_long_long_t& _) {
        value -= _.value;
    }

    void operator*=(const safe_long_long_t& _) {
        value *= _.value;
    }

    void operator/=(const safe_long_long_t& _) {
        value /= _.value;
    }

    void operator%=(const safe_long_long_t& _) {
        value %= _.value;
    }

    ~safe_long_long_t() = default;
};

safe_long_long_t operator+(const safe_long_long_t& l1, const safe_long_long_t& l2) {
    safe_long_long_t current = l1;
    current += l2;
    return current;
}

safe_long_long_t operator-(const safe_long_long_t& l1, const safe_long_long_t& l2) {
    safe_long_long_t current = l1;
    current -= l2;
    return current;
}

safe_long_long_t operator*(const safe_long_long_t& l1, const safe_long_long_t& l2) {
    safe_long_long_t current = l1;
    current *= l2;
    return current;
}

safe_long_long_t operator/(const safe_long_long_t& l1, const safe_long_long_t& l2) {
    safe_long_long_t current = l1;
    current /= l2;
    return current;
}

safe_long_long_t operator%(safe_long_long_t& l1, safe_long_long_t& l2) {
    safe_long_long_t current = l1;
    current %= l2;
    return current;
}

bool operator==(safe_long_long_t l1, safe_long_long_t l2) {
    return l1.get_value() == l2.get_value();
}

bool operator>(safe_long_long_t l1, safe_long_long_t l2) {
    return l1.get_value() > l2.get_value();
}

bool operator<(safe_long_long_t l1, safe_long_long_t l2) {
    return l1.get_value() < l2.get_value();
}

bool operator>=(safe_long_long_t l1, safe_long_long_t l2) {
    return l1.get_value() >= l2.get_value();
}

bool operator<=(safe_long_long_t l1, safe_long_long_t l2) {
    return l1.get_value() <= l2.get_value();
}

ostream &operator<<(ostream &stream, safe_long_long_t _) {
    stream << _.get_value();
    return stream;
}

istream &operator>>(istream &stream, safe_long_long_t &_) {
    stream >> _;
    return stream;
}