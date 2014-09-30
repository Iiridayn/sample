/*
Made by: Ged Crimsonclaw, Michael Clark
Date: 12/13/2001
Purpose: Testing my knowledge of linked lists.
*/

import Lister;

public class Lab14
{
  public static void main (String[] args)
	{
	  Lister nums = new Lister();
		
		for(int i = 0; i < 50; i++)
		  nums.add((int)(100 - Math.random()*201));
			
		//sorting code
	  for(int i = 0; i < 50; i++)
		  nums.sort(nums.list);
		
		nums.print(nums.list);
	}	
}
