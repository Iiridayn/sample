// 3.34.c by Ged Crimsonclaw (Michael Clark) 2-4-2004

int switch_prob(int x) {
	int result = x;
  
  switch(x-50) { //Why?  We may never know, but it matches the provided asm
  /* Ged's Code */  
  case 0:
 	case 2:
  	result <<= 2;
	  break;
  case 3:
  	result >>= 2;
	  break;
  case 4:
  	result *= 3;
  case 5:
  	result *= result;
  case 1:
  default:
  	result += 10;
  /* End Ged's Code */
  }
  
  return result;
}
