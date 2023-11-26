#include <iostream>
#include <memory>

class B;  //Forward declaration

class A {
public:
    std::shared_ptr<B> bPtr;

    ~A() {
        std::cout << "A destructor called" << std::endl;
    }
};

class B {
public:
    // std::shared_ptr<A> aPtr;
    std::weak_ptr<A> aPtr;	//use weak ptr to break the loop

    ~B() {
        std::cout << "B destructor called" << std::endl;
    }
};

int main() {
	while(1) {
		std::shared_ptr<A> aPtr = std::make_shared<A>();
		std::shared_ptr<B> bPtr = std::make_shared<B>();

		// make it loop
		aPtr->bPtr = bPtr;
		bPtr->aPtr = aPtr;

		std::cout << "in while loop..." << std::endl;
	} return 0;
}
