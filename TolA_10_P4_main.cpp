//****************************************************************************
//       CLASS:  CSCE-2110.001
//  ASSIGNMENT:  Program 4
//     STUDENT:  Andrew Tolle
// DESCRIPTION:  This program creates a basic graph 
//				
//
//				 This file contains the main menu, the processInput function
//				 and the file function
//
//
// REQUIRED SOURCE FILES:   TolA_10_P4_main.cpp   
//							TolA_10_P4_class.h							
//
//
//****************************************************************************
#include "TolA_10_P4_class.h"

int processInput(Collector* myNodes, std::string input, char delimiter);	// prototype for input function
void file(Collector* myNodes, std::string filename);						// prototype for file function

int main(void)
{
	bool loop = true;		// keeps menu looping until user exists
	std::string cmd;		// command from user (entire command line)

	Collector myNodes;		// creates new Collector object

	while(loop)			// loops until user quits
	{
		std::cout << std::endl;
		std::cout << std::endl << "******************************************************************";		// list of commands
		std::cout << std::endl << "** NOTE: Command and command field can be entered on one line   **";
		std::cout << std::endl << "**       i.e. node add A (ENTER)                                **";
		std::cout << std::endl << "**       Commands can be all uppercase or all lower case        **";
		std::cout << std::endl << "******************************************************************";
		std::cout << std::endl;
		std::cout << std::endl << "COMMAND          COMMAND FIELD	   DESCRIPTION";
		std::cout << std::endl << "node add         <name>             Add a new node";
		std::cout << std::endl << "node delete      <name>             Delete a node";
		std::cout << std::endl << "node search      <name>             Search for a node";	
		std::cout << std::endl << "edge add         <node A> <node B>  Add a new edge from A to B";
		std::cout << std::endl << "edge delete      <node A> <node B>  Delete edge from A to B";
		std::cout << std::endl << "kruskal                             Runs Kruskal's algorithm and prints adjacency list";	
		std::cout << std::endl << "print matrix                        Print adjacency matrix";
		std::cout << std::endl << "print list                          Print adjacency list" << std::endl;
		std::cout << std::endl << "cmd> ";

		do{std::getline(std::cin,cmd);}		// loops until the user enters a string (this prevents sending empty strings to processInput())
		while(cmd == "");					

		loop = processInput(&myNodes,cmd, ' ');	// passes command line to processInput function, which returns true or false depending on
	}											// whether or not the user quits. A space is passed as the delimiter because this is coming from
												// the command line
	return 0;	// ends program normally
}

