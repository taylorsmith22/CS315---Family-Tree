#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
vector<int> usedFamilyNames; //Creates vector of used family numbers
vector<int> usedChildNames; //Creates vector of used child numbers
vector<int> usedHusbandAndWifeNames; //Creates vectors of used parent numbers

struct person;

//Struct of a family
struct family {
	int familyNumber; //Number that determines which family it is
	person *husband; //Pointer a person that is the husband 
	person *wife; //Pointer to a person that is the wife
	person* children[10]; //Pointer to an array of persons that are the children
	static family* makeNewFamily(int number) { //Creates a new family
		family* familyStruct = new family; 
		familyStruct->familyNumber = number; //Sets the family number to number entered;
		familyStruct->husband = NULL; //Sets the husband to NULL
		familyStruct->wife = NULL; //Sets the wife to NULL
		for (int i = 0; i < 10; i++) { //Sets each child to NULL
			familyStruct->children[i] = NULL;
		}
		return familyStruct;
	}
};

//Struct of a person
struct person {
	int personNumber; //Number that determines which person it is
	family *marriage; //Pointer to a family that is the marriage
	family *parentage; //Pointer to a family that is the parentage
	static person* makeNewPerson(int number) { //Creates a new person
		person* personStruct = new person; 
		personStruct->personNumber = number; //Sets the person number to the number that was entered;
		personStruct->marriage = NULL; //Sets the marriage to NULL
		personStruct->parentage = NULL; //Sets the parentage to NULL
		return personStruct;
	}
};

//Struct for using UnionFind to determine if two people are related
struct unionStruct {
	int personNum; //Holds the persons number
    	int representative; //Holds the persons representative
    	int size; //Holds the size of the list of people
};

vector<person*> people; //Creates a vector of persons
vector<family*> families; //Creates a vector of families
unionStruct myUnion[100]; //Creates a an array of unionStructs

//Function that uses stringstream to parse the command that the user inputs
//Taken from another project in CS270
vector<string> parseCommand(string command) {
        istringstream iss(command);
        vector<string> parsedCommand;

        string s;
        while (iss >> quoted(s)) {
                parsedCommand.push_back(s);
        }
        return parsedCommand;
}

//Function that checks to see if a given person number has been used 
bool checkNumber(int number, vector<int> aVector) {
	for (double i = 0; i < aVector.size(); i++) {
		if (number == aVector[i])
			return true;
	}

	return false;
}

//Function that creates a person for the unionStruct
void CreatePerson(int personNum) {
	myUnion[personNum].personNum = personNum; //Sets the personNum the persons number
	myUnion[personNum].representative = personNum; //Sets the persons representative to itself
	myUnion[personNum].size = 1; //Sets the persons size to 1
}

//Function that unions two people together
void Union(int person1, int person2)
{
	int x = myUnion[person1].representative;
	int y = myUnion[person2].representative;
	if (x == y) { //Checks to see if the two people are already unioned together
		return;
	}
	//If perosn1's size is smaller than person2's size
	else if(myUnion[person1].size < myUnion[person2].size) {
		myUnion[person2].size = myUnion[person2].size + myUnion[person1].size; //Set person2'a size to its size plus person1's size
		for (int i = 1; i < person1; i++) { //Uses a for loop to set all the people whom person1 was their representative to where their representative is now person2 
			if (myUnion[i].representative == myUnion[person1].representative) //Checks to see if person1 was their representative
				myUnion[i].representative = myUnion[person2].representative;
		}
		for (int i = person1 + 1; i < 99; i++) { //Uses a for loop to set all the people whom person1 was their representative to where their represenative is now person2
			if (myUnion[i].representative == myUnion[person1].representative) //Checks to see if person1 was their representative
				myUnion[i].representative = myUnion[person2].representative;
		}
		myUnion[person1].representative = myUnion[person2].representative; //Sets person1's representative to person2's representative
		return;
	}
	//If person1's size is greater than or equal to person2's size
	else { 
		myUnion[person1].size = myUnion[person1].size + myUnion[person2].size; //Set person1's size to its size plus person2's size
		for (int i = 1; i < person2; i++) { //Uses a for loop to set all the pople whom person2 was their representative to where their representative is now person1
			if (myUnion[i].representative == myUnion[person2].representative) //Checks to see if person2 was their representative
				myUnion[i].representative = myUnion[person1].representative; 
		}
		for (int i = person2 + 1; i < 99; i++) { //Uses a for loop to set all the people whom person2 was their representative to where their representative is now person1
			if (myUnion[i].representative == myUnion[person2].representative) //Checks to see if person2 was their representative
				myUnion[i].representative = myUnion[person1].representative;
		}
		myUnion[person2].representative = myUnion[person1].representative; //Sets person2's representative to person1's representative
		return;
	}
 
}


