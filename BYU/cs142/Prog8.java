import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;

public class Prog8 extends java.applet.Applet implements MouseListener, MouseMotionListener
{
	private int[] x = new int[100];
	private int[] y = new int[100];
	private int location = 0, setmod = 0;
	private Point temp;
	
	public void init()
	{
		addMouseListener (this);
		addMouseMotionListener (this);
		
		resize(300,300);
	}
	
	public void paint(Graphics page)
	{
		if (setmod == 1)
		{
			page.fillPolygon (x, y, location);
		}
		else
		{
			for (int i=0; i <= location; i++)
			{
				page.drawLine (x[i], y[i], x[(i+1)], y[(i+1)]);
			}
		}
	}
	
	public void mousePressed (MouseEvent event)
	{
		temp = event.getPoint();
		if (location == 0)	
		{
			x[location] = temp.x;
			y[location] = temp.y;
		}
		else
		{
			if (setmod == 0)
			{
				x[(location+1)] = temp.x;                
				y[(location+1)] = temp.y;
				repaint();
			}
			if ((Math.abs(x[0] - temp.x) <= 5) && (Math.abs(y[0] - temp.y) <= 5))
			{	
				location++;
				x[location] = x[0];
				y[location] = y[0];				
				setmod = 1;
			}
		}
	}
	
	public void mouseDragged (MouseEvent event)
	{
		if (setmod == 0)
		{
			temp = event.getPoint();
			x[(location+1)] = temp.x;
			y[(location+1)] = temp.y;
			
			if (location != 0)
			{
				if ((Math.abs(x[0] - temp.x) <= 5) && (Math.abs(y[0] - temp.y) <= 5))         
				{
					location++;
					x[location] = x[0];
					y[location] = y[0];
					setmod = 1;
				}
			}
			repaint();
		}
		else
		{
			temp = event.getPoint();
			for (int i=0;i<location;i++)
			{
				if ((Math.abs(x[i] - temp.x) <= 10) && (Math.abs(y[i] - temp.y) <= 10))
				{	
					x[i] = temp.x;
					y[i] = temp.y;
				}
			}
			
			repaint();
		}
	}
	
	public void mouseReleased (MouseEvent event)
	{
		if (setmod == 0)
			location++;
	}
	
	//Empty defs
	public void mouseClicked (MouseEvent event){}
	public void mouseEntered (MouseEvent event) {}
	public void mouseExited (MouseEvent event) {}
	public void mouseMoved (MouseEvent event) {}
}


