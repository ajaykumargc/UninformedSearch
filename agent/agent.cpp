// Aiagent.cpp : 

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <list>
#include <math.h>
#include <malloc.h>
#include <algorithm>

using namespace std;

int** adjMatrix = NULL;
int** adjCostMatrix = NULL;
string* nodeArray = NULL;
string* explored = NULL;
string* marked = NULL;
static int parent = 1;
static int nodeNum = 1;

typedef struct node
{
	string nodeName;
	int pathcost;
	int parent;
	int nodeNum;
	int nodelevel;
};

struct Comparator
{
	bool operator()(const node& left, const node& right)
	{
		return left.pathcost>right.pathcost;
	}
};

struct ComparatorAlpha
{
	bool operator()(const node& left, const node& right)
	{
		return left.nodeName>right.nodeName;
	}
};

struct ComparatorAlphaclone
{
	bool operator()(const node& left, const node& right)
	{
		return left.nodeName<right.nodeName;
	}
};

bool nodenameSort(const node& left, const node& right)
{
	return left.nodeName<right.nodeName;
}

priority_queue<node,vector<node>,ComparatorAlpha> alphaSort;
priority_queue<node, vector<node>, ComparatorAlphaclone> alphaSortclone;
priority_queue<node, vector<node>, Comparator> frontier;
//list<node> alphaSortQ;
queue<node> frontierQ;
list<node> frontierStack;

int searchIndex(string nodename, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (nodeArray[i] == nodename)
		{
			return i;
		}
	}
}

int bfsAlgo(string src, string dest, int nodeSize)
{
	list<node> logPath;
	node source;
	source.nodeName = src;
	source.pathcost = 0;
	source.parent = 1;
	source.nodeNum = 1;
	static int oldlevel = 0;
	static int curlevel;

	explored = new string[nodeSize];
	marked = new string[nodeSize];
	for (int k = 0; k < nodeSize; k++)
	{
		explored[k] = "null";
		marked[k] = "null";
	}
	
	if (source.nodeName == dest)
	{
		//log the path
		ofstream myfile;
		myfile.open("output.txt");
		myfile << source.nodeName << endl;
		myfile << source.nodeName << endl;
		myfile << "0";
		myfile.close();
		return 0;
	}

	//frontier.push(source);
	frontierQ.push(source);

	while (true)
	{
		if (frontierQ.empty())
		{
			ofstream myfile;
			myfile.open("output.txt");
			int logLen = logPath.size();
			list<node>::iterator it;
			int i = 0;
			for (it = logPath.begin(); it != logPath.end(); ++it)
			{
				if (++i != logLen)
				{
					myfile << (*it).nodeName << "-";
				}
				else
				{
					myfile << (*it).nodeName;
				}
			}
			myfile << endl;
			myfile << "NoPathAvailable";
			myfile.close();
			return 1;
		}
		node shallowNode = frontierQ.front();
		
		int index = searchIndex(shallowNode.nodeName, nodeSize);
		explored[index] = shallowNode.nodeName;
		marked[index] = shallowNode.nodeName;
		frontierQ.pop();

		//condition for goal test 
		if (shallowNode.nodeName == dest)
		{
			logPath.push_back(shallowNode);
			//return solution found

			ofstream myfile;
			myfile.open("output.txt");
			
			int logLen = logPath.size();
			list<node>::iterator it;
			int i = 0;
			for (it = logPath.begin(); it != logPath.end(); ++it)
			{
				if (++i != logLen)
				{
					myfile << (*it).nodeName << "-";
				}
				else
				{
					myfile << (*it).nodeName;
				}
			}
			myfile << endl;

			logPath.reverse();
			list<node> path;
			list<node>::iterator itr;
			static int actualcost = 0;
			static int oldparent = 0;
			for (it = logPath.begin(); it != logPath.end(); ++it)
			{
				if (oldparent == 0)
				{
					oldparent = (*it).parent;
					path.push_front((*it));
					continue;
				}
				int nodenum = (*it).nodeNum;

				if ((*it).nodeNum == oldparent)
				{
					oldparent = (*it).parent;
					path.push_front((*it));
				}
				else
				{
					continue;
				}
			}

			int firtIndex = -1;
			for (it = path.begin(); it != path.end(); ++it)
			{
				if (firtIndex == -1)
				{
					firtIndex = searchIndex((*it).nodeName, nodeSize);
					continue;
				}
				else
				{
					int currentIndex = searchIndex((*it).nodeName, nodeSize);
					actualcost = actualcost + adjCostMatrix[firtIndex][currentIndex];
					firtIndex = currentIndex;
				}
			}
			
			int pathLen = path.size();
			int j = 0;
			for (itr = path.begin(); itr != path.end(); ++itr)
			{
				if (++j != pathLen)
				{
					myfile << (*itr).nodeName <<"-";
				}
				else
				{
					myfile << (*itr).nodeName;
				}
			}
			myfile << endl;
			myfile << actualcost;
			myfile.close();
			return 0;
		}

		logPath.push_back(shallowNode);
		set<node> childNodes;
		set<node>::iterator iter;

		for (int j = 0, k=index; j < nodeSize; j++)
		{
			if (adjMatrix[index][j] > 0)
			{
				//compare if visited or not 
				if (marked[j] == "null")
				{
					node temp;
					temp.nodeName = nodeArray[j];
					temp.pathcost = shallowNode.pathcost + 1;
					temp.nodeNum = ++nodeNum;
					temp.parent = shallowNode.nodeNum;

					marked[j] = temp.nodeName;;
					curlevel = temp.pathcost;
					if (oldlevel != curlevel)
					{
						alphaSort.push(temp);
						//alphaSortQ.push_back(temp);
						//alphaSortQ.sort(nodenameSort);
					}
				}
			}
		}

		if (frontierQ.empty())
		{
			while (!alphaSort.empty())
			{
				node tofrontier = alphaSort.top();
				frontierQ.push(tofrontier);
				alphaSort.pop();
				//alphaSortQ.pop_front();
			}
			oldlevel = curlevel;
		}
	}
}

