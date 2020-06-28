#include <iostream>

using namespace std;

template<class T>
class safe_c_array_t {
private:
    T *arr;
    int a_size;

public:
    explicit safe_c_array_t(int size) {
        a_size = size;
        arr = new T[size];

    }

    safe_c_array_t(const safe_c_array_t &current) {
        a_size = current.a_size;
        arr = new T[a_size];
        for (int i = 0; i < a_size; i++) {
            arr[i] = current[i];
        };
    }

    void operator=(safe_c_array_t &current) {
        if (this != &current) {
            delete[] arr;
            a_size = current.a_size;
            arr = new T[a_size];
            for (int i = 0; i < a_size; i++) {
                arr[i] = current[i];
            };
        };
    }

    const T operator[](int index) const {
        if (index < 0 || index >= a_size) {
            cout << "index out of range!";
            exit(-1);
        };

        return arr[index];
    }

    T &operator[](int index) {
        if (index < 0 || index >= a_size) {
            cout << "index out of range!";
            exit(-1);
        };

        return arr[index];
    }

    ~safe_c_array_t() {
        delete[] arr;
    }
};

//int main() {
//    safe_c_array_t<int> cur(10);
//    cur[2] = 7;
//    cout << cur[2];
//
//    safe_c_array_t<int> cur2(10);
//    cur2[2] = 5;
//    cur2 = cur;
//    cout << cur2[2];
//    cout << cur2[2];
//
//    return 0;
//}
