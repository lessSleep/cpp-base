#include <iostream>
// class BigFive
// {
// public:
//     BigFive() = default; // <= 构造函数

//     BigFive(const BigFive& ref) = default; // <= 拷贝构造函数
//     BigFive& operator= (const BigFive& ref) = default; // <= 拷贝赋值函数

//     /*下面就新进的两员大将, 至少瞅3秒*/
//     BigFive(BigFive&& rref) = default; // <= 移动拷贝构造函数。const不见了，形参前的两个&
//     BigFive& operator=(BigFive&& rref) = default; // <= 移动拷贝赋值函数。const不见了，形参前的两个&

//     ~BigFive() = default; // <= 析构函数
// };

class BigFive {
private:
    int* data;
    int size;
public:
    BigFive() : data(nullptr), size(0) {}
    BigFive(int s) : data(new int[s]), size(s) {}
    BigFive(const BigFive& other) : data(new int[other.size]), size(other.size) {
        std::copy(other.data, other.data + other.size, data);
    }
    BigFive& operator=(const BigFive& other) {
        if (this != &other) {
            int* temp = new int[other.size];
            std::copy(other.data, other.data + other.size, temp);
            delete[] data;
            data = temp;
            size = other.size;
        }
        return *this;
    }
    BigFive(BigFive&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }
    BigFive& operator=(BigFive&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
    ~BigFive() {
        delete[] data;
    }
};


class BigThree {
private:
    int* data;
    int size;
public:
    BigThree() : data(nullptr), size(0) {}
    BigThree(int s) : data(new int[s]), size(s) {}
    BigThree(const BigThree& other) : data(new int[other.size]), size(other.size) {
        std::copy(other.data, other.data + other.size, data);
    }
    BigThree& operator=(const BigThree& other) {
        if (this != &other) {
            int* temp = new int[other.size];
            std::copy(other.data, other.data + other.size, temp);
            delete[] data;
            data = temp;
            size = other.size;
        }
        return *this;
    }
    ~BigThree() {
        delete[] data;
    }
};

void three(){

    BigThree obj1(5);
    BigThree obj2(obj1);
    BigThree obj3;
    obj3 = obj2;
}

int main() {

    three();

    BigFive obj1(5);
    BigFive obj2(obj1);
    BigFive obj3;
    obj3 = obj2;
    BigFive obj4(std::move(obj3));
    BigFive obj5;
    obj5 = std::move(obj4);
    return 0;
}
