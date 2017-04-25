//  Lissa Callahan
//  CS135 Final Project, Spring 2017
// "Math Games" Math pratice for elementary school students (midterm), plus Jeopardy-style game for 2-3 players. 

#include <iostream>
#include <string> 
#include <fstream> // for file read and write
#include <cstdlib>//For RNG
#include <ctime> //RNG  need to seed system time for "truly" random numbers
#include <cmath> // for pow()
#include <algorithm> // for max()

using namespace std;

//The "main menu", user chooses to continue as "student" or "teacher".
void studentTeacher();
// If teacher, user is prompted for password (max 3 attempts) and sent to teacherSettings.
void teacherMenu(int attempts);
// Prompts student for name, to access file for scores and such.
void studentMenu();
// Option to change the percentage required for passing.
void teacherSettings();
// function to write to file (used for percentage and scores)
void writeFile(string filepath, string info);
// int scores stored in array, int length number of indexes
void writeFile(string filepath, int scores[], int length);
// function to read info from file 
string readFile(string filepath);
//need to read scores from student's file to diplay available levels
void readScores(string filepath, int(&scores)[10]);
// Menu for which to choose the operator (+, -, *, / or combo), uses student name (string).
void operatorMenu(string name);
//Choose from available levels (1 plus any unlocked levels), uses student name and mathtyye from operator menu.
void levelMenu(string name, char mathType, string mathPath);
// Runs math quiz of appropriate operator, level and saves score in name file.
void quiz(int level, char mathType, int &score);
//Generates random number, with proper number of digits for level and negative numbers for B levels.
int randNum(int magnitude, bool negatives);
//Same function name, diff parameters, uses same min and max from previous (so different function).
int randNum(int min, int max);
// List of "wrong answer" responses, to display in random order.
string crappyResponse();
// List of "correct answer" responses, to display in random order.
string happyResponse();
// Takes in players, names, and gives "buzzer letter".
void jeopardyMenu();
// Displays the board in preparation for play.
void displayBoard(int[5][5]);
// Initialize board if new game
void newGame(int round);
//
void playGame();



//main only used to initiate the first menu function and to seed RNG
int main()
{
	// Seed random number generator
	unsigned seed = time(0);
	srand(seed);

	studentTeacher();
	return 0;
}

// Student, teacher or quit: main menu
void studentTeacher()
{
	const int INITIAL_ATTEMPT = 1; //initialization
	int choice;
	bool quit = false;
	while (!quit)  //bool shortcut allows us to not need quit == false, b/c bool (quit varible is a bool, expecting a bool in () )
	{
		cout << "Main Menu" << endl
			<< "________________" << endl << endl
			<< "1. Instructor." << endl
			<< "2. Student Practice." << endl
			<< "3. Jeopardy Game!" << endl
			<< "4. Quit." << endl << endl;
		cin >> choice;
		system("CLS"); //no need for prototype because it's in std library (yay)!

		switch (choice)
		{
		case 1:
			teacherMenu(INITIAL_ATTEMPT); //correct password parameter to get to teacher settings menu
			break;
		case 2:
			studentMenu();
			break;
		case 3:
			jeopardyMenu();
			break;
		case 4: 
			quit = true; //assignment
			break;
		default: //anything else, gives message
			cout << "Please enter an option 1-4.\n";
		}
	}
}
// If teacher, user is prompted for password (max 3 attempts) and sent to teacherSettings.
void teacherMenu(int attempts)
{
	string password;
	cout << "Please enter your (case-sensitive) password, please: " << endl;
	cin >> password;
	system("CLS");
	if (password == "pass") //password correct; == tests equality
	{
		teacherSettings(); //if password is correct, to teacher settings menu
	}
	else
	{
		if (attempts >= 3)// Given a max of 3 tries
		{//return to studentTeacher menu if pass wrong 3 times
			cout << "Sorry, that password is wrong." << endl;
			return;
		}
		else
		{
			teacherMenu(attempts + 1); //each attempt adds 1 to counter
		}
		return;
	}
}
// Option to change the percentage required for passing.
void teacherSettings()
{
	int choice;
	int percentage = 0;
	cout << "Instructor Settings Menu" << endl
		<< "__________________________" << endl << endl;
	cout << "Please choose an option from the following:" << endl << endl;
	cout << "1. Set passing %" << endl; //currently only option
	cin >> choice;
	system("CLS");

	switch (choice)
	{
	case 1:
		break;
	}
	if (choice == 1) // If they wish to change the percentage required to pass...
	{
		string percentage = readFile("storage/passing_percentage.txt");//read in percentage from file where it's stored
		cout << "The current percentage is " << percentage << "%, to what would you like to change the passing percentage?" << endl;
		cin >> percentage; //new percentage
		system("CLS");
		cout << "The new percentage required to pass a level is " << percentage << "%.  Please press \"ENTER\" to continue." << endl; //display new percentage
		system("pause"); //to press enter
		writeFile("storage/passing_percentage.txt", percentage);  //output new percentage, i.e. write to passing_percentage file
		system("CLS");
	}
}

