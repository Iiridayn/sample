public class random
{
  public static void main(String[] args)
	{
	  final int SIDES = 2;
		int i = 1;
		boolean worky = true;
		
		try
		{
		  i = Integer.parseInt(args[0]);
		}
		catch(Exception e)
		{
		  worky = false;
		}
		
		for(; i > 0; i--)
		  System.out.println("" + (int)((Math.random()*SIDES)+1));
	}
}
