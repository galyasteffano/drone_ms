
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "drone.h"
pthread_mutex_t mymutex;

void *goDrone(void *threadid)
{
	srand(time(NULL));
	int state=1;
 	int randInt;
	long tid;
	tid =(long)threadid;
	int x,y;
	x=0;
	y=0;
	x=rand()%50;
	y=rand()%50;		
	int final=4;//at leats 4
	
    
	while(state !=18)
	{
		switch(state)
		{
			case 1: // register drone
			{
				randInt=rand() %15; //very small chance of error
				if(randInt==0)    //state is entered  beacause of error
			 	{                           // the drone was not registered
		    			state=1;  
		    			printf("DRONE:%ld IS NOT REGISTERED\n",tid);
		 		}
				else
			 	{	
		    		printf("DRONE:%ld REGISTERED\n",tid);
					state=2;
		  		}
				break;
			}
	 		case 2:  //Activate drone
		   	{	 
		  		randInt=rand() %50; // error drone was not activated
		 		if(randInt==0)
		 	  	{	
		   			state=2;
		   			printf("DRONE:%ld NOT ACTIVATED\n",tid);
		   	  	}
		 		else
		        {
                    printf("DRONE:%ld ACTIVATED\n",tid);
					state=3;
                }    
				break;
			}
	 		case 3: // loading address
		    {
	         	randInt=rand()%50;  
				if(randInt==0)  		//if because of an error the address was not loaded don't go to the next
			  	{	           			//state
                    state=3; 			//stay at current state until address is loaded
                    printf("DRONE:%ld MISSING ADDRESS...\nWAITING FOR ADDRESS TO LOAD..\n",tid);
                }
                else
                {  
		     		printf("DRONE:%ld LOADED ADDRESS\n",tid);
					state=4; 
		        }
				break;
			}
			case 4: 					// Drone at gate
        	{	
                printf("DRONE:%ld REQUESTING CLEARANCE..\n",tid);
                state=5;
                break;
			}
			case 5:						//cleared to take off
			{
			 	randInt=rand() %50;
		 	 	if(randInt==0)  		//checking for possible collision
		 	   	{
		   			state=14;
    		   			
		  	   	}
		 		else      				//no possible collision, okay to take off
		 	  	{ 
		  			printf("DRONE:%ld CLEARED TO TAKE OFF\n",tid );
					state=6;
		  	  	}
				break;
			}

	 		case 6: 					//Take off
            {
			     printf("DRONE:%ld TAKING OFF\n",tid);
		   	     state=7; //reach set altitude
				 break;
			}
	 		case 7:  					//reached allowed altitude
		    {
				randInt=rand() %15;
		 		if(randInt==0)       	//check for collision
		 	  	{
		   	  		state=14;     
 		   		} 
		 		else
		   	 	{
		     		printf("DRONE:%ld REACHED ALLOWED ALTITUDE\n",tid);
					state=8; 			//fly state
		   		}	
				randInt=rand()%50;
				if(randInt==0)
				{
					printf("DRONE:%ld HAS NOT REACHED MAX ALTITUDE YET\n",tid);
				}
				break;
			}
	 		case 8: 					//Fly
		     {
				randInt=rand() %2; 		//while flying keep checking for objects ahead     
				if(randInt==0)
		        {
		   			state=14;
		   		}   
				else
		   	 	{
					state=16; 			//stay in this state until address is reached
		       	}		
				break;
			}
	 		case 9: 					//address reached
            {
				randInt=rand() %100;
	        	if(randInt==0)
	            {
					state=9;    //stay in this state;checking for DESTINATION
	          	}
	         	else			
			    {  
		  			printf("DRONE:%ld REACHED ADDRESS\n",tid);
					state=10;
	            }
                break;
			}	
	 		case 10:				// Landing 
		    {
				randInt=rand() %50;
				if(randInt==0) //before you land check for collisions
				{  		   
					printf("DRONE:%ld LOOKING FOR PLACE TO LAND\n",tid);
					   
		  		}
				else 
		  	 	{
		      		state=11;		//deliver the package
		      		printf("DRONE:%ld LANDING...\n",tid);
		      			
	            }
				break;
			}
	 		case 11:				//deliver
 		     {
                randInt=rand() %100; 
                if(randInt==0)  
                {
					state=11;  		//stay in this state until payload is dropped
	           		printf("DRONE:%ld PAYLOAD IS BEING DELIVERED\n",tid);
                }
                else
                {  
                    state=12;
                   	printf("DRONE:%ld PAYLOAD DELIVERED\n",tid);
                }
                break;
	          }
	 		case 12:				//return home
            {
				randInt=rand() %20;
	         	if(randInt==0)
				{	
	              	printf("DRONE:%ld CHECKING FOR COLLISIONS\n",tid);
					state=14;		//if there is object detected go to state 14
				}
	            else  				//no objects on way, go home
	           	{
	             	printf("DRONE:%ld DRONE TAKING OFF\n",tid);
					printf("DRONE:%ld REACHED ALTITUDE\n",tid);
	             	state=17; 		//fly until DESTINATION is reached
	            }
		    	break;
			} 
	 		case 13:				//ERROR state in case of malfunction
            {       
				pthread_mutex_lock(&mymutex);
	         	state=13;
	         	printf("DRONE:%ld cannot deliver\n",tid);
	         	printf(" Emergency landing...\n sending coordinates..\n");
	         	state=1;  //start over
				pthread_mutex_unlock(&mymutex);
			break;
			}
			case 14: 						//detected object ahead,
            {
	           	randInt=rand() %20;
	           	if(randInt==0)
	            {
					state=13;  		
				}
	          	else
	            { 
					printf("DRONE:%ld DETECTED OBJECT AHEAD\n",tid);
					state=15;
				}
				break;
			}
         		
			case 15:						// go around target
		     {
				randInt=rand() %50;			//small chance for emergency landing
	           	printf("DRONE:%ld AVOIDING COLLISION..\n",tid);
				if(randInt==0)
				{
					pthread_mutex_lock(&mymutex); //lock this section	
					x=x-2;
					printf("DRONE:%ld GOING WEST[%d][%d]\n",tid,x,y);
					x--;
			
					printf("DRONE:%ld GOING WEST[%d][%d]\n",tid,x,y);
					x--;
		
					printf("DRONE:%ld GOING WEST[%d][%d]\n",tid,x,y);
		
					pthread_mutex_unlock(&mymutex);  //unlock it
				}
				else
				{
					pthread_mutex_lock(&mymutex); //lock this
					x=x+2;
					printf("DRONE:%ld GOING EAST[%d][%d]\n",tid,x,y);
					x++;
					printf("DRONE:%ld GOING EAST[%d][%d]\n",tid,x,y);
					x++;
					printf("DRONE:%ld GOING EAST[%d][%d]\n",tid,x,y);
					//NOTE:put a loop here!!
					pthread_mutex_unlock(&mymutex);
                }
				state=16;
                break;
     			}
			case 16:							//navigate state
			{   
				state=16;
				randInt=rand()%3;
				final=rand()%14; 				//up to 10 print lines
				while(final==0 && final==1 )
				{
					final=rand()%14;
				}	
				int count=0;
                
				while(count!=final)
				{
					printf("DRONE:%d FLYING[%d][%d]\n",tid,x,y);
					x++;
					y++;
					sleep(2);
         			count++;
				}
				state=9;
				break;
			}
			case 17:
			{	
				final=rand()%14;
				while(final==0 && final==1)  //final must be at least 2 units big to display some path
				{
					final=rand()%14;  		//if not keep randomizing final intill 
				}		
				state=17;
				int x1=10;
				int y1=10;
				int count1=0;
				count1=final;
				x1=rand()%50;
				y1=rand()%50;
				while(count1>=0)
				{
					printf("DRONE:%ld FLYING[%d][%d]\n",tid,x1,y1);
					x1--;
					y1--;
					sleep(2);	
					count1--;
				}
				printf("DRONE:%ld LANDING\n",tid);
				printf("DRONE:%ld BACK AT STATION\n",tid);
				state=18;//exit 
				break;
			}		
		}
	 	sleep(2);
      }   
  	pthread_exit(NULL);
   

  }
int main()
 {
	int numThreads;
	printf("How many drones shall I run?\n");
	scanf("%d",&numThreads);
       
	pthread_t *threads=(pthread_t*) malloc(sizeof(pthread_t)*numThreads);
    	int rc;
    	long t;
    	for(t=0;t<numThreads;t++)
		{
			sleep(4);
			rc=pthread_create(&threads[t],NULL, goDrone,(void *)t);
        	if(rc)
            {
				printf("ERROR; return code from pthread_create() is %d\n",rc);
				return -1;
			}                  
         }
		for(t=0;t<numThreads;t++)
		{
			pthread_join(threads[t],NULL);
		}
		free(threads);
     return 0;
}











 














