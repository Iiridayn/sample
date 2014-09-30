import java.awt.*;

/*
	Ged Crimsonclaw is cool.  Created by him.  Makes pretty shapes.
*/

public class Prog10 extends java.applet.Applet
{
	myShape[] shapes;
	private final int SHAPES = 4;
	
	public void init()
	{
		shapes = new myShape[SHAPES];
		shapes[0] = new mySquare (45, 45, Color.green, 60);
		shapes[1] = new myCircle (175, 175, Color.blue, 60);
		shapes[2] = new myRectangle (70, 150, Color.red, 50, 80);
		shapes[3] = new myTriangle (150, 100, Color.yellow, 60);
		resize(300,300);
	}

	public void paint(Graphics page)
	{
		for (int i = 0;i<SHAPES;i++)
		{
			shapes[i].paint(page);
		}
	}
}


