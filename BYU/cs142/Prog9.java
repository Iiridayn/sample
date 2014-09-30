import java.awt.*;
import javax.swing.Timer;
import java.awt.event.*;

public class Prog9 extends java.applet.Applet
{
	final int MAXSIZE = 5, DELAY = 100;
	int x = ((MAXSIZE/2)), y = (MAXSIZE - 1);
	int[][] loc = new int[MAXSIZE][MAXSIZE];
	Timer timer = new Timer (DELAY, new the_timer_class());	
	
	public void init()
	{
		resize((MAXSIZE+2) * 30,(MAXSIZE+3) * 30);
		
		for (int i = 1; i <= (MAXSIZE*MAXSIZE);i++)
		{
			if (x >= MAXSIZE)
				x = 0;
			
			if (y >= MAXSIZE)
				y = 0;
			
			if (loc[x][y] != 0)
			{
				y-=2;
				x--;
				if (x<0)
					x+=5;
				if (y<0)
					y+=5;
			}
			
			loc[x][y] = i;
			y++;
			x++;
		}
		timer.start();
	}
	
	public void paint(Graphics page)
	{
		int r, g , b;
		for (int i=0; i < MAXSIZE; i++)
		{	
			for (int j=0; j < MAXSIZE; j++)
			{
				r = (int) (Math.random()*256);
				g = (int) (Math.random()*256);
				b = (int) (Math.random()*256);
				page.setColor (new Color (r, g, b));
				page.drawString("" + loc[i][j], (i+1) * 30, (j+1) * 30);
			}
		}
		
		page.setColor (new Color (0, 0, 0));
		page.drawString ("Sum: " + tester(), 30, (MAXSIZE+1)*30 + 15);
	}
	
	//mess below, affects above
	public int tester ()
	{
		int sum = 0, temp;
		boolean bool;
		for (int i = 0;i < MAXSIZE;i++)
			sum += loc[i][i];
		temp = sum;
		sum = 0;
		for (int i = 0, j = (MAXSIZE-1); i <MAXSIZE && j>=0;i++, j--)
			sum += loc[i][j];
		if (temp == sum)
			bool = true;
		else
			bool = false;
		sum = 0;
		for (int i = 0; i < MAXSIZE; i++)
		{	
			sum = 0;
			for (int j = 0; j < MAXSIZE; j++)
				sum += loc[i][j];
			if (temp == sum)
				bool = true;
			else
				bool = false;
		}
		for (int i = 0; i < MAXSIZE; i++)
		{	
			sum = 0;
			for (int j = 0; j < MAXSIZE; j++)
				sum += loc[j][i];
			if (temp == sum)
				bool = true;
			else
				bool = false;
		}
		if (bool)
			return sum;	
		else
			return 0;
	}
	private class the_timer_class implements ActionListener
	{
		public void actionPerformed (ActionEvent event)
		{
			repaint();
		}
	}
}


