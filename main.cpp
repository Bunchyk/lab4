#include <iostream>
#include <cassert>
#include <sstream>
#include <stdexcept>

using namespace std;

class Vector {
private:
    int* arr;
    int capacity;
    int current;

public:
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;

    class iterator {
    private:
        int* ptr;
    public:
        using value_type = Vector::value_type;
        using difference_type = Vector::difference_type;
        using pointer = Vector::pointer;
        using reference = Vector::reference;
        using iterator_category = Vector::iterator_category;

        iterator() : ptr(nullptr) {}
        explicit iterator(int* p) : ptr(p) {}
        iterator(const iterator& other) : ptr(other.ptr) {}

        iterator& operator++() {
            ++ptr;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        iterator& operator--() {
            --ptr;
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        int& operator*() { return *ptr; }
        int* operator->() { return ptr; }

        friend bool operator==(const iterator& a, const iterator& b) {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(const iterator& a, const iterator& b) {
            return a.ptr != b.ptr;
        }
    };

    iterator begin() { return iterator(arr); }
    iterator end() { return iterator(arr + current); }
    auto rbegin() { return std::reverse_iterator(end()); }
    auto rend() { return std::reverse_iterator(begin()); }

    friend class Stack;

    Vector() : arr(new int[1]), capacity(1), current(0) {}

    ~Vector() {
        delete[] arr;
    }

    explicit Vector(size_t n) : arr(new int[n]), capacity(n), current(0) {
        for (size_t i = 0; i < n; ++i) {
            arr[i] = 0;
        }
    }


    Vector(const Vector& other) : arr(new int[other.capacity]), capacity(other.capacity), current(other.current) {
        for (int i = 0; i < current; ++i) {
            arr[i] = other.arr[i];
        }
    }


    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }

        delete[] arr;

        capacity = other.capacity;
        current = other.current;
        arr = new int[capacity];

        for (int i = 0; i < current; ++i) {
            arr[i] = other.arr[i];
        }

        return *this;
    }


    Vector(Vector&& other) noexcept : arr(other.arr), capacity(other.capacity), current(other.current) {
        other.arr = nullptr;
        other.capacity = 0;
        other.current = 0;
    }


    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] arr;

        arr = other.arr;
        capacity = other.capacity;
        current = other.current;

        other.arr = nullptr;
        other.capacity = 0;
        other.current = 0;

        return *this;
    }

    void push(int data) {
        if (current == capacity) {
            int* temp = new int[2 * capacity];
            for (int i = 0; i < capacity; ++i) {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
            capacity *= 2;
        }
        arr[current++] = data;
    }

    int get(int index) const {
        if (index < current) {
            return arr[index];
        }
        return -1;
    }

    void clear() {
        delete[] arr;
        arr = nullptr;
        current = 0;
        capacity = 0;
    }

    int size() const { return current; }
    int getCapacity() const { return capacity; }

    void resize(int n) {
        if (n == size()) return;

        Vector temp;
        if(n > capacity) {
            capacity = n *2;
            for (int i = 0; i < current; ++i) {
                temp.push(arr[i]);
            }
            for (int i = current; i < n; ++i) {
                temp.push(0);
            }
        }else {
            for (int i = 0; i < n; ++i) {
                temp.push(arr[i]);
            }
        }
        *this = temp;
    }

    void assign(size_t n, int value) {
        clear();
        for (size_t i = 0; i < n; ++i) {
            push(value);  //
        }
    }

    int at(int index) const {
        if (index < 0) {
            index += current;
        }

        if (index < 0) {
            return arr[0];
        }

        if (index >= current) {
            return arr[current - 1];
        }

        return arr[index];
    }
};


class Stack {
private:
    Vector* vec;

public:
    using iterator = Vector::iterator;

    ~Stack() { delete vec; }

    Stack() : vec(new Vector()) {}
    explicit Stack(const size_t n) { vec = new Vector(n); }

    Stack(const int a[], const size_t n) {
        vec = new Vector();
        for (int i = 0; i < n; i++) vec->push(a[i]);
    }

    Stack(const Stack& a) : vec(new Vector(*a.vec)) {}

