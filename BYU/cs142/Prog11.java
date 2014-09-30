import java.awt.*;
import java.awt.event.*;
import java.text.DecimalFormat;

/*
// Does a credit card interest calculator
*/

public class Prog11 extends java.applet.Applet implements ActionListener
{
	
	private double InitialBalance = 0, valueOfBalance, interestPercent, valueOfInput, interestTotal = 0, valueOfRate, payment, interest;
	private DecimalFormat fmt = new DecimalFormat ("0.00"), percent = new DecimalFormat ("0");
	private Label label1, label2, label3, label4, label5, balance, totalInterest, percentInterest;
	private TextField rate, balanceInput;
	private Button calculate;
	
	public void init()
	{
		calculate = new Button ("                Advance Month                    ");
		calculate.addActionListener (this);
		
		label1 = new Label ("Original Balance:");
		balanceInput = new TextField (15);
		balanceInput.addActionListener (this);
		
		label2 = new Label ("Interest Rate:              ");
		rate = new TextField (5);
		rate.addActionListener (this);
		
		label3 = new Label ("Balance:   ");
		balance = new Label ("                                  ");
		
		label4 = new Label ("Total Interest:  ");
		totalInterest = new Label ("                     ");
		
		label5 = new Label ("Interest relative to original balance:");
		percentInterest = new Label ("  ");
		
		add (calculate);
		add (label1);
		add (balanceInput);
		add (label2);
		add (rate);
		add (label3);
		add (balance);
		add (label4);
		add (totalInterest);
		add (label5);
		add (percentInterest);
		
		resize (255, 300);
	}

	public void actionPerformed (ActionEvent event)
	{
		String temp;
		
		temp = balanceInput.getText();
		valueOfInput = Double.parseDouble (temp);
		
		if ((InitialBalance != valueOfInput) || ((valueOfRate*100) != Double.parseDouble (rate.getText())))
		{
		  InitialBalance = valueOfBalance = valueOfInput;
			temp = rate.getText();
			valueOfRate = Double.parseDouble (temp);
			valueOfRate /= 100;
			interestTotal = 0;
			interestPercent = 0;
		}
		
		payment = (((valueOfRate * valueOfBalance) > 20)? (valueOfRate * valueOfBalance) : 20);
		
		valueOfBalance -= payment;
		interest = valueOfRate * valueOfBalance;
		
		if (!((valueOfBalance) <= 0))
		  valueOfBalance += interest;
		else
		{
			valueOfBalance = 0;
			interest = 0;
		}
		
		interestTotal += interest;
		interestPercent = interestTotal / InitialBalance;
		
		percentInterest.setText ("" + percent.format (interestPercent * 100) + "%");
		totalInterest.setText ("$" + fmt.format (interestTotal));
		balance.setText ("$" + fmt.format(valueOfBalance));
	}
}