vector<person*> visited; //Creates a vector to hold the people that were visited
vector<person*> neighbors; //Creates a vector to hold the neighbors
person* myQueue[100]; //Create an array to be the queue
int front = -1; //initially set the front to -1
int rear = -1; //initially set the rear to -1

//Function that checks to see if the queue is empty
bool isEmpty() { //if the front and rear are both -1 then the queue is empty
	if (front == -1 && rear == -1)
		return true;
	else 
		return false;
}

//Function that adds a person to the queue
void addToQueue(person* aPerson) {
	if (isEmpty() == true) { //if the queue is empty
		myQueue[0] = aPerson; //Set the first element to the person
		front = 0; //Set the front to 0
		rear = 0; //Set the rear to 0
	}
	else if (rear != 99) { //if the queue is not full
		myQueue[rear + 1] = aPerson; //Set the next element to the person
		rear = rear + 1; //Add one to the rear
	}
	else {
		cout << "Queue is full" << endl;
	}
}

//Function that removes a person from the queue and returns the person
person* removeFromQueue() {
	if (isEmpty() == true) { //If the queue is empty
		cout << "Queue is empty" << endl;
		return NULL;
	}
	else if (front == rear) { //If there is only one person in the queue
		front = -1; //Set the front to -1
		rear = -1; //Set the rear to -1
		return myQueue[front + 1]; //Return the person
	}
	else {
		front = front + 1; //Move the front up one
		return myQueue[front - 1]; //Return the person
	}
}

//Function that implements Breadth First Search to get a path
vector<person*> breadthFirstSearch(person* startingPerson, person* endPerson) {

	visited.push_back(startingPerson); //Add the person to the list of visited people
	addToQueue(startingPerson); //Add the person to the queue

	vector<person*> prev; //Creates a vector to hold the path
	
	//Iterates until the queue is not empty
	while (isEmpty() == false) {
		person* newPerson = removeFromQueue(); //Remove the person from the queue and set the person removed as the person you are using
		if (newPerson->parentage != NULL) { //if the person has a known parentage
			if (newPerson->parentage->husband != NULL) //if the person has a known father
				neighbors.push_back(newPerson->parentage->husband); //Add the husband to the list of neighbors
			if (newPerson->parentage->wife != NULL) //if the person has a known mother
				neighbors.push_back(newPerson->parentage->wife); //Add the wife to the list of neighbors
			for (int i = 0; i < 10; i++) { //Check too see the person has siblings
				if (newPerson->parentage->children[i] != NULL) //if the person has a sibling
					neighbors.push_back(newPerson->parentage->children[i]); //Add the sibling to the list of neighbors
			}
		}
		if (newPerson->marriage != NULL) { //if the person has a known marriage
			if (newPerson->marriage->husband != NULL) //If the person has a known husband 
				neighbors.push_back(newPerson->marriage->husband); //Add the husband to the list of neighbors
			if (newPerson->marriage->wife != NULL) //if the user has a known wife
				neighbors.push_back(newPerson->marriage->wife); //Add the wife to the list of neighbors
			for (int i = 0; i < 10; i++) { //Check to see if the person has children
				if (newPerson->marriage->children[i] != NULL) //if the person has a child
					neighbors.push_back(newPerson->marriage->children[i]); //Add the child to the list of neighbors
			}
		}

		for (long unsigned int i = 0; i < neighbors.size(); i++) { //Go through each neighbor that the person has
			bool check = false;
			for (long unsigned int j = 0; j < visited.size(); j++) { //Check to see if the neighbor has been visited
				if (neighbors[i] == visited[j]) //if is has then see the check equal to true
					check = true;
			}
			if (check == false) { //if the neighbor has not been visited 
				visited.push_back(neighbors[i]); //Add the neighbor to the list of visited people
				addToQueue(neighbors[i]); //Add the neighbor to the queue
				prev.push_back(neighbors[i]); //Add the neighbor to the path
			}
		}
	}
	return prev;
}


