/*
Made by: Ged Crimsonclaw, Michael Clark
Date: 12/13/2001
Purpose: Testing my knowledge of linked lists.
*/

public class Lister
{
  public ListerNode list;
	
	Lister()
	{
		list = null;
	}
	
	public void add (int newInt)
	{
		ListerNode node = new ListerNode (newInt);
		ListerNode current;
		
		if (list == null)
			list = node;
		else
		{
			current = list;
			while(current.next!= null)
				current = current.next;
			current.next = node;
		}
	}
	
	public void print(ListerNode current)
	{
	  String result = "";
		
		System.out.println(current.Int);
		
		if(current.next != null)
		  print(current.next);
	}
	
	public void sort(ListerNode current)
	{
	  if (current.next != null)
		{
		if(current.Int<current.next.Int)
		{
		  int temp = current.Int;
			current.Int = current.next.Int;
			current.next.Int = temp;
		}
		
		current = current.next;
		
		if(current.next != null)
		  sort(current);
		}
	}
	
	private class ListerNode
	{
	  public int Int;
		public ListerNode next;
		
		public ListerNode(int newInt)
		{
		  Int = newInt;
			next = null;
		}
	}
}	

