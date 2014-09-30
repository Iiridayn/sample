#include <stdio.h>

class c1 {
public:
	c1() { return; }
	int m1() { return 1; }
	int m2() { return this->m1(); }
};
class c2 : public c1 {
public:
	c2() : c1() { return; }
	int m1() { return 2; }
};
int main() {
	c1 *o1 = new c1();
	c2 *o2 = new c2();
	printf("(%d %d %d)\n", o1->m1(), o2->m1(), o2->m2());
}