//Find function that determines if two people are related
void Find(int person1, int person2) {
        if (myUnion[person1].representative == myUnion[person2].representative) {
                cout << "Person " << person1 << " and Person " << person2 << " are related" << endl; //tells the user that the two people are related
                
		
		/*Unfinished implementation of getting the shortest path */
		/*person* firstPerson;
                person* secondPerson;
                for (long unsigned int i = 0; i < people.size(); i++) {
                        if (people[i]->personNumber == person1)
                                firstPerson = people[i];
                        if (people[i]->personNumber == person2)
                                secondPerson = people[i];
                }
                vector<person*> path = breadthFirstSearch(firstPerson, secondPerson);
		vector<person*> newPath;
		int size;
		int x = 0;
		for (long unsigned int j = 0; j < path.size(); j++) {
			if (path[j] == secondPerson){
				size = x;
			}
			x++;
		}
		
		for (person* aPerson = secondPerson; size > 0; aPerson = path[size]) {
			newPath.push_back(aPerson);

			size--;
		}
		for (long unsigned int i = 0; i < newPath.size(); i++) {
			cout << "Person " << newPath[i]->personNumber << " -> ";
		}
		cout << endl;*/
		/*End of unfinished shortest path implementation*/
		
        }
        else
                cout << "Person " << person1 << " and Person " << person2 << " are not related" << endl;
}


