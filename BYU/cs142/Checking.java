/**********************************************************
 |                  Checking.class                        |
 |         (c) Ged Crimsonclaw, 10/11/2001                |
 |                                                        |
 *********************************************************/

import java.text.NumberFormat;

public class Checking implements Account
{
	private NumberFormat fmt = NumberFormat.getCurrencyInstance ();
	private double money;
	private String name;
	private int closed = 0;
	public static int checking = 0;
	
	public Checking (String name, double start)
	{
		this.name = name;
		money = start;
		checking++;
	}
	
	public Checking (String name)
	{
		this.name = name;
		money = 0;
		checking++;
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
