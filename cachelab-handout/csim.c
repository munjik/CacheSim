/* Munji Kahalah
 * NetID: 101706335
 * 04/15/2018
 * The Cache L1ab */

 #include <stdio.h>
 #include <unistd.h>
 #include "cachelab.h"
 #include <stdlib.h>
 #include <getopt.h>
 #include <math.h>


/* functions declared up here */
 void showUsage();

 /* General organization of cache (S, E, B, m).
    (a) A cache is an array of sets.
    Each set contains one or more lines.
    Each line contains a valid bit, some tag bits, and a block of data.
    Start by making structs for the three described above */

  /* Our cache line */
  typedef struct {
    int validBit;
    unsigned long long tag;
    int bData;
  }  lineCache;

  /* cache set */
  typedef struct {
    lineCache *newLines;
  } setCache;

  /* the cache */
  typedef struct {
    setCache *arraySets;
  } theCache;

  /* the powers: 2^s and 2^b, according to the book on how to calculate */
  typedef struct {
    int B;
    int S;
  } theBigs;


 int main(int argc, char *argv[]) {

   /* the options on the command line the user will type in */
   int choices, s, E, b;
  //  int hit_count;
  //  int miss_count;
  //  int eviction_count;

   /* see line 100 and 101  */
   theCache cacheCache;
   /* these variabels are taken from CMU, in part of the fscanf example */
   char operation;
   unsigned long long address;
   int size;

   FILE *pFile; // pointer to file object

   /* we have colons to give them arguements */
   while(-1 != (choices = getopt(argc, argv, "hvs:E:b:t:"))) {
     switch (choices) {
      /* first two (h and v) will not take the atoi(optarg) function */
       /* -h: Optional help flag that prints usage info */
       case 'h':
        showUsage();
        break;
       /* -v: Optional verbose flag that displays trace info */
      //  case 'v':
      //           v = 1;
      //           break;

      /* -s <s>: Number of set index bits (S = 2s is the number of sets) */
       case 's':
        s = atoi(optarg);
        break;

      /* -E <E>: Associativity (number of lines per set)*/
       case 'E':
        E = atoi(optarg);
        break;

      /* -b <b>: Number of block bits (B = 2b is the block size) */
       case 'b':
        b = atoi(optarg);
        break;

      /* -t <tracefile>: Name of the valgrind trace to replay */
       case 't':
        pFile = fopen(optarg, "r");
        break;
        /* make sure arguments are correct */
       default:
          printf("Invalid arguemnts, try again...\n");
     }
   }
   /* calculating our powers 2^s and 2^b */
   theBigs big;
   big.S = pow(2, s);
   big.B = pow(2, b);

   /* We will be allocating the memory for the number of sets and then
   for the lines in each set. E = 1 line per set */
   cacheCache.arraySets = malloc(big.S * sizeof(setCache));
   for (int j = 0; j < big.S; j++) {
     cacheCache.arraySets[j].newLines = malloc(E * sizeof(lineCache));
   }

   /* if null print out our usage and inform user that no file has been traced*/
   if (pFile == NULL) {
     printf("NO FILE TO TRACE\n");
     showUsage();
   }
   else {
     while(fscanf(pFile, "%c %llx,%d", &operation, &address, &size) > 0) {
       /* we have three operations: M L S and we ignore 'I' */
       if ((operation != 'I') && (operation == 'M' || operation == 'L' || operation == 'S')) {
         printf("%c %llx %d", operation, address, size);


       }

     }

   }

   /* Just prints out the file once read */
  //  while (1) {
  //    c = fgetc(pFile);
  //    if(feof(pFile)) {
  //      break;
  //    }
  //    /* test for copying the file and printing it onto command line*/
  //   printf("%c", c);
  //  }
   printSummary(0, 0, 0);
   fclose(pFile); // close file
   return 0;
 }

 /* error usage */
 void showUsage()
 {
   printf("Specific Error [to be displayed here]\n");
   printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
   printf("Options\n");
   printf("-h          Print this help message.\n");
   printf("-v          Optional verbose flag.\n");
   printf("-s <num>    Number of set index bits.\n");
   printf("-E <num>    Number of lines per set.\n");
   printf("-b <num>    Number of block offset bits.\n");
   printf("-t <file>   Trace file.\n");
 }