//Function uses commands to perform a task
void Command(vector<string> parsedCommand) {
        if (parsedCommand[0] == "Family") { //Checks to see if the command is Family
                int familyNum = stoi(parsedCommand[1]); //Converts the family number to integer
                if (checkNumber(familyNum, usedFamilyNames) == false) { //Checks to see if the family number has been used
                        if (parsedCommand[2] == "Husband" && parsedCommand[4] == "Wife") {
                                int husbandNum = stoi(parsedCommand[3]); //Converts husbands number to integer
                                int wifeNum = stoi(parsedCommand[5]); //Converts Wifes number to integer
                                if (checkNumber(husbandNum, usedHusbandAndWifeNames) == false && husbandNum <= 99) { //Checks to see if the husbands number has been used
                                        if (checkNumber(wifeNum, usedHusbandAndWifeNames) == false && wifeNum <= 99) { //Checks to see if the wifes number has been used
                                                vector<int> children;
						//Goes through each Child command
                                                for (double i = 6; i < parsedCommand.size(); i = i + 2) { 
                                                        if (parsedCommand[i] == "Child") {
                                                                int child = stoi(parsedCommand[i+1]); //Converts to integer
                                                                if (checkNumber(child, usedChildNames) == false && child <= 99) { //Checks to see if the childs number has been used
                                                                        children.push_back(child); //Adds the childs number to the used child numbers
                                                                }
								else { //Prints an error message if the child number has already been used
									cout << "Person number " << child << " cannot have multiple parentages" << endl;
									return;
								}

                                                        }
							else { //Prints an error message if a command besides Child is used
								cout << "Unknown family member. Try Child" << endl;
								return;
							}
                                                }
                                                family* family1 = family::makeNewFamily(familyNum); //Creates a new family
                                                families.push_back(family1); //Adds the family to the list of families
						
						if (husbandNum != 0) {						
							person* person1 = person::makeNewPerson(husbandNum); //Creates a new person (the husband)
                                                	people.push_back(person1); //Adds the husband to the list of people
							families[families.size() - 1]->husband = person1; //Assigns the families husband to the husband that was entered
							people[people.size() - 1]->marriage = families[families.size() - 1]; //Assigns the husbands marriage to the family
							usedHusbandAndWifeNames.push_back(husbandNum); //Adds the number used on the husband to the list of person numbers used
							if (myUnion[husbandNum].personNum != husbandNum) //Create the husband
								CreatePerson(husbandNum);

						}

						if (wifeNum != 0) {
							person* person2 = person::makeNewPerson(wifeNum); //Creates a new person (the wife)
                                                	people.push_back(person2); //Adds the wife to the list of people
                                                	families[families.size() - 1]->wife = person2; //Assigns the families wife to the wife that was entered
							people[people.size() - 1]->marriage = families[families.size() - 1]; //Assigns the husbands marriage to the family
							usedHusbandAndWifeNames.push_back(wifeNum); //Adds the number used on the wife to the list of person numbers used
							if (myUnion[wifeNum].personNum != wifeNum) //Create the wife
								CreatePerson(wifeNum); 
						}

						if (wifeNum != 0 && husbandNum != 0) //Union the husband and wife
							Union(husbandNum, wifeNum);

                                                for (long unsigned int j = 0; j < children.size(); j++) {
							person* person1 = person::makeNewPerson(children[j]); //Creates a new person (the child)
                                                        people.push_back(person1); //Add the child to the list of people
                                                        families[families.size() - 1]->children[j] = person1; //Assigns the families child to the child that was entered
							people[people.size() - 1]->parentage = families[families.size() - 1]; //Assigns the childs parentage to the family
                                                        usedChildNames.push_back(children[j]); //Adds the number used on the child to the list of person numbers used
							if (myUnion[children[j]].personNum != children[j]) //Create the child
								CreatePerson(children[j]); 
							if (husbandNum != 0) //union the husband and child
								Union(husbandNum, children[j]);
							if (wifeNum != 0) //Union the wife and child
								Union(wifeNum, children[j]);
                                                }

						//Unions all of the children together if there are no parents
						if (husbandNum == 0 && wifeNum == 0 && children.size() >= 2) {
							for (double k = 0; k < children.size() - 1; k++)
								Union(children[k], children[k+1]);
						}

						//people[people.size() - 1]->marriage = families[families.size() - 1]; //Assigns the husbands marriage to the family
						//people[people.size() - 2]->marriage = families[families.size() - 1]; //Assigns the wifes marriage to the family

                                                usedFamilyNames.push_back(familyNum); //Adds the number used on the family to the list of family numbers used
                                                //usedHusbandAndWifeNames.push_back(husbandNum); //Adds the number used on the husband to the list of person numbers used
                                                //usedHusbandAndWifeNames.push_back(wifeNum); //Adds the number used on the wife to the list of person numbers used


						//Prints out the family that was entered
                                                cout << "Family: " << families[families.size() - 1]->familyNumber;
                                                cout << " Husband: ";
					       if (families[families.size() - 1]->husband != NULL)
						       cout << families[families.size() - 1]->husband->personNumber;
					       else 
						       cout << "0 ";
                                                cout << " Wife: ";
					       	if (families[families.size() - 1]->wife != NULL) 
							cout << families[families.size() - 1]->wife->personNumber;
						else 
							cout << "0 ";
                                                for (long unsigned int j = 0; j < children.size(); j++) {
                                                        cout << " Child: " << families[families.size() - 1]->children[j]->personNumber;
                                                }
                                                cout << endl;
					}
                                        else { //Prints an error message if the wifes number has already been used
                                                cout << "Person number " << wifeNum << " cannot have multiple marriages" << endl;
						return;
                                        }
                                }
                                else { //Prints an error message if the Husbands number has already been used
                                        cout << "Person number " << husbandNum << " cannot have multiple marriages" << endl;
					return;
                                }
                        }
			else { //Prints an error message if there was no husband or wife entered
				cout << "Missing Husband or Wife" << endl;
				return;
			}

                }
                else { //Prints an error message if the family number has already been used
                        cout << "Family " << familyNum << " has already been assigned" << endl;
			return;
                }
        }
	else if (parsedCommand[0] == "Relate") { //If the command is Relate
        	int person1 = stoi(parsedCommand[1]); //Convert the first person to an integer
               	int person2 = stoi(parsedCommand[2]); //Convert the second person to an integer
                Find(person1, person2); //Determine if they are related
        }

	else if (parsedCommand[0] == "Parent") {
		int parent = stoi(parsedCommand[1]);
		cout << "Graph Parent of " << parent << " is " << myUnion[parent].representative << endl;
	}

        else { //Prints an error message if the first word is not Family
                cout << "Unknown command" << endl;
		return;
        }
}

int main () {
	string commandLine;
	while (getline(cin, commandLine)) {
	//for (int i = 0; i < 10; i++) {
		//string commandLine;
		//getline (cin, commandLine);
        	vector<string> command = parseCommand(commandLine);
		Command(command);
	}

	return 0;
}