int processInput(Collector* myNodes, std::string input, char delimiter)
{
	Node *nodeA, *nodeB;			
	std::stringstream ss(input);		// string stream to be used to fill command vector with commands
	std::string token;					// each command pushed into vector
	std::vector<std::string> cmdvector;	// vector for commands
	std::string nodeA_name = "";		// string for node A name
	std::string nodeB_name = "";		// string for node B name
	float dist;							// edge distance
	std::string edge_name;				// edge name	

	while(std::getline(ss, token, delimiter))	// fills command vector with each command by the delimiter
	{
		cmdvector.push_back(token);
	}

	//********************
	//** COMMAND = NODE **
	//********************
																// each if-else statement checks the [0] element of the command vector 
	if(cmdvector[0] == "node")									// for a match. If found, it pops off that command and checks the next one
	{															// in the following if-else statements.  
		cmdvector.erase(cmdvector.begin());		
		if(cmdvector[0] == "add")
		{
			cmdvector.erase(cmdvector.begin());					// each time the command is popped off, the vector size is checked for 
			if(cmdvector.size() > 0)							// the appropriate number of remaining commands, given the function
				myNodes->addNode(cmdvector[0]);					// to be called. If a command is in correct or too few commands remain
			else                								// (i.e. edge add A instead of edge add A B), the function returns to main
			{													// to await the next input from the command line
				while(nodeA_name == "")	// if no parameter was entered
				{
					std::cout << std::endl << "ERROR: No node name entered." << std::endl << "Enter node name: ";
					std::cin >> nodeA_name;
					std::cin.ignore();
				}
				myNodes->addNode(nodeA_name);
			}
		}														
		else if(cmdvector[0] == "delete")						
		{
			cmdvector.erase(cmdvector.begin());		// pops off first command
			if(cmdvector.size() > 0)
				myNodes->deleteNode(cmdvector[0]);		// deletes node
			else
			{													
				while(nodeA_name == "")	// if no parameter was entered
				{
					std::cout << std::endl << "ERROR: No node name entered." << std::endl << "Enter node name: ";
					std::cin >> nodeA_name;
					std::cin.ignore();
				}
				myNodes->deleteNode(nodeA_name);
			}	
		}
		else if(cmdvector[0] == "search")
		{
			cmdvector.erase(cmdvector.begin());		// pops off first command

			if(cmdvector.size() > 0)
			{
				if(myNodes->findNode(cmdvector[0]) == NULL)		// if find node returns NULL then node doesn't exist	
					std::cout << std::endl << "NODE " << cmdvector[0] << ": NOT FOUND";
				else
					std::cout << std::endl << "NODE " << cmdvector[0] << ": FOUND";
			}
			else
			{ 
				while(nodeA_name == "")	// if no parameter was entered
				{
					std::cout << std::endl << "ERROR: No node name entered." << std::endl << "Enter node name: ";
					std::cin >> nodeA_name;
					std::cin.ignore();
				}
				if(myNodes->findNode(nodeA_name) == NULL)		// if find node returns NULL then node doesn't exist	
					std::cout << std::endl << "NODE " << nodeA_name << ": NOT FOUND";
				else
					std::cout << std::endl << "NODE " << nodeA_name << ": FOUND";
			}
		}
		else
		{
			std::cout << std::endl << "ERROR: Invalid command" << std::endl;
		}
	}

	//********************
	//** COMMAND = EDGE **
	//********************

	else if(cmdvector[0] == "edge")				// if command is edge
	{
		cmdvector.erase(cmdvector.begin());		// pops off first command
		if(cmdvector[0] == "add")				// if command is add
		{
			cmdvector.erase(cmdvector.begin());		// pops off second command

			if(cmdvector.size() >= 4)
			{
				nodeA_name = cmdvector[0];			// stores node name
				nodeB_name = cmdvector[1];			// stores node name
				edge_name = cmdvector[2];			// stores edge name
				dist = atof(cmdvector[3].c_str());	// stores edge dist

				nodeA = myNodes->findNode(nodeA_name);	// searches for each node ([0] and [1]) and if they are both not NULL
				nodeB = myNodes->findNode(nodeB_name);	// it calls the addEdge function
				
				if(nodeA != NULL && nodeB != NULL)
					myNodes->addEdge(nodeA,nodeB,edge_name,dist);				// if both nodes exist it sends to add edge function
				else if(nodeA == NULL)							// if A doesn't exist
					std::cout << std::endl << "*** ERROR *** NODE " << nodeA_name << ": NOT FOUND";
				else if(nodeB == NULL)							// if B doesn't exist
					std::cout << std::endl << "*** ERROR *** NODE " << nodeB_name << " : NOT FOUND";
			}
			else
			{
				std::cout << std::endl << "ERROR: Improper number of command elements";
			}
/*			else if(cmdvector.size() == 1)			
			{
				nodeA_name = cmdvector[0];	// finds node entered
				while(nodeB_name == "")		// if only first node parameter was entered
				{
					std::cout << std::endl << "ERROR: Second node not entered" << std::endl << "Enter node name: ";
					std::cin >> nodeB_name;
					std::cin.ignore();
				}
			}
			else if(cmdvector.size() == 0)	// if no nodes were entered
			{
				while(nodeA_name == "")	// gets first node name
				{
					std::cout << std::endl << "ERROR: First node not entered" << std::endl << "Enter node name: ";
					std::cin >> nodeA_name;
					std::cin.ignore();
				}
				while(nodeB_name == "")	// if only first node parameter was entered
				{
					std::cout << std::endl << "ERROR: Second node not entered" << std::endl << "Enter node name: ";
					std::cin >> nodeB_name;
					std::cin.ignore();
				}
			}*/
		}
		else if(cmdvector[0] == "delete")		// if command is delete
		{
			cmdvector.erase(cmdvector.begin());		// pops off first command

			if(cmdvector.size() >= 2)
			{
				nodeA_name = cmdvector[0];
				nodeB_name = cmdvector[1];
			}
			
			else if(cmdvector.size() == 1)
			{
				nodeA_name = cmdvector[0];
				while(nodeB_name == "")	// if only first node parameter was entered
				{
					std::cout << std::endl << "ERROR: Second node not entered" << std::endl << "Enter node name: ";
					std::cin >> nodeB_name;
					std::cin.ignore();
				}
			}
			else if(cmdvector.size() == 0)	// if no nodes were entered
			{
				while(nodeA_name == "")	// gets first node name
				{
					std::cout << std::endl << "ERROR: First node not entered" << std::endl << "Enter node name: ";
					std::cin >> nodeA_name;
					std::cin.ignore();
				}
				while(nodeB_name == "")	// if only first node parameter was entered
				{
					std::cout << std::endl << "ERROR: Second node not entered" << std::endl << "Enter node name: ";
					std::cin >> nodeB_name;
					std::cin.ignore();
				}
			}

			nodeA = myNodes->findNode(nodeA_name);	// searches for each edge ([0] and [1]) and if they are both not NULL
			nodeB = myNodes->findNode(nodeB_name);	// it calls the deleteEdge function
				
			if(nodeA != NULL && nodeB != NULL)
			{
				myNodes->deleteEdge(nodeA,nodeB);	
			}
			else 		// if either node doesn't exist
				std::cout << std::endl <<"*** ERROR *** EDGE DOES NOT EXIST FOR DELETION: " << nodeA_name << "-" << nodeB_name;

		}
		else
		{
			std::cout << std::endl << "ERROR: Invalid command" << std::endl;
		}
	}

	//*********************
	//** COMMAND = PRINT **
	//*********************	
	
	else if(cmdvector[0] == "print")				// if command is print
	{
		cmdvector.erase(cmdvector.begin());		// pops off first command

		if(cmdvector[0] == "matrix")				// if next command is matrix, calls printMatrix function
		{
			myNodes->printMatrix();
		}
		else if(cmdvector[0] == "list")				// if next command is list, calls printList function
		{
			myNodes->printList();			
		}
		else if(cmdvector[0] == "edges")				// if next command is list, calls printList function
		{
			myNodes->printEdges();
		}
		else
		{
			std::cout << std::endl << "ERROR: Invalid command" << std::endl;
		}
	}

	//************************
	//** COMMAND = KRUSKALS **
	//************************

	else if(cmdvector[0] == "kruskals" || cmdvector[0] == "kruskal" || cmdvector[0] == "krushkals")
	{
		myNodes->Kruskals();
	}

	//********************
	//** COMMAND = PATH **
	//********************

	else if(cmdvector[0] == "path" || cmdvector[0] == "distance")
	{
		cmdvector.erase(cmdvector.begin());		// pops off first command
		
		if(cmdvector.size() >= 2)
		{
			nodeA = myNodes->findNode(cmdvector[0]);	// searches for each edge ([0] and [1]) and if they are both not NULL
			nodeB = myNodes->findNode(cmdvector[1]);	// it calls the deleteEdge function
				
			if(nodeA != NULL && nodeB != NULL)
			{
				myNodes->ShortestPath(nodeA,nodeB);	
			}
			else if(nodeA == NULL)		// if either node doesn't exist
				std::cout << std::endl <<"*** ERROR *** NODE " << cmdvector[0] << " DOES NOT EXIST";
			else if(nodeB == NULL)
				std::cout << std::endl <<"*** ERROR *** NODE " << cmdvector[1] << " DOES NOT EXIST";
		}
	}


	//********************
	//** COMMAND = FILE **
	//********************

	else if(cmdvector[0] == "file")					
	{
		cmdvector.erase(cmdvector.begin());		// pops off first command
		
		if(cmdvector.size() > 0)				// passes filename to file function
			file(myNodes,cmdvector[0]);
	}	

	//********************
	//** COMMAND = QUIT **
	//********************

	else if(cmdvector[0] == "quit")				// if quit, returns 0 to main which ends while loop for menu and ends program
		return 0;
	else
	{
		std::cout << std::endl << "ERROR: Invalid command" << std::endl;
	}

	return 1;
}

void file(Collector* myNodes, std::string filename)		// function for reading from .csv file
{
	std::ifstream file;			// opens in stream
	std::string cmd;			// reads each word

	file.open(filename.c_str());	// opens file

	if (!file)		// if file could not open
		std::cout << std::endl << "ERROR: File could not be opened";
	else
	{
		while (file.good())	// loops until EOF
		{
			getline(file,cmd,'\r');	// reads commands until \r return cursor

			if(cmd != "")			// passes commands to processInput so long as they are not \n
				processInput(myNodes,cmd,',');

			getline(file,cmd,'\n');		// eats \n
		}

		file.close();			// closes file
	}
}