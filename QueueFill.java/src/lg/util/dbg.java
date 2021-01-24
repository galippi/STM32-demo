package lg.util;

public class dbg {
    public static void println(int level, String str)
    {
        if (level <= debugLevel)
        {
            System.out.println(str);
        }
    }
    public static void set(int _debugLevel)
    {
        debugLevel = _debugLevel;
    }
    static int debugLevel;
}
