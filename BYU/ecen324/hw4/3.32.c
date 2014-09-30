int absdiff2(int x, int y) {
	int result;
 	
  if(x<y)
  	goto LESS;
  result = y-x;
  goto RET;
LESS:
	result = x-y;
RET:
	return result; 
}


/*	Assembly version
int absdiff2(int x, int y) {
	int result;
  
  result = x-y;
  if(x<y)
  	result = y-x;
 	
  return result;
}
*/
