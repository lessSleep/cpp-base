template <typename Derived>
class Base {
public:
    void addFree(int i) {
        // ...
        static_cast<Derived*>(this)->add(i);
        // ...
    }
};

class Derived : public Base<Derived> {
public:
    void add(int i) {
        // ...
        total += i;
    }
private:
    int total = 0;
};


class BaseNormal {
public:
    void virtual addFree(int i) {

    }
};

class DerivedNormal : public BaseNormal {
public:
    void addFree(int i) {
        // ...
        total += i;
    }
    
private:
    int total = 0;
};

int main() {
    BaseNormal* p = new DerivedNormal();
    p->addFree(5);
    Base* p = new Base<Derived>();
    return 0;
}