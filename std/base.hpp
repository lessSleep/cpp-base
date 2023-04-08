class MyClass {
public:
    MyClass() = default;
    MyClass(const MyClass&) = delete; // disable copy constructor
    MyClass& operator=(const MyClass&) = delete; // disable copy assignment operator
    // rest of the class implementation
}; 

// Example usage:
// MyClass a; // default constructor
// MyClass b(a); // error: copy constructor is deleted
// MyClass c = a; // error: copy constructor is deleted


class NoCopy {
public:
    NoCopy(int x) : data(x) {} // constructor with int parameter
    NoCopy(const NoCopy&) = delete; // disable copy constructor
    NoCopy& operator=(const NoCopy&) = delete; // disable copy assignment operator
    // rest of the class implementation
    int operator()() const { return data; } // overload of the function call operator
private:
    int data;
};

// Example usage:
// NoCopy a(5); // constructor with int parameter
// NoCopy b(a); // error: copy constructor is deleted
// NoCopy c = a; // error: copy constructor is deleted