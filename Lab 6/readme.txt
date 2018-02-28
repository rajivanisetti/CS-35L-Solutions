IMPPLEMENTATION AND ISSUES

I started off including the pthread.h library and examining the main.c file for places I could utilize multithreading. 
I noticed the the main function had a series of nested for loops that seemed pretty optimal for our parallel purposes. 
I went ahead and factored out the bulk of the calculations and for loops into another function that I could pass 
to my pthreads. I noticed that the for loops were making calculations for each individual pixel's color values. 
I decided to divy up the work of each thread by the columns of the image, where each thread had its own respective 
column to calculate. 

I factored the calculations out into a function "void * multiThread(void * threadID)". I used the argument void 
pointer to cast to an int to get my thread ID. I also had to make sure to return NULL at the end, which caused some 
errors at first that I didn't know how to fix. Regrettably, it was a simple fix. The function essentially was 
the same as before, besides me changing some loop specifications to divy up the work of the threads evenly.
In addition, I switched the array initialization of the color values to my own global array so I could print
it in the main function. In addition, in order to make things compile, I had to change some variables in the 
main function to global variables, as both the main function and multiThread function referenced them. 
I had to make nthreads, scene, and the array of my color values global. I also changed some of the names a 
little just to make sure it wouldn't compile if I forgot to change something.

I then made sure to introduce the pthread_create and pthread_join functions in my main function, and used
loops to create and free them. After this, I simply looped through my color value array that was set during 
the multiThread function and printed out the three color values associated with the height and width. 

In terms of changes the makefile, I only had to add the -lpthread library.

I would say the main problems I faced were compilation errors at first, and my issue with the multiThread
function with the void pointers. Other than this, I would say the implementation of this embarrassingly 
parallel function was relatively unbuggy compared to previous labs. 

RESULTS

The results of multithreading were as expected. I tested with 1, 2, 4, and 8 threads and performance improved 
drastically each time. I first used the "make" command to compile the remade program (after numerous failures 
corrections). I then used the following commands with the resulting outputs.

time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m46.356s
user	0m46.345s
sys		0m0.005s

time ./srt 2-test.ppm > 2-test.ppm.tmp

real	0m23.296s
user	0m46.310s
sys		0m0.002s

time ./srt 4-test.ppm > 4-test.ppm.tmp

real	0m11.662s
user	0m46.357s
sys		0m0.005s

time ./srt 8-test.ppm > 8-test.ppm.tmp

real	0m5.981s
user	0m46.295s
sys		0m0.001s

As one can see, the program becomes progressively faster as we use more threads, with the 8-threaded version being 
almost 8x as fast as the single threaded version. Our parallelization was a success. 





