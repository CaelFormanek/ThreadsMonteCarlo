#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <ctype.h>

/* for threads */
void* calculate_hits(void* param);

/* function declaration for getting a random double */
double random_double();

/* global variable for number of points */
int npoints;

/* global variable for number of points inside the circle */
int hit_count = 0;

int main(int argc, char** argv)
{
  /* check the correct number of command line args */
  if (argc != 2)
  {
    printf("You must provide the name of the program and the number of points to be generated, nothing more or less.\n");
    return 1;
  }

  /* make sure the argument is an integer */
  int a = 0;
  for (a = 0; a < strlen(argv[1]); a++)
  {
    if (!isdigit(argv[1][a]))
    {
      printf("You entered %s, but you must enter an integer number (no decimals or extra characters).\n", argv[1]);
      return 1;
    }
  }

  /* establish npoints */
  npoints = atoi(argv[1]);

  /* make at least two threads, each will do half the number of points */
  pthread_t tid0;
  pthread_attr_t attr0;
  pthread_attr_init(&attr0);

  pthread_t tid1;
  pthread_attr_t attr1;
  pthread_attr_init(&attr1);

  /* in the function, if the number of points is odd, tid0 will do 1 more point */
  int i = 1;
  pthread_create(&tid0, &attr0, calculate_hits, &i);
  pthread_create(&tid1, &attr1, calculate_hits, NULL);

  pthread_join(tid0, NULL);
  pthread_join(tid1, NULL);

  /* output estimation of pi */
  double pi = 4.0 * ((hit_count*1.0) / (npoints*1.0));
  printf("Estimation of pi: %f\n", pi);
}

/* implementation for randomdouble */
double random_double()
{
  return random() / ((double)RAND_MAX + 1);
}

 /* check for points inside circle */
void* calculate_hits(void* param)
{
  /* this section checks if the number of points is odd so the correct number is generated */
  int numpoints = npoints / 2;
  if (((npoints % 2) == 1) && (param != NULL))
  {
    ++numpoints;
  } 
  int i = 0;
  int hits = 0;
  for (i = 0; i < numpoints; i++)
  {
    /* generate points between -1.0 and 1.0 */
    double x = random_double() * 2.0 - 1.0;
    double y = random_double() * 2.0 - 1.0;
    if (sqrt(x*x + y*y) < 1.0)
    {
      ++hits;
    }
  }
  hit_count += hits;
  pthread_exit(0);
}