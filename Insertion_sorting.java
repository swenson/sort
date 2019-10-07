import java.util.*;
public class Insertion_sorting {
    
	  @SuppressWarnings("resource")
	public static void main (String args[]) {
		  System.out.println("enter the numbers ");
		Scanner sw=new Scanner(System.in);
		int n=sw.nextInt();
		int arr[]=new int[n];
		for(int i=0;i<n;i++) {
			arr[i]=sw.nextInt();
		}
	        int i,j,k,min;
	                    
	            for(i=0;i<n-1;i++) {
	                  min=i;
	            	for (j=i+1;j>0;j--) {
	            	      if(arr[min]>arr[j]) {
	            	    	  min=j;
	            	      }}
	            	
	                  
	            	
	            }
	            	      
	               
	        
	        
	        
	        
	        for(i=0;i<n;i++) {
	        System.out.println(arr[i]);
	        }
	     
	  
	  }
	  
	  
}
