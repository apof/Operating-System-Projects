# Operating-System-Projects
Projects of Operating Systems DI course

--> Proj1 - Data WareHouse using SkipList and Chained Hashing data structures

The overall data structure is designed to store uviveristy students records and serve efficiently insert search and update queries using probabilistic data structure Skip List (https://en.wikipedia.org/wiki/Skip_list)

-->Proj2 - Process Hierarchies for Parallel Spatial Queries

  Implementing and organizing multiprocessing tree hierarchy that consists of a Master and arbitrary shape handlers each of whom creates an arbitrary number of workers. 
  Each shape handler represents a different utility(circle,semicircle,ring,square,ellipse) which concerns a different 2-d shape query. Given a binary file of 2-d points and a some shapes of given characteristics (center,radius..) we have to decide which of the given points "live" in every given shape. The whole process structures has to be synchronized. Named pipes have been used for process communication. Also low level I/O system calls, fork(), exec(), wait() and waitpid() have been utilized. 
  
-->Proj 3 - Bar Multiprocessing Simulation

Multiple different processes have to been synchronized using POSIX Semaphores and Shared memory in order to simulate the functionality of a restaurant avoiding race-condition,busy-waiting and deadlocks. There are differnet roles and utilities such as doorman,customers waiters etc.

-->Proj 4 - Implementing a FileSystem utility like tar based on inode-Table data structure.

Given a hierarchy of files/directories we have to save or restore it from disk preventing its "shape",content and Metadata. Insert,Extract,Zip,Print Metadata and other operations have been implemented for our tar file manipulation. The whole inode-Data structure is firstly been created in main memory. Then we write it on disk serializing it and every time we want to make a query or another operation we are recovering it back to Main memory format.
Authors of Proj4: Apostolos Florakis(https://github.com/apof) - George Meimarakis Katsogiannis (https://github.com/Geokats)

Extended description (in Greek) and schematic representation for each task can be found in each Project's directory.
