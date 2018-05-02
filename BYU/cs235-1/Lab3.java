// Made by: Michael Clark, Ged Crimsonclaw
// Date: 3/11/2002
// Comments: Navigates a maze.  Cool.  MINE.  =;-P

import java.io.*;
import java.util.StringTokenizer;

public class Lab3
{
	static list path = new list();
	static list theHead = path;
	
	public static void main(String[] args) throws FileNotFoundException, IOException
	{
		String infile = "";
		boolean worky = true;
		list visited = null;
		list head = visited;
		list maze = new list();
		list temp = maze;
		
		
		BufferedReader stdin = new BufferedReader (new InputStreamReader (System.in));
		
	  try{
			infile = args[0];
		}
		catch(Exception e)
		{
			worky = false;
		}
		
		if(worky == false)
		{
			System.out.print("Please enter the name of the file that your maze is stored in: ");
			infile = stdin.readLine();
		}
		
		BufferedReader mazefile = new BufferedReader (new FileReader (infile));
		String line;
		int i = 0;
		boolean yabba = true;
		
		do
		{
		  i++;
			if((line = mazefile.readLine()) == null)
				yabba = false;
			else
			{
				temp.next = new list();
				add(line, temp.next);
				temp = temp.next;
			}
		}
		while(yabba);
		
		maze = maze.next; //These two are a quick fix
		i--;		
		
		boolean isTrue = navigate(maze, visited, i, maze);
		String output = "";
		
		theHead = theHead.next; //see the above comment
		if(!isTrue)
			System.out.println("No path found");
		else
			while(theHead != null)
			{
				output = theHead.value + "\n" + output;
				theHead = theHead.next;
			}
		
		System.out.println(output);
		System.out.println();
		System.out.println(); //formatting prettier
	}
	
	static boolean navigate (list maze, list visited, int last, list header)
	{
		list head = new list();
		head.value = maze.value;
		head.next = visited;
				
		if(maze.value == last)
		{
			path.next = new list();
			path = path.next;
			path.value = maze.value;
			return true;
		}
		
		while(maze.up != null)
		{
			if(!isVisited(maze.up.value, head))
				if(navigate(find(header, maze.up.value), head, last, header))
				{
					path.next = new list();
					path = path.next;
					path.value = maze.value;
					return true;
				}
				
				maze.up = maze.up.next;
		}
		
		return false;
	}
	
	static void add(String line, list maze)
	{
		StringTokenizer st = new StringTokenizer(line);
		maze.value = Integer.parseInt(st.nextToken());
		list head = null;
		list last = null;
		list temp = null;
		
		while(st.hasMoreTokens())
		{
			temp = new list();
			temp.value = Integer.parseInt(st.nextToken());
			if (head == null) {
				head = temp;
				last = temp;
			}
			else {
				last.next = temp;
				last = last.next;
			}
		}
		maze.up = head;
	}
	
	static list find(list maze, int toFind)
	{
		list temp = maze;
		
		if(temp != null)
		{
			if(temp.value == toFind)
				return temp;
			else
				return find(temp.next, toFind);
		}
		return null;
	}
	
	static boolean isVisited(int toFind, list visited)
	{
		if (find(visited, toFind) == null)
			return false;
		else
			return true;
	}
	
	static void tester(list maze){
		list temp = maze;
		while(temp != null){
			System.out.print("" + temp.value);
			
			while(temp.up != null){
				System.out.print(" " + temp.up.value);
				temp.up = temp.up.next;}
			System.out.println();
			
			temp = temp.next;}
	}
}

class list
{
	int value;
	list up;
	list next;
	
	list(){}
}
