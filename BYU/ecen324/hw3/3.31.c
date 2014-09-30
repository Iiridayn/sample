int decode2(int x, int y, int z) {
  int eax = (y-z);
	return (x*eax)^((eax<<31)>>31);
}
