/*********************************************************
 |                  Prog3b.class                         |
 |         (c) Ged Crimsonclaw, 9/20/2001                |
 |                                                       |
 *********************************************************/

public class Prog3b
{
	public static void main (String[] agrs)
	{
      //defines the two mutaions that I need
			String mute1, mute2;
			System.out.println ();
			
			//Fight song
			String Fight = ("Rise, all loyal Cougars\n" +
                        "And hurl your challenge to the foe.\n" +
                        "We will fight, day or night,\n" +
                        "Rain or snow.\n" +
                        "Loyal, strong and true,\n" +
                        "Wear the white and blue.\n" +
                        "As we sing, get set to spring.\n" +
                        "Come on Cougars, it's up to you!\n" +
                        "So Rise and Shout, the Cougars are out\n" +
                        "on the trail to fame and glory.\n" +
                        "Rise and shout, our cheers will ring out\n" +
                        "As you unfold your vict'ry story.\n" +
                        "on you go to vanquish the foe\n" +
                        "For Alma Mater's sons and daughters.\n" +
                        "As we join in song,\n" +
                        "In praise of you, our faith is strong.\n" +
                        "We'll raise our colors high in the blue\n" +
                        "And cheer our Cougars of BYU.\n" +
                        "Rah! Rah! Rah-rah-rah!\n" +
                        "Rah! Rah! Rah-rah-rah!\n" +
                        "Rah! Rah! Rah-rah-rah!\n" +
                        "GO COUGARS!!!");
		  System.out.println (Fight);
			 
			//Locating Chars
			System.out.println ();
			System.out.println ("The first \'B\' is at: " + Fight.indexOf ('B'));
			System.out.println ("The first \'Y\' is at: " + Fight.indexOf ('Y'));
			System.out.println ("The first \'U\' is at: " + Fight.indexOf ('U'));
			
			System.out.println ();
			System.out.println ("The fight song is " + Fight.length() + " chars in length.");
			
      mute1 = Fight.toUpperCase ();
			System.out.println ();
			System.out.println ("The ALL CAPS string returned a value of " + mute1.compareTo(Fight) + " when compared to the original.");
			
			//Changing the song...  =;-)
			mute2 = Fight.replace ('B', 'O');
			mute2 = mute2.replace ('Y', 'P');
			System.out.println ();
			System.out.println ("The new song:\n" + mute2);
			
			//Locations of new initials...
			System.out.println ();
			System.out.println ("The first \'O\' is at: " + mute2.indexOf ('O'));
			System.out.println ("The first \'P\' is at: " + mute2.indexOf ('P'));
			System.out.println ("The first \'U\' is at: " + mute2.indexOf ('U'));
	}
}