//******************
// FILE I/O
//******************
// function to write to file (used for percentage and scores)
void writeFile(string filepath, string info)
{
	ofstream outputFile; //declare 
	outputFile.open(filepath);//parameter is filepath variable
	outputFile << info; //output the new info to file
	outputFile.close(); //close file
	return;
}
// int scores stored in array, int length number of indexes
void writeFile(string filepath, int scores[], int length)
{
	ofstream outputFile;
	outputFile.open(filepath);
	for (int index = 0; index < length; index++) //initialize index, condition is < the number of indexes, and add one to index.
	{
		outputFile << scores[index] << endl; // write scores to file, in appropriate index location (each in own row, 1 column)
	}
	outputFile.close();
}
// function to read info from file
string readFile(string filepath)
{
	ifstream inputFile; //declare
	string info; //file info is stored as a string (always)
	inputFile.open(filepath); //parameter is variable filepath
	inputFile >> info; //reading info from the file
	inputFile.close();
	return info;
}
//need to read scores from student's file to diplay available levels
void readScores(string filepath, int(&scores)[10])
{
	ifstream inputFile;
	inputFile.open(filepath);
	//determine if existing
	if (inputFile)
	{
		for (int index = 0; index < 10; index++)
		{
			//read score from file
			inputFile >> scores[index];
		}
	}

	inputFile.close();
}
// Prompts student for name, to access file for scores and such.
void studentMenu()
{
	string name;
	cout << "Hello! Welcome to MathWiz! Please enter your name with no spaces and all lowercase [first_last]: " << endl;
	cin >> name;
	system("CLS"); //terminal command
	cout << "Great, let's get started!" << endl << endl;
	operatorMenu(name);
}
// Menu for which to choose the operator (+, -, *, / or combo), uses student name (string).
void operatorMenu(string name)
{
	int choice; //options 1-5 (easier than char or string, consistancy)
	cout << "Student Menu" << endl
		<< "--------------" << endl << endl
		<< "Please enter an option from the following: " << endl << endl
		<< "1. Addition (levels 1-5)" << endl
		<< "2. Subtraction (levels 1-5)" << endl
		<< "3. Multiplication (levels 1-5)" << endl
		<< "4. Division (levels 1-5)" << endl
		<< "5. Combination of all operators (levels 1-5)" << endl << endl;
	cin >> choice;
	switch (choice)
	{//each menu parameter gives a menu with the 5 (10, considering A & B) level options for chosen operator
	case 1:
		// need (+, -, *, /) symbol to use during quiz *and* string name for filepath to save scores.
		levelMenu(name, '+', "addition");
		break;
	case 2:
		levelMenu(name, '-', "subtraction");
		break;
	case 3:
		levelMenu(name, '*', "multiplication");
		break;
	case 4:
		levelMenu(name, '/', "division");
		break;
	case 5:
		levelMenu(name, 'C', "combination"); // combination-- random display of all 4 operators
		break;
	default:
		cout << "Please enter a valid selection, 1-5.\n\n"; // input needs to be 1 - 5
	}
}
//Choose from available levels (1 plus any unlocked levels), uses student name and mathtype from operator menu.
//Student name, mathType is operator option, mathPath is operator name for file.
void levelMenu(string name, char mathType, string mathPath)
{
	ifstream inFile;
	int level;
	int percentage;
	inFile.open("storage/passing_percentage.txt"); //open percentage file
	inFile >> percentage; //read in current passing percentage from file where it's stored
	inFile.close();
	const int TOTAL_LEVELS = 10;
	// initializing array with 10 items, so 0's are in each array space in the file initially.
	int scores[TOTAL_LEVELS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//parameter is filepath to operater folder, and then to student's name
	readScores("storage/" + mathPath + "/" + name + ".txt", scores);
	//User sees only levels available to them (1 and those passed up to).
	cout << "Your unlocked levels: " << endl << endl;
	int indexCounter = 0;

	do
	{
		cout << indexCounter + 1 << ".  Level " << indexCounter + 1 << "!  (your best score: " << scores[indexCounter] << "%)" << endl;
	} while ((scores[indexCounter++] >= percentage) && (indexCounter < 10));

	cin >> level; //chosen level
	if (level > 0 && level <= indexCounter) //if level choice is within 1 and passed into levels
	{//chosen operator with index location (example: level 1 is [0] index).
		quiz(level, mathType, scores[level - 1]);
		writeFile("storage/" + mathPath + "/" + name + ".txt", scores, TOTAL_LEVELS);
	}
	else
	{
		cout << "Invalid selection." << endl; // error message, level out of bounds
	}
}
//Runs math quiz of appropriate operator, level and saves score in name file.
//Char mathType here because we are pulling in the paramenter from operatorMenu, and
//passing scores in the array as a reference so the score stored in file is actually updated.
void quiz(int level, char mathType, int &score)
{
	//Pattern here--to have the same magnitude for levels 1&2 (1A & 1B), 3&4 (2A & 2B), etc....
	//I'm dividing the level number by 2 and adding that to level mod 2 to result in a magnitude 
	//of 1 for level 1&2, 2 for 3&4, etc..
	int magnitude = (level / 2) + (level % 2); // ^^
	bool negatives = (level % 2 == 0); // B levels are all even (2, 4, 6, 8, 10), so if even, allow negative numbers
	int answer; //answer entered by student
	int quotient; // need quotient and remainder so division can be calculated and entered "long" way instead of with a float (calculator way)
	int remainder; // above
	int questions; //10 questions total
	int correctCount = 0; //initiating correct question count at 0, when student's answer is correct
	ifstream inFile; //to pull in passing percentage from file
	int percentage; //need to convert percentage from string (in file) to int for math
	const int TOTAL_MATH_TYPES = 4;
	char mathTypes[TOTAL_MATH_TYPES] = { '+', '-', '*', '/' };
	bool randomizeMathType = false;
	if (mathType == 'C')
	{
		randomizeMathType = true;
	}
	for (questions = 0; questions < 10; questions++) //display 10 questions each time quiz is started
	{
		if (randomizeMathType)//re-assigning mathType if it is 'C'
		{
			int index = randNum(0, 3);
			mathType = mathTypes[index]; //index varies 0-3
		}
		int num1 = randNum(magnitude, negatives); //calling randNum function with levels and sublevels parameters
		int num2 = randNum(magnitude, negatives);
		if (mathType == '/' && num2 == 0) // For divison, if denominator is 0, add 1 to denominator
										  // so that we don't try and divide by 0. 
		{
			num2 += 1;
		}

		cout << endl << num1 << " " << mathType << " " << num2 << "  =  "; //display equation and prompt for answer
		switch (mathType)
		{
		case '+':
			cin >> answer;
			if (answer == (num1 + num2)) //if correct
			{
				correctCount++; //add 1 to correct question count to determine quiz score
				cout << happyResponse() << endl << endl; //display congradulatory response, call function which randomates list of responses
			}
			else // or incorrect (could probably do this outside switch statement, but this is easier for division)
				cout << "Incorrect. The answer is " << (num1 + num2) << "." << endl //shows correct answer right away
				<< crappyResponse() << endl << endl; //display response, call function which randomates list of "wrong" responses
			break;
		case '-':
			cin >> answer;
			if (answer == (num1 - num2))
			{
				correctCount++;
				cout << happyResponse() << endl << endl;
			}
			else
				cout << "Incorrect. The answer is " << (num1 - num2) << "." << endl
				<< crappyResponse() << endl << endl;
			break;
		case '*':
			cin >> answer;
			if (answer == (num1 * num2))
			{
				correctCount++;
				cout << happyResponse() << endl << endl;
			}
			else
				cout << "Incorrect. The answer is " << (num1 * num2) << "." << endl
				<< crappyResponse() << endl << endl;
			break;
		case '/':
			//if (num2 == 0) //cannot divide by zero, so if denominator is 0
			//num2 += 1; // add one to the denominator so it is no longer 0
			cout << "please enter quotient (without remainder):  ";
			cin >> quotient;
			cout << "and remainder:  ";
			cin >> remainder;
			if ((quotient == (num1 / num2)) && (remainder == (num1 % num2))) // if both parts (quotient and remainder) are correct
			{
				correctCount++; //add 1 to correct count
				cout << happyResponse() << endl << endl; //display response
			}
			else
				cout << "Incorrect. The answer is " << (num1 / num2) << " with R = " << (num1 % num2) << "." << endl
				//displays correct answer, broken up with quotient and remainder
				<< crappyResponse() << endl << endl;
			break;
		}
	}
	inFile.open("storage/passing_percentage.txt"); //open percentage file
	inFile >> percentage; //read in current passing percentage from file where it's stored
	inFile.close();
	int currentScore = correctCount * 10;
	score = max(score, currentScore);
	if (currentScore >= percentage) //if percentage correct is at least the passing percentage stored in file...
	{
		//displays total score
		cout << "Your score is " << currentScore << "%. " << " Congratulations, you are ready to move on to the next level!" << endl << endl;
	}
	else
	{ //if score is less than the passing percentage set by instructor
		cout << "Your score is " << currentScore << "%." << " Please see your teacher for some extra help." << endl << endl;
	}
	system("pause"); //wait for key press to clear screen
	system("CLS"); //clear screen after they hit any key
}
//Generates random number, with proper number of digits for level and negative numbers for B levels.
int randNum(int level, bool negatives)
{
	int magnitude = pow(10, level); //magnitude for multiple digits according to level
	int num = rand() % magnitude;
	if (negatives)
	{
		num *= pow(-1, randNum(0, 9)); // using other randNum function [randNum(int min, int max)]
	}
	return num;
}
//Same function name, diff parameters, uses same min and max from previous randNum function (so different function).
int randNum(int min, int max)
{ // integer between 0 and 9
	int x = (rand() % (max - min + 1)) + min;
	return x;
}
// List of "correct answer" responses, to display in random order.
string happyResponse()
{
	//srand seed up top in int main function
	const int MIN_SIZE = 0;
	const int MAX_SIZE = 9; // 10 different responses (0 to 9 = 10)
	string response;
	string responses[10]; //negative responses will be in diff scope, so reusing variable "responses"
	responses[0] = "Correct!";
	responses[1] = "Great job!";
	responses[2] = "Cool beans!";
	responses[3] = "Correct-o-mundo!";
	responses[4] = "Cowabunga, dude!";
	responses[5] = "Smashing!";
	responses[6] = "Super-duper!";
	responses[7] = "Wow, way to be!";
	responses[8] = "Awesome, keep it up!";
	responses[9] = "Bingo!";
	//generate an index for array
	int index = randNum(MIN_SIZE, MAX_SIZE);  //initializing index for array
	response = responses[index];
	return response; // return randomly generated response to quiz function
}
//List of "wrong answer" responses, to display in random order.
string crappyResponse()
{
	const int MIN_SIZE = 0;
	const int MAX_SIZE = 9;
	string response;
	string responses[10]; //positive responses in diff scope, so reusing variable
	responses[0] = "Bummer, dude!";
	responses[1] = "Oops! Try again";
	responses[2] = "Bullocks!";
	responses[3] = "Mmm...Let's try another.";
	responses[4] = "Drat!";
	responses[5] = "False!";
	responses[6] = "Sorry, that's incorrect.";
	responses[7] = "Scat!";
	responses[8] = "Blast!";
	responses[9] = "Whoops! Keep trying!";
	//generate an index for array
	int index = randNum(MIN_SIZE, MAX_SIZE);  //initializing index for array
	response = responses[index];
	return response;
}

void jeopardyMenu()
{
	int numPlayers;
	int points[5][5];
	string names[3];
	char buzz[] = {'a', 'k', 'v'};

	points = initializeBoard(1);

	cout << "Welcome to the Math Board Game. Would you like to play a game with 2 or 3 players?" << endl;
	cin >> numPlayers;
	if (numPlayers > 1 && numPlayers < 4)
	{
		for (int i = 0; i < numPlayers; i++)
		{
			cout << endl << endl << "Great! Please enter Player " << i + 1 <<"'s first name:   ";
			cin >> names[i];
			cout << "Hi," << names[i] << ", your buzzer letter is " << buzz[i] << "." << endl;
		}
	}
	else
		cout << "You may only play this game with 2 or 3 players, please choose again." << endl;

	displayBoard(points);
}

void displayBoard(int points[5][5]) //rows, then columns
{
	int roundNum = 0; // round 1 or 2
	int points[5][5];
	char mathType[] = { '+', '-', '*', '/', '?' };
	//41
	cout << "+---------------------------------------+" << endl;
	for (int i = 0; i < 5; i++) {//display the operators
		cout << "|   " << mathType[i] << "   ";
	}
	cout << "|" << endl;
	cout << "|=======================================|" << endl;

	for (int i = 0; i < 5; i++) { //rows
		for (int j = 0; j < 5; j++) { //columns
			cout << "|  " << (i + 1) * 100 /** roundTwo*/ << "  ";
		}
		cout << "|" << endl;
	}
	cout << "+---------------------------------------+" << endl;

	if (points == 0)
		cout << 'x';
	else 
		cout << points << endl;
	//if box already done, cout << 'x';
	
}

void newGame(int round)
{
	//read in round
	//write board to file with new game
	open.file.....

	//round 1 or 2 (3 is "Bonus Round")
	int round = 1;
	int points;
	int points[5][5];
	for (int i = 0; i < 5; i++) { //rows
		for (int j = 0; j < 5; j++) { //columns
			cout << "|  " << (i + 1) * 100 /** roundTwo*/ << "  ";
		}
	}
			return points;
			void playGame();
}

	void playGame()
	{
		string choice; 
		string names[3];
		answer = ((num1)mathType(num2));

		//display current players with their points*** read from file
		// read board from file

		cout << "It's " << names[i] << "'s turn! Please select a square (example: + 100, for 'addition for 100 points')." << endl;
		cin >> choice;

		//display 1 problem with mathType

		cout << "The answer is:  ";
		cin >> answer;
		//if correct, save points, show congrats message, cin another choice
		cout << happyResponse() << endl << endl;
		//else incorrect, show message, cin another buzzer letter
		cout << "Sorry, that's incorrect. Would anyone else like to try? [Please buzz in!]" << endl;
		//if no letter, (timer???*) display correct answer and zero (x) points on board
		//no timer... add 4th buzzer to dictate, " no one wants to answer"
		
	}