node findFirst(list<node> stcak)
{
	int ind = 0;
	list<node>::iterator lp;
	for (lp = stcak.begin(); lp != stcak.end(); ++lp)
	{
		if (ind == 0)
		{
			node retVal;
			retVal.nodeName = (*lp).nodeName;
			retVal.nodeNum = (*lp).nodeNum;
			retVal.parent = (*lp).parent;
			retVal.pathcost = (*lp).pathcost;
			return retVal;
		}
	}
}

node findFirstUcs(deque<node> stcak)
{
	int ind = 0;
	deque<node>::iterator lp;
	for (lp = stcak.begin(); lp != stcak.end(); ++lp)
	{
		if (ind == 0)
		{
			node retVal;
			retVal.nodeName = (*lp).nodeName;
			retVal.nodeNum = (*lp).nodeNum;
			retVal.parent = (*lp).parent;
			retVal.pathcost = (*lp).pathcost;
			return retVal;
		}
	}
}

int dfsAlgo(string src, string dest, int nodeSize)
{
	list<node> logPath;
	node source;
	source.nodeName = src;
	source.pathcost = 0;
	source.parent = 1;
	source.nodeNum = 1;
	static int oldlevel = 0;
	static int curlevel;
	explored = new string[nodeSize];
	marked = new string[nodeSize];
	for (int k = 0; k < nodeSize; k++)
	{
		explored[k] = "null";
		marked[k] = "null";
	}

	if (source.nodeName == dest)
	{
		//log the path
		ofstream myfile;
		myfile.open("output.txt");
		myfile << source.nodeName << endl;
		myfile << source.nodeName << endl;
		myfile << "0";
		myfile.close();
		return 0;
	}

	frontierStack.push_front(source);
	
	while (!frontierStack.empty())
	{
		node shallowNode = findFirst(frontierStack); //write function to return first node 
		logPath.push_back(shallowNode);
		frontierStack.pop_front();

		set<node> childNodes;
		set<node>::iterator iter;

		int index = searchIndex(shallowNode.nodeName, nodeSize);
		explored[index] = shallowNode.nodeName;
		marked[index] = shallowNode.nodeName;

		for (int j = 0, k = index; j < nodeSize; j++)
		{
			if (adjMatrix[index][j] > 0)
			{
				//compare if visited or not 
				if (marked[j] == "null")
				{
					node temp;
					temp.nodeName = nodeArray[j];
					temp.pathcost = shallowNode.pathcost + 1;
					temp.nodeNum = ++nodeNum;
					temp.parent = shallowNode.nodeNum;

					if (temp.nodeName == dest)
					{
						//successs
						explored[j] = temp.nodeName;
						alphaSortclone.push(temp);
						while (!alphaSortclone.empty())
						{
							node toStack = alphaSortclone.top();
							frontierStack.push_front(toStack);
							alphaSortclone.pop();
						}
						while (!frontierStack.empty())
						{
							node toLog = findFirst(frontierStack);
							if (toLog.nodeName == temp.nodeName)
							{
								logPath.push_back(temp);
								frontierStack.pop_front();
								break;
							}
							else
							{
								logPath.push_back(toLog);
								frontierStack.pop_front();
							}
						}

						ofstream myfile;
						myfile.open("output.txt");

						int logLen = logPath.size();
						int i = 0;
						list<node>::iterator it;
						for (it = logPath.begin(); it != logPath.end(); ++it)
						{
							if (++i != logLen)
							{
								myfile << (*it).nodeName << "-";
							}
							else
							{
								myfile << (*it).nodeName;
							}
						}
						myfile << endl;

						logPath.reverse();
						list<node> path;
						list<node>::iterator itr;
						static int actualcost = 0;
						static int oldparent = 0;
						for (it = logPath.begin(); it != logPath.end(); ++it)
						{
							if (oldparent == 0)
							{
								oldparent = (*it).parent;
								path.push_front((*it));
								continue;
							}
							int nodenum = (*it).nodeNum;

							if ((*it).nodeNum == oldparent)
							{
								oldparent = (*it).parent;
								path.push_front((*it));
							}
							else
							{
								continue;
							}
						}

						int firtIndex = -1;
						for (it = path.begin(); it != path.end(); ++it)
						{
							if (firtIndex == -1)
							{
								firtIndex = searchIndex((*it).nodeName, nodeSize);
								continue;
							}
							else
							{
								int currentIndex = searchIndex((*it).nodeName, nodeSize);
								actualcost = actualcost + adjCostMatrix[firtIndex][currentIndex];
								firtIndex = currentIndex;
							}
						}

						int pathLen = path.size();
						int j = 0;
						for (itr = path.begin(); itr != path.end(); ++itr)
						{
							if (++j != pathLen)
							{
								myfile << (*itr).nodeName << "-";
							}
							else
							{
								myfile << (*itr).nodeName;
							}
						}
						myfile << endl;
						myfile << actualcost;
						myfile.close();
						return 0;
					}

					marked[j] = temp.nodeName;
					alphaSortclone.push(temp);
				}
			}
		}

		while (!alphaSortclone.empty())
		{
			node tofrontier = alphaSortclone.top();
			frontierStack.push_front(tofrontier);
			alphaSortclone.pop();
		}
	}
	// return non zero value catch and log it.
	ofstream myfile;
	myfile.open("output.txt");
	int logLen = logPath.size();
	int i = 0;
	list<node>::iterator it;
	for (it = logPath.begin(); it != logPath.end(); ++it)
	{
		if (++i != logLen)
		{
			myfile << (*it).nodeName << "-";
		}
		else
		{
			myfile << (*it).nodeName;
		}
	}
	myfile << endl;
	myfile << "NoPathAvailable";
	myfile.close();
	return 1;
}

