#include <stdio.h>

class c1 {
protected:
	int x;
	int y;
public:
	c1() { return; }
	void setx1(int v) { x = v; }
	void sety1(int v) { y = v; }
	int getx1() { return x; }
	int gety1() { return y; }
};
class c2 : public c1 {
protected:
	int y;
public:
	c2() : c1() { return; }
	void sety2(int v) { y = v; }
	int getx2() { return x; }
	int gety2() { return y; }
};
int main() {
	c2 *o2 = new c2();
	o2->setx1(101);
	o2->sety1(102);
	o2->sety2(999);
	printf("(%d %d %d %d)\n", o2->getx1(), o2->gety1(), o2->getx2(), o2->gety2());
}
