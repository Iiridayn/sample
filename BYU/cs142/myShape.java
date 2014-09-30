//Ged Crimsonclaw
//myShape.class
//Does the stuff(tm)

import java.awt.*;

abstract public class myShape	
{	
	abstract public void paint(Graphics page);
		
	protected int x, y;
	protected Color c;
	
	public myShape (int x, int y, Color c)
	{
		this.x = x;
		this.y = y;
		this.c = c;
	}
	
	public void drawCenter (Graphics page)
	{
		page.setColor(Color.black);
		page.drawString (x + " , " + y, x, y);
	}
}
