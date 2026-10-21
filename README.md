What was the problem you were solving in the projects for this course?

The purpose of the projects for the course was to implement an advising assistance program for ABCU. The program needed to read a file with information about the courses and display all of the courses in alphanumeric order. In addition, there would be functions to search for a course by name and display its prerequisites. Project One would be to compare various data structures and develop a solution to the problem posed by the project using pseudocode. Project Two would be to implement the best solution to the problem using C++.

How did you approach the problem?

For these two projects I compared the use of vectors, hash tables and binary search trees. A binary search tree was in the end the best solution, because an unbalanced tree has search time O(n) and the inorder-traversal of a tree (traversing the tree in order) automatically sorts all the courses in the tree in alphabetical order for storage in a file. The only other data structure that also automatically sorted the information was the vector, but a vector does not allow for as fast of a search as a binary search tree.

How did you overcome roadblocks?

When initially attempting to open the file for the course information (in a virtual desktop), I had to modify the file location to point to the location of the file within the environment where Eclipse was running. Additionally, the course information CSV file had been created with empty fields at the end of the rows for the prerequisite information (i.e., many trailing fields for the empty columns). I then modified the parsing to ignore these fields while still validating that the proper course information and prerequisite information were being added to the program (i.e., that a course had been properly added to the tree). Testing the program with a course that had multiple prerequisites also verified that the proper information was being displayed (i.e., not only the prerequisite number, but also the full course name).

How has this work expanded your approach to designing software?

These two programs for the Course Sorter helped me understand that it is very important to make a plan of how a program is going to work before you actually start to write the code. The pseudocode I made to plan the program before actually writing it in C++ in two separate parts (One and Two) was extremely helpful in breaking down the requirements into smaller tasks that I could complete one at a time. Also, I have become much more aware of a program’s worst-case scenario, rather than just assuming that a data structure will perform adequately under most circumstances.

How has this work changed the way you write maintainable, readable, and adaptable programs?

Also, the way I set up the functionality in the different functions to parse, validate, insert, search for and print out the course information, makes it a lot easier to read and therefore to understand. The information on how to add more prerequisites to a course is also stored in the way the information is stored in the different collections for the different parts of the Course class. This makes it easy to implement changes and test them.
