// 3.35.c by Ged Crimsonclaw (Michael Clark) 2-4-2004

typedef int *var_matrix;

// Constraints: must be correct, and have only 5 locals
int var_prod_ele_ged(var_matrix A, var_matrix B, int i, int k, int n) {
	int *Aptr = &A[i*n];
  int *Bptr = &B[n];
  int cnt = n;
  int result = 0;
  
	do {
  	result += (*Aptr) * (*Bptr);
    Aptr += 1;
    Bptr += n;
    cnt--;
  } while (cnt);
  
  return result;
}
