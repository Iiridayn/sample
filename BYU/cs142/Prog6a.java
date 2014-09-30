/*********************************************************
 |                  Prog7class                           |
 |         (c) Ged Crimsonclaw, 10/10/2001               |
 |                                                       |
 *********************************************************/

import BankAccount;
import cs1.Keyboard;

public class Prog7
{
	public static void main (String[] agrs)
	{
		String ans, name;
		double money, value, checking, savings;
		int menu;
				
		System.out.print ("What is your name? ");
		name = Keyboard.readString ();
		
		BankAccount SA = new BankAccount (name, 500);
		BankAccount CH = new BankAccount (name);
		
		System.out.println ();
		System.out.println ("Accounts created.");
		System.out.println ();
		
		checking = CH.returnValue ();
		savings = SA.returnValue ();
		
		do
		{
			System.out.println ();
			System.out.println ("Main Menu for user " + name + ":");
			System.out.println ();
			System.out.println ("Checking Balance: " + checking);
			System.out.println ("Savings Balance: " + savings);
			System.out.println ();
			System.out.println ("1. Deposit Money to Checking");
			System.out.println ("2. Deposit Money to Savings");
			System.out.println ("3. Withdraw Money from Checking");
			System.out.println ("4. Withdraw Money from Savings");
			System.out.println ("5. Transfer Funds from Savings to Checking");
			System.out.println ("6. Transfer Funds from Checking to Savings");
			System.out.println ("7. Close Checking account");
			System.out.println ("8. Close Savings account");
			System.out.println ("9. Exit");
			System.out.println ();
			System.out.print ("Your choice: ");
			
			menu = Keyboard.readInt ();
			double amount;
			
			switch (menu)
			{
				case 1:
					System.out.print ("How much would you like to deposit to Checking? ");
					amount = Keyboard.readDouble ();
					checking = CH.deposit (amount);
					break;
				
				case 2:
					System.out.print ("How much would you like to deposit to Savings? ");
					amount = Keyboard.readDouble ();
					savings = SA.deposit (amount);
					break;
					
				case 3:
					System.out.print ("How much would you like to withdraw from Checking? ");
					amount = Keyboard.readDouble ();
					if ((CH.returnValue() - amount) >= 0)
						checking = CH.withdraw (amount);
					else
					{	
						System.out.println ();
						System.out.println ("Insufficent funds.");
					}
					break;
					
				case 4:
					System.out.print ("How much would you like to withdraw from Savings? ");
					amount = Keyboard.readDouble ();
					if ((SA.returnValue() - amount) >= 0)
						savings = SA.withdraw (amount);
					else
					{
						System.out.println ();
						System.out.println ("Insufficent funds.");
					}
					break;
					
				case 5:
					System.out.print ("How much would you like to transfer from Savings to Checking? ");
					amount = Keyboard.readDouble ();
					if ((SA.returnValue() - amount) >= 0)
					{	
						double temp = savings;
						savings = SA.withdraw (amount);
						if (temp != savings)
							checking = CH.deposit (amount);
						else
							System.out.println ("Account closed.  No transfer made.");
					}
					else
					{
						System.out.println ();
						System.out.println ("Insufficent funds.");
					}
					break;
					
				case 6:
					System.out.print ("How much would you like to transfer from Checking to Savings? ");
					amount = Keyboard.readDouble ();
					if ((CH.returnValue() - amount) >= 0)
					{	
						double temp = checking;
						checking = CH.withdraw (amount);
						if (checking != temp)
							savings = SA.deposit (amount);
						else
							System.out.println ("Account closed.  No transfer made.");
					}
					else
					{
						System.out.println ();
						System.out.println ("Insufficent funds.");
					}
					break;
				
				case 7:
					CH.close ();
					break;
					
				case 8:
					SA.close ();
					break;
				
				case 9:
					break; //for real
					
				default:
					System.out.println ();
					System.out.println ("I'm sorry, but that is not an option.");
					System.out.println ("Please choose an option on the menu.");
			}
		}
		while (menu != 9);
	}
}