bool findElem(list<node> toSearchList, string nodename)
{
	list<node>::iterator itr;
	bool retVal = false;
	if (!toSearchList.empty())
	{
		for (itr = toSearchList.begin(); itr != toSearchList.end(); ++itr)
		{
			if ((*itr).nodeName == nodename)
			{
				retVal = true;
				return retVal;
			}
		}
	}
	return retVal;
}

bool findElemUcs(deque<node> toSearchList, string nodename)
{
	deque<node>::iterator itr;
	bool retVal = false;
	if (!toSearchList.empty())
	{
		for (itr = toSearchList.begin(); itr != toSearchList.end(); ++itr)
		{
			if ((*itr).nodeName == nodename)
			{
				retVal = true;
				return retVal;
			}
		}
	}
	return retVal;
}

bool ComparePathcost(const node &lhs,const node &rhs)
{
	if (lhs.pathcost == rhs.pathcost)
	{
		return lhs.nodeName < rhs.nodeName;
	}
	else
	{
		return lhs.pathcost < rhs.pathcost;
	}
}

int ucsAlgo(string src, string dest, int nodeSize)
{
	list<node> logPath;
	deque<node> frontierList;
	list<node> exploredList;
	deque<node> alphaList;
	deque<node>::iterator fitr;
	deque<node>::iterator aitr;
 	node source;
	source.nodeName = src;
	source.pathcost = 0;
	source.parent = 1;
	source.nodeNum = 1;
	source.nodelevel = 0;
	static int oldlevel = 0;
	static int curlevel;
	marked = new string[nodeSize];
	for (int k = 0; k < nodeSize; k++)
	{
		marked[k] = "null";
	}

	//frontier.push(source);
	frontierList.push_back(source);
	sort(frontierList.begin(), frontierList.end(), ComparePathcost);

	while (true)
	{
		if (frontierList.empty())
		{
			//failure
			ofstream myfile;
			myfile.open("output.txt");
			int logLen = exploredList.size();
			int i = 0;
			static int actualCost = 0;
			list<node>::iterator it;
			for (it = exploredList.begin(); it != exploredList.end(); ++it)
			{
				if (++i != logLen)
				{
					myfile << (*it).nodeName << "-";
				}
				else
				{
					myfile << (*it).nodeName;
				}
				actualCost = (*it).pathcost;
			}
			myfile << endl;
			myfile << "NoPathAvailable";
			myfile.close();
			return 1; 
		}

		node shallowNode = findFirstUcs(frontierList);
		frontierList.pop_front();

		if (shallowNode.nodeName == dest)
		{
			exploredList.push_back(shallowNode);
			//success log the path
			ofstream myfile;
			myfile.open("output.txt");

			int logLen = exploredList.size();
			int i = 0;
			static int actualCost = 0;
			list<node>::iterator it;
			for (it = exploredList.begin(); it != exploredList.end(); ++it)
			{
				if (++i != logLen)
				{
					myfile << (*it).nodeName << "-";
				}
				else
				{
					myfile << (*it).nodeName;
				}
				actualCost = (*it).pathcost;
			}
			myfile << endl;
			//iterate the path and get actual path
			exploredList.reverse();
			list<node> path;
			list<node>::iterator itr;
			static int oldparent = 0;
			for (it = exploredList.begin(); it != exploredList.end(); ++it)
			{
				if (oldparent == 0)
				{
					oldparent = (*it).parent;
					path.push_front((*it));
					continue;
				}
				int nodenum = (*it).nodeNum;

				if ((*it).nodeNum == oldparent)
				{
					oldparent = (*it).parent;
					path.push_front((*it));
				}
				else
				{
					continue;
				}
			}

			int pathLen = path.size();
			int j = 0;
			for (itr = path.begin(); itr != path.end(); ++itr)
			{
				if (++j != pathLen)
				{
					myfile << (*itr).nodeName << "-";
				}
				else
				{
					myfile << (*itr).nodeName;
				}
			}
			myfile << endl;
			myfile << actualCost;
			myfile.close();
			return 0;
		}

		exploredList.push_back(shallowNode);
		logPath.push_back(shallowNode);

		set<node> childNodes;
		set<node>::iterator iter;

		int index = searchIndex(shallowNode.nodeName, nodeSize);
		marked[index] = shallowNode.nodeName;

		for (int j = 0, k = index; j < nodeSize; j++)
		{
			if (adjMatrix[index][j] > 0)
			{
				node temp;
				temp.nodeName = nodeArray[j];
				temp.pathcost = shallowNode.pathcost + adjCostMatrix[index][j];
				temp.nodeNum = ++nodeNum;
				temp.parent = shallowNode.nodeNum;

				bool inFrontier = findElemUcs(frontierList, temp.nodeName);
				bool inExplored = findElem(exploredList, temp.nodeName);

				//compare if visited or not 
				if ( !inFrontier && !inExplored)
				{		
					frontierList.push_back(temp);
					sort(frontierList.begin(), frontierList.end(), ComparePathcost);
				}
				else
				{
					if (inFrontier)
					{
						deque<node>::iterator itr;
						bool present = false;
						if (!frontierList.empty())
						{
							itr = frontierList.begin();
							while (itr < frontierList.end())
							{
								if (((*itr).nodeName == temp.nodeName) && ((*itr).pathcost > temp.pathcost))
								{
									present = true;
									(*itr).nodeName = temp.nodeName;
									(*itr).nodeNum = temp.nodeNum;
									(*itr).parent = temp.parent;
									(*itr).pathcost = temp.pathcost;
									break;
								}
								++itr;
							}
						}
						sort(frontierList.begin(), frontierList.end(),ComparePathcost);
					}
					else if (inExplored)
					{
						list<node>::iterator itr;
						if (!exploredList.empty())
						{
							for (itr = exploredList.begin(); itr != exploredList.end(); ++itr)
							{
								if (((*itr).nodeName == temp.nodeName) && ((*itr).pathcost > temp.pathcost))
								{
									exploredList.insert(itr, temp);
									exploredList.erase(itr);
								}
							}
						}
					}
				}
			}
		}
	}
}

