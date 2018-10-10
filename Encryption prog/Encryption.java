import RSA.*;

public class encrypt{
  public static void main(){
    public static void main(String [] args){
  	// generate public and private keys
          KeyPair keyPair = RSA.buildKeyPair();
          PublicKey pubKey = keyPair.getPublic();
          PrivateKey privateKey = keyPair.getPrivate();

  		String str = readFile("exemple.txt",US-ASCII);
  		System.out.println(str);


  	}
  }
}
