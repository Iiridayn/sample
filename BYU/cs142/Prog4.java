/*********************************************************
 |                  Prog4.class                          |
 |         (c) Ged Crimsonclaw, 9/26/2001                |
 |                                                       |
 *********************************************************/

import java.util.Random;
import cs1.Keyboard;

public class Prog4
{
	public static void main (String[] agrs)
	{
		//all variables
		
		int menu = 1, count, guess, guess10, points, totalpts, random, random10, roundcount = 0, allscore = 0;
		
		while (menu == 1)
		{
			count = 0;
			totalpts = 0;
			System.out.println ();

			while (count < 10)
			{
			  count++;
								
				do
				{
					System.out.print ("I'm thinking of a number between" +
   	                          " one and 100.  Guess what it is:  ");
					guess = Keyboard.readInt ();
					if ((guess > 100) || (guess < 1))
						System.out.println ("I'm sorry, but you need to enter a guess between 1 and 100");
				}
				while ((guess > 100) || (guess < 1));
				
				guess10 = guess/10;
	
				Random generate =new Random();
				random = Math.abs (generate.nextInt() % 100 + 1);
				random10 = random/10;
	
				if (guess10 == random10)
				{
					points = 10 - Math.abs (random - guess);
				}
				else
					points = 0;
		
				totalpts += points;
				System.out.println ("The number was: " + random);
				System.out.println ("You earned " + points + " points.");
				System.out.println ("Your total (average) score so far is " + totalpts/count);
			}

			roundcount++;
			allscore += totalpts/count;
	
			System.out.println();
			System.out.println("Would you like to play another round?");
			System.out.print("Enter a \'1\' for yes, or a \'0\' for no: ");
			menu = Keyboard.readInt();
			System.out.println();
		
			do
			{
				switch (menu)
				{
					case 1:
						break;
					case 0:
						break;
					default:
						System.out.print("I\'m sorry, but I would like you to enter either a one or zero: ");
						menu = Keyboard.readInt ();
				 }		
			}
			while ((menu != 1) && (menu != 0));

		}

		System.out.println ("Thank you for playing.  Your average score for your " + roundcount +
									" rounds is " + (allscore/roundcount));

	}
}
