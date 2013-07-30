#include <iostream>
#include <vector>

class Foo {
public:
    explicit Foo(std::size_t j) : array_of_int(j) {}
    virtual  ~Foo() { }
private:
    std::vector<int> array_of_int;
};

class Bar: public Foo {
public:
    explicit Bar(std::size_t j) : Foo(j), array_of_char(j) {}
private:
    std::vector<int> array_of_char;
};
int main() {
    Foo* f = new Foo(10);
    Foo* b = new Bar(200);
    *f = *b;
    delete f;
    delete b;
    return 0;
    }
