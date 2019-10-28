# Assignment 3

Milestone 0: CMake and Unit Testing filled out 
Due: Friday, September 20, 2019 11:59 PM

Milestone 1: PCB file loading and First Come, First Served 
Due: Friday, September 27, 2019 11:59 PM

Milestone 2: Priority scheduling, Round Robin, and analysis of algorithms 
Due: Friday, October 4, 2019 11:59 PM

Note: 
1. After you run assignment3_test, the PCBs.bin will be created in the ./test directory.
so in the ./build directory, you can run the analysis program like "./analysis ../test/PCBs.bin RR 4"; or you can simply copy and paste the PCBs.bin to the ./build directory, and then run "./analysis PCBs.bin RR 4".

2. You can manually copy the time analysis from console and paste it to this readme file, but directly output from your program is strongly recommended.     
---------------------------------------------------------------------------
Add your scheduling algorithm analysis below this line in a readable format. 
---------------------------------------------------------------------------

First Come First Serve Scheduling Results
Average wait time: 199.733337
Average turn around time: 213.300003
Total time: 407.000000

Priority Scheduling Results
Average wait time: 193.699997
Average turn around time: 207.266663
Total time: 407.000000

Round Robin Scheduling Results
Average wait time: 230.399994
Average turn around time: 243.966660
Total time: 407.000000

real    0m0.002s
user    0m0.000s
sys     0m0.002s

