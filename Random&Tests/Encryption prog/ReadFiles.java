import java.io.File;
import java.util.Scanner;
public class ReadFiles
{
  public static void main(String[] args) throws Exception
  {

    System.out.println(readf(args[0]));


  }

  public static Scanner readf(String path){
    File file = new File(path);
    Scanner sc = new Scanner(file);
    sc.useDelimiter("\\Z");
    return sc.next();
  }


}