int main()
{
	// read the input from the file and parse it get the meaning full information
	ifstream file_reader("input.txt");

	if (!file_reader.is_open())
	{
		// file opening error handle the case
		return 1;
	}
	string eachLine;
	int task = 0;
	getline(file_reader, eachLine);

	stringstream strTask(eachLine);
	if (!(strTask >> task))
	{
		//error handling
		return 1;
	}

	getline(file_reader, eachLine);
	string sourceNode = eachLine;

	getline(file_reader, eachLine);
	string destNode = eachLine;

	// parse for the total number of nodes
	int totalNode = 0;
	getline(file_reader, eachLine);
	stringstream nodeStr(eachLine);

	if (!(nodeStr >> totalNode))
	{
		//error handling
		return 1;
	}

	nodeArray = new string[totalNode];

	adjMatrix = new int*[totalNode];

	for (int i = 0; i < totalNode; i++)
	{
		getline(file_reader, eachLine);
		nodeArray[i] = eachLine;
	}

	adjCostMatrix = new int*[totalNode];
	//parse the matrix and create a matrix temporary.
	for (int i = 0; i < totalNode; i++)
	{
		getline(file_reader, eachLine);
		istringstream nodeValue(eachLine);
		adjCostMatrix[i] = new int[totalNode];
		adjMatrix[i] = new int[totalNode];

		for (int j = 0; j < totalNode; j++)
		{
			if (nodeValue >> adjCostMatrix[i][j])
			{
				//populate the graph adjMatrix
				if (adjCostMatrix[i][j] > 0)
				{
					adjMatrix[i][j] = 1;
				}
				else
				{
					adjMatrix[i][j] = 0;
				}
			}
		}
	}

	switch (task)
	{
	case 1:
		bfsAlgo(sourceNode, destNode, totalNode);
		break;
	case 2:
		dfsAlgo(sourceNode, destNode, totalNode);
		break;
	case 3:
		ucsAlgo(sourceNode, destNode, totalNode);
		break;
	default: // do log the message
		break;
	}
	return 0;
}
