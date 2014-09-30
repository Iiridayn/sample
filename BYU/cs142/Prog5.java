/*********************************************************
 |                  Prog5class                           |
 |         (c) Ged Crimsonclaw, 10/3/2001                |
 |                                                       |
 *********************************************************/

import cs1.Keyboard;

public class Prog5
{
	public static void main (String[] agrs)
	{
		long input = 0, last, decNumber = 0, bin;
		String Input, binNumber = "";
		
		System.out.print ("Please enter a 16-bit binary number, or a decimal number between 0 and 32767: ");
		Input = Keyboard.readString();
		
		do
		{
			System.out.print ("Enter 1 for binary, 2 for decimal: ");
			bin = Keyboard.readLong ();
		}
		while ((bin != 1) && (bin != 2));
		
		input = Long.parseLong(Input);
		
		if (bin == 2)
		{
			int count = 0;
			while (input > 0)
			{	
				last = input%2;
				binNumber = (last + binNumber);
				
				input /= 2;
				count++;
			}
			
		}
		
		else
		{
			for (int count = 0; count < 16; count++)
			{
				last = input % 10;
				input /= 10;
				decNumber += (last * (Math.pow(2, count)));
			}
		}
		
		System.out.println ();
		if (decNumber != 0)
			System.out.println (Input + " in decimal is: " + decNumber);
		else	
			System.out.println (Input + " in binary is: " + binNumber);
	}
}