    Stack(const std::reverse_iterator<Vector::iterator>& rbegin,
          const std::reverse_iterator<Vector::iterator>& rend) : vec(new Vector()) {
        for (auto it = rbegin; it != rend; ++it) {
            vec->push(*it);
        }
    }

    void clear() {
        vec->clear();
        vec = new Vector();
    }

    int at(int i) const { return vec->at(i); }

    void resize(size_t n) { vec->resize(n); }
    void assign(size_t n, int value) { vec->assign(n, value); }

    size_t length() const { return vec->size(); }

    [[nodiscard]] bool empty() const { return vec->size() == 0; }

    void swap(Stack& a) noexcept { std::swap(vec, a.vec); }

    Stack &operator=(const Stack& s) {
        if (this == &s) return *this;

        delete vec;
        vec = new Vector(*s.vec);
        return *this;
    }

    Stack &operator=(Stack&& s) noexcept {
        if (this == &s) return *this;
        delete vec;
        vec = s.vec;
        s.vec = nullptr;
        return *this;
    }

    friend bool operator==(const Stack& a, const Stack& b) {
        if (a.length() != b.length()) return false;

        for (size_t i = 0; i < a.length(); ++i) {
            if (a.at(i) != b.at(i)) return false;
        }
        return true;
    }

    friend bool operator!=(const Stack& a, const Stack& b) {
        return !(a == b);
    }

    friend bool operator<(const Stack& a, const Stack& b) {
        size_t minLength = std::min(a.length(), b.length());
        for (size_t i = 0; i < minLength; ++i) {
            if (a.at(i) != b.at(i)) return a.at(i) < b.at(i);
        }
        return a.length() < b.length();
    }

    friend bool operator>(const Stack& a, const Stack& b) {
        return b < a;
    }

    friend bool operator<=(const Stack& b, const Stack& a) {
        return !(b < a);
    }

    friend bool operator>=(const Stack& a, const Stack& b) {
        return !(a < b);
    }

    int& operator[](size_t index) {
        return vec->arr[index];
    }

    friend std::ostream &operator<<(std::ostream& os, const Stack& s) {
        os << "[";
        for (size_t i = 0; i < s.length(); ++i) {
            os << s.at(i);
            if (i + 1 < s.length()) os << ", ";
        }
        os << "]";
        return os;
    }

    void push(int data) {
        vec->push(data);
    }

    int pop() {
        int res = vec->arr[vec->size() - 1];
        vec->resize(vec->size() - 1);
        return res;
    }

    Stack(Vector::iterator a, Vector::iterator b) : vec(new Vector()) {
        for (auto it = a; it != b; ++it) {
            vec->push(*it);
        }
    }


    auto begin() { return vec->begin(); }
    auto end() { return vec->end(); }
    auto rbegin() { return vec->rbegin(); }
    auto rend() { return vec->rend(); }


    static bool is_equal(Stack& a, Stack& b) {
        if (a.length() != b.length()) return false;

        auto it1 = a.begin();
        auto it2 = b.begin();
        while (it1 != a.end()) {
            if (*it1 != *it2) return false;
            ++it1;
            ++it2;
        }

        return true;
    }


    void assign(iterator it1, iterator it2) {
        clear();
        while (it1 != it2) {
            vec->push(*it1);
            ++it1;
        }
    }
};

void print(Stack& a) {
    for (int i = 0; i < a.length(); ++i) {
        cout << a.at(i) << endl;
    }
}

int main(int argc, char const *argv[])
{
    int array[] = {19, 47, 74, 91};

    Stack a(array, 4);
    for (auto it = a.begin(); it != a.end(); ++it) std::cout << *it << " ";
    cout<<endl;

    Stack b(a.begin(), a.end());
    assert(Stack::is_equal(a, b));
    for (auto &&it : b) std::cout << it << " ";
    cout<<endl;

    Stack c(b.begin(), b.end());
    assert(std::equal(c.begin(), c.end(), b.begin()));
    for (auto it = c.rbegin(); it != c.rend(); ++it) std::cout << *it << " ";
    cout << endl;

    Stack d(c.rbegin(), c.rend());
    for (auto &&it : d) std::cout << it << " ";
}
