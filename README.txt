Uninformed Search

Andy is shy and has no idea how to connect to his dream girl, Alice. He
saw her at the school orientation, but he couldn’t find the opportunity to approach her. One day Andy
reads an article on the Internet: “Six degrees of separation is the assumption that everyone and
everything is six or fewer steps away, by way of introduction, from any other person in the world, so
that a chain of a-friend-of-a-friend statements can be made to connect any two people in a maximum of
six steps.” This assumption strongly encourages Andy. He thinks if he were introduced to the friend of his
friend, he eventually would meet his true love, Alice. The question is how to build the link from Andy to
Alice.

There are some constraints when you plan the link:
1. Andy is so shy that he can only be introduced to a person through a mutual friend. For example, if
you plan Andy to meet Eason, Andy must be first introduced to Emma via Calvin, and then let
Emma introduce Andy to Eason. This creates a link such as Andy-Calvin-Emma-Eason.
2. Andy wants to know Alice ASAP. If a mutual close friend introduces Andy to someone, he would
know this person faster. For example, to reach Claire, the time cost of Andy-Beck-Claire is 13
but the one of Andy-David-Claire is 55, so Andy prefers to know Claire by Beck.
Programming Task
You will need to write a program to implement the following search algorithms, to help Andy find
optimal traversal link(s) to reach Alice.
(1) Breadth-first search
(2) Depth-first search  
(3) Uniform-cost search using the distance of their relationship as cost
Note: When the costs of two or more nodes are equal, you need to make sure these nodes are popped
off the search queue in alphabetical order. This will resolve ambiguity and ensure that there is only one
correct solution for each problem.
Note 2: Your algorithm should perform loop detection.Do not enqueue a child
that has a state already visited, unless the child has a better cost than when we previously visited that
state 

Input:
You are provided with a file input.txt that describes the network as exemplified in Fig. 1.
<task#> bfs = 1, dfs = 2, ucs = 3
<source> the name of the first node, such as Andy in Fig. 1
<destination> the name of the target node, such as Alice in Fig. 1
<#nodes> the total number of nodes
<nodes> See the explanation below
<graph> See the explanation below
The few lines in <nodes> will look something like this:
Andy
Bill
Alice
This is a list of names representing the nodes of the graph.
The lines in <graph> of the file contains the matrix representation of the graph edges. It will look
something like this:
0 5 10
5 0 2
10 2 0
The rows and columns correspond to the nodes in the same order. For example, the first row tells us that
Andy is not connected to himself (0), connected to Bill with a value 5, and connected to Alice with a
value 10. In this assignment, we will assume that all relationships are mutual, so the matrix is symmetric. A
weight of 0 indicates that there is no edge between such two people, and any other value describes the
distance of their relationship.
Output:
The program should output in the format:
<Expansion>
<Output>
<PathCost>
For example,
Stacy-Emma-Helen-Frank-Jennifer-John-Jenny-Gerald-Claire-Patrick
Stacy-Emma-Frank-Gerald-Patrick
130
The nodes (separated by “-”) are in the order that show the link of the friendship. If your program cannot
find any solution path in the graph, please output NoPathAvailable. The output filename is fixed to
output.txt. 
Examples:
Please make sure your program follows the example here.
Nodes: Andy Bob Claire David Alice
Edges: 5
Source: Andy
Destination: Alice
Andy Bob Claire David Alice
Andy 0 1 1 0 0
Bob 1 0 5 4 0
Claire 1 5 0 0 1
David 0 4 0 0 0
Alice 0 0 1 0 0
BFS:
Log: Andy-Bob-Claire-David-Alice
Path: Andy-Claire-Alice
DFS:
Log: Andy-Bob-David-Claire-Alice
Path: Andy-Claire-Alice
Uniform-cost Search:
Log: Andy-Bob-Claire-Alice
Path: Andy-Claire-Alice