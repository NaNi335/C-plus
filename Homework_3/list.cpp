#include "list.h"

int_list_t::int_list_t() {
    first = new node_t(0, nullptr, nullptr);
    last = new node_t(0, nullptr, first);
    first->next = last;
    list_size = 0;
}

int_list_t::node_t *int_list_t::get(int pos) const {
    if (pos <= list_size / 2) {
        node_t *curr = first->next;
        for (int i = 0; i < pos; ++i) {
            curr = curr->next;
        }

        return curr;
    } else {
        node_t *curr = last;
        for (int i = 0; i < list_size - pos; ++i) {
            curr = curr->prev;
        }

        return curr;
    }
}

int_list_t::int_list_t(size_t count, int value) : int_list_t() {
    for (int i = 1; i < count; ++i) {
        push_back(value);
    }
}

int_list_t::int_list_t(const int_list_t &other) : int_list_t() {
    list_size = other.list_size;
    auto curr = other.first->next;
    while (curr != other.last) {
        push_back(curr->value);
        curr = curr->next;
    }
}

int_list_t::~int_list_t() {
    clear();
    delete first;
    delete last;
}


int_list_t &int_list_t::operator=(const int_list_t &other) {
    int_list_t curr(other);
    swap(curr);
    return *this;
}

int &int_list_t::operator[](size_t pos) {
    node_t *curr = get(pos);
    return curr->value;
}

const int &int_list_t::operator[](size_t pos) const {
    node_t *pointer = get(pos);
    return pointer->value;
}


int &int_list_t::front() {
    return first->next->value;
}

int int_list_t::front() const {
    return first->next->value;
}

int &int_list_t::back() {
    return last->prev->value;
}

int int_list_t::back() const {
    return last->prev->value;
}


void int_list_t::clear() {
    node_t *curr = first->next;
    node_t *next_node = curr->next;
    while (next_node != last) {
        delete (curr);
        curr = next_node;
        next_node = curr->next;
    }
    list_size = 0;
}

size_t int_list_t::size() const {
    return list_size;
}

bool int_list_t::empty() const {
    return (list_size == 0);
}


void int_list_t::insert(size_t pos, int new_val) {
    node_t *curr = get(pos);
    auto *new_node = new node_t(new_val, curr, curr->prev);
    curr->prev->next = new_node;
    curr->prev = new_node;
    list_size++;
}

void int_list_t::push_front(int new_val) {
    insert(0, new_val);
}

void int_list_t::push_back(int new_val) {
    insert(list_size, new_val);
}


void int_list_t::erase(size_t pos) {
    node_t *curr = get(pos);
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    delete curr;
    list_size--;
}

void int_list_t::pop_front() {
    erase(0);
}

void int_list_t::pop_back() {
    erase(list_size - 1);
}


int_list_t int_list_t::splice(size_t start_pos, size_t count) {
    int_list_t result_list = int_list_t();
    node_t *curr = get(start_pos);
    result_list.first->next = curr;
    node_t *end_1 = curr->prev;
    curr->prev = result_list.first;

    for (int i = 0; i < count - 1; i++) {
        curr = curr->next;
    }

    result_list.last->prev = curr;
    node_t *end_2 = curr->next;
    curr->next = result_list.last;

    end_1->next = end_2;
    end_2->prev = end_1;

    list_size -= count;
    result_list.list_size = count;

    return result_list;
}

int_list_t &int_list_t::merge(int_list_t &other) {
    if (!other.empty()) {
        last->prev->next = other.first->next;
        other.first->next->prev = last->prev;
        other.last->prev->next = last;
        last->prev = other.last->prev;

        other.first->next = other.last;
        other.last->prev = other.first;
        list_size += other.list_size;
        other.list_size = 0;
    }

    return *this;
}

void int_list_t::reverse(){
    for (auto curr = first; curr != nullptr; curr = curr->prev) {
        std::swap(curr->next, curr->prev);
    }

    std::swap(first, last);
}

void int_list_t::swap(int_list_t &other) {
    std::swap(first, other.first);
    std::swap(last, other.last);
    std::swap(list_size, other.list_size);
}

std::istream &operator>>(std::istream &stream, int_list_t &list) {
    int value = 0;
    list.clear();
    while (stream >> value) {
        list.push_back(value);
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const int_list_t &list) {
    auto curr = list.first->next;
    while (curr != list.last) {
        stream << curr->value << " ";
        curr = curr->next;
    }
    return stream;
}
