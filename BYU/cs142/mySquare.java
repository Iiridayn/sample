//	Ged Crimsonclaw
//	mySquare.class
//	Draws a Square

import java.awt.*;

public class mySquare extends myShape
{
	private int size;
	
	public mySquare (int x, int y, Color c, int size)
	{
		super (x,y,c);
		this.size = size;
	}
	
	public void paint (Graphics page)
	{
		page.setColor(c);
		page.drawRect(x-size/2, y-size/2, size, size);
		drawCenter(page);
	}
}
