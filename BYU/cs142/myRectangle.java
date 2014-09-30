//	Ged Crimsonclaw
//	myRectangle.class
//	Draws a Rectangle

import java.awt.*;

public class myRectangle extends myShape
{
	private int length, width;
	
	public myRectangle (int x, int y, Color c, int length, int width)
	{
		super (x,y,c);
		this.length = length;
		this.width = width;
	}
	
	public void paint (Graphics page)
	{
		page.setColor(c);
		page.drawRect(x-length/2, y-width/2, length, width);
		drawCenter(page);
	}
}
