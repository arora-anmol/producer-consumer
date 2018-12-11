# producer-consumer
Practicing Inter-Process/Inter-thread communication, by solving the producer-consumer problem. Also known as the bounded buffer problem, we are trying to analyse how both threads/ processes perform when we have a buffer of a fixed size (B), number of elemnents(N),  Consumers (C) and Producers (P). This was one of the labs for my operating systems course(ECE-254), and I added some extra stuff to it. Credits to the lab TAs and the course instructor for the help throughout this amazing course.

# What is it?
We are practicing inter-process communication in both threads and processes. I have used message queues for processes, and shared memory for threads. We create a buffer of size **B**, and then **P** producers produce a total of **N** elements.      **C** consumers consume the items in the buffer, and we stop once we are done consuming/ producing them.

## How to run? 
collect_data.py is used to run the different cases and build the results csv. You can run it inside either src/threads directory or src/processes directory. <br>
For example, if we want to do it for threads, you'll go to project dir and run ``` cd src/threads``` to get in the threads dir <br>
To create the ./produce.out file: ```make```  <br>
To run tests and collect data: ``` python collect_data.py ``` 

This will generate a csv which can be used for results. 
Credits to Lab Instructors/TAs for buiding the python script for data collection. I have added some more test cases than the usual.  

## Want to see some old results?
I have plotted some results in a python notebook which can be seen in the **/results/analysis.ipynb** <br>
Run ``` cd results/``` and open the **analysis.ipynb**

