#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<sys/ipc.h>
#include<semaphore.h>
 
#define N 5
 
time_t end_time;/*end time*/
sem_t mutex,students,professors;/*Three semaphors*/
int Numberstudents=0;/*The number of students waiting for answers*/
 
void professor(void *arg);
void student(void *arg);
void questiondone();
void questionstart();
void answerstart();
void answerdone();
void questionend();
 
int main(int argc,char *argv[])
{
	pthread_t id1,id2;
	int status=0;
	end_time=time(NULL)+20;/*professor*/
 
	/*Semaphore initialization*/
	sem_init(&mutex,0,1);
	sem_init(&students,0,0);
	sem_init(&professors,0,1);
 
	/*professor_thread initialization*/
	status=pthread_create(&id1,NULL,(void *)professor,NULL);
	if(status!=0)
		perror("Create professors is failure!\n");
	/*student_thread initialization*/
	status=pthread_create(&id2,NULL,(void *)student,NULL);
	if(status!=0)
		perror("Create students is failure!\n");
 
	/*student_thread first blocked*/
	pthread_join(id2,NULL);
	pthread_join(id1,NULL);
 
	exit(0);
}
 
void professor(void *arg)/*professor Process*/
{
	while(time(NULL)<end_time || Numberstudents>0)
	{
		sem_wait(&students);/*P(students)*/            
		sem_wait(&mutex);/*P(mutex)*/
		
		Numberstudents--;
		printf("\nProfessor:Number of Students %d.",Numberstudents);
		printf("\n");
		answerstart();	
		//if (Numberstudents == 1)
				//{
				//	questionend();
				//	break;
				//}		
		sem_post(&mutex);/*V(mutex)*/
		sem_post(&professors);/*V(professors)*/		
		sleep(3);  
		
		
			
	}
	
}
 
void student(void *arg)/*students Process*/
{
	while(time(NULL)<end_time)
	{
		sem_wait(&mutex);/*P(mutex)*/
		if(Numberstudents<N)
		{
			Numberstudents++;
			printf("\nStudent:Number of Sudents:%d",Numberstudents);
			printf("\n");
			questionstart();
			sem_post(&mutex);/*V(mutex)*/
			sem_post(&students);/*V(students)*/
			sem_wait(&professors);/*P(professors)*/
			
		}
		else
			/*V(mutex)*/
			/*If the number is full of students,just put the mutex lock let go*/
			sem_post(&mutex);
				
		sleep(1);
	}
}

void questionend()
{
	printf("\nThere seems to be no more questions. Thank you for coming.");
}

void anwerdone()
{

	// we must wait for the prof to be done
        printf("I am  thinking for an answer.");   
       
       
        
        
}


void answerstart()
{
	if (Numberstudents >0)
				{
					printf("\nProfessor says: Interesting question... Let me think.");
				}
	else
	{
		questionend();
	}

	
         char answers[12][100] = 
        {
            "42", "I'm not sure.", "Definitively.", "I do not understand your question.",
            "Please, come back on Monday.", "Sorry, I'm retiring tomorrow.", 
            "You need to restate your question.", "If I knew I would tell you.",
            "PI divided by 4.", "0,87 radians.", "That was not a question, try again!"
        };
        
     
	 srand((unsigned)time(NULL));  
     int x = 0;
	 x = rand()%12;  
	 printf("\nMy answer is:%s",answers[x]);


}



void answerdone()
{

	printf("Professor says: I hope that helped.");
}


void questionstart() 
{
	printf("\nStudent says: Professor I have a question....");
	
        char questions[12][100] =
        {
            "What is the 1001th prime number?", "What is the last digit of PI?", "What is the answer for all questions?",
            "How can one compute the real component of the square root of -1?", "Why do we use greek letters instead of latin ones?",
            "What is the sound of one hand?", "Why is the sky blue?", "Why do we always see the same side of the moon?",
            "What's on the deep end of the pool?", "Do babies really come from Paris?"
        };

     srand((unsigned)time(NULL));  
     int x = 0;
	 x = rand()%12;  
	 printf("\nMy question is:%s",questions[x]);
        
} 
