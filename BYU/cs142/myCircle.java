//	Ged Crimsonclaw
//	myCircle.class
//	Draws a circle

import java.awt.*;

public class myCircle extends myShape
{
	private int radius;
	
	public myCircle (int x, int y, Color c, int radius)
	{
		super (x,y,c);
		this.radius = radius;
	}
	
	public void paint (Graphics page)
	{
		page.setColor(c);
		page.drawOval (x-(radius/2), y-(radius/2), radius, radius);
		drawCenter(page);
	}
}
