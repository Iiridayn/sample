/*********************************************************
 |                  BankAccount.class                     |
 |         (c) Ged Crimsonclaw, 10/11/2001                |
 |                                                        |
 *********************************************************/

import java.text.NumberFormat;

public class BankAccount
{
	private NumberFormat fmt = NumberFormat.getCurrencyInstance ();
	private double money;
	private String user;
	public int closed = 0;
	
	public BankAccount (String name, double start)
	{
		user = name;
		money = start;
	}
	
	public BankAccount (String name)
	{
		user = name;
		money = 0;
	}
	
	public double deposit (double amount)
	{
		if (closed == 1)
		{
			System.out.println ();
			System.out.println ("I'm sorry, but you have closed this account.");
		}
		else
		{
			money += amount;
			return money;			
		}
		return money;
	}

	public double withdraw (double amount)
	{
		if (closed == 1)
		{	
			System.out.println ();
			System.out.println ("I'm sorry, but you have closed this account.");
		}
		else
		{
			if ((money - amount) >= 0)
			{
				money -= amount;
				return money;
			}
			else
			{	
				System.out.println ();
				System.out.println ("I'm sorry, but you don't have the money");
			}
		}
		return money;
	}
	
	public double returnValue ()
	{
		return money;
	}

	public void close ()
	{
		closed = 1;
	}
	
}
