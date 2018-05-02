//**************************************//
//     Written by Ged Crimsonclaw       //
//    A simple hash value generator     //
//**************************************//

class hashgen
{
	public static void main(String[] args)
	{
		int buckets;
		
		try
		{
			buckets = Integer.parseInt(args[0]);
		}
		catch(NumberFormatException n)
		{
			System.out.println("Please specify the number of buckets as the first argument");
			return;
		}
		catch(ArrayIndexOutOfBoundsException i)
		{
			System.out.println("Program must be run with some arguments!");
			return;
		}
		
		int base = 1;
		int tops = args.length;
		try //can specify the number of the first bucket (ie 0 or 1) as last argument
		{
			base = Integer.parseInt(args[--tops]);
		}
		catch(Exception e)
		{
			tops++;
		}
		
		int total;
				
		for(int i = 1; i < tops; i++)
		{
			total = 0;
			System.out.print(args[i] + ": ");
			int temp = args[i].length();
			for(int j = 0; j < temp; j++)
			{
				total += (int)(args[i].charAt(j));
			}
			System.out.println((total%buckets)+base);
		}//for
	}//main
}//hashgen
