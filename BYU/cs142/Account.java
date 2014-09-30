/**********************************************************
 |                  Account.class                         |
 |         (c) Ged Crimsonclaw, 10/11/2001                |
 |           Demonstraights use of interfaces             |
 *********************************************************/

public interface Account
{
	public double deposit (double amount);
	public double withdraw (double amount);
	public double returnValue ();
	public void close ();
}
