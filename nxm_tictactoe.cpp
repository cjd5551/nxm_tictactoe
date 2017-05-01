// Name     : Christian Dunn
// Date     : Mar. 14, 2017
// Purpose  : To create a tic-tac-toe game that utilizes a simple randomizer AI
// Compiler : Visual Studio 2016

#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int EMPTY = 0;

int get_Turn();															// Function prototype
int get_WinScenario();													// Function prototype
pair<int, int> get_Dimensions();										// Function prototype
void player_First(int, int, int);										// Function prototype
void ai_First(int, int, int);											// Function prototype
int openBoard(int, int, int**);											// Function prototype
void showBoard(int**, int, int);										// Function prototype
int **get_Init(int, int, int**);										// Function prototype
int get_Integer(int, int, int**);										// Function prototype
bool board_is_full(int, int, int**);									// Function prototype
bool winCheck(int, int, int**, int);									// Function prototype
pair<int, int> randNum(int, int, int**);								// Function prototype
pair<int, int> getUserInput(int, int, int**);							// Function prototype

// Function : main()
// Purpose  : To compile main operations used in producing output for game
int main() {
	int rows, columns, win_scenario, turn_val;							// Declare integer variables used in collecting initial game data
	pair<int, int> temp;												// Declare variable of a pair of integers to hold temporary dimensions of board

	temp = get_Dimensions();											// Call function to get board dimensions from user and store in respective memory location
	rows = temp.first;													// Set number of rows equal to the first value stored in pair of dimensions
	columns = temp.second;												// Set number of columns equal to the second value stored in pair of dimensions

	win_scenario = get_WinScenario();									// Call function to get win scenario from user and store in respective memory location

	turn_val = get_Turn();												// Call function to get desired order of player moves and store in respective memory location

	if (turn_val == 0) {												// If user selected [0], user plays first
		player_First(rows, columns, win_scenario);						// Call function that completes operations according to player making the first move
	}
	else {																// Otherwise AI plays first
		ai_First(rows, columns, win_scenario);							// Call function that completes operations according to AI making the first move
	} // end if-else

	system("pause");
	return 0;
} // end main()

  // Function : openBoard()
  // Purpose  : Return the value of the number of spots unoccupied within game board
int openBoard(int rows, int columns, int **board) {
	int num = 0;														// Declaration of integer variable used in returning the number of open locations

	for (int i = 0; i < rows; i++) {									// For loop to iterate through each row of game board
		for (int j = 0; j < columns; j++) {								// Inner for loop to iterate through each column of game board
			if (board[i][j] == 0)										// If statement to check for status of current position in board
				num++;													// If location is empty increment the number of open spaces
		} // end inner for
	} // end outer for

	return num;															// Return the value of open spaces in the current board
} // end openBoard()

  // Function : get_Integer()
  // Purpose  : Return integer input that has been check for inconsistencies
int get_Integer(int rows, int columns, int **board) {
	int val;															// Integer variable declaration to hold value of user input
	bool valid = false;													// Boolean variable declaration used in error checking of user input

	while (!valid) {													// While loop to iterate as invalid input is brought into memory
		cin >> val;														// Read user input into memory and store in respective location
		valid = true;													// Assume no errors were present in reading user data entered

		if (cin.fail()) {												// Check user input for type error or inconsistency
			cin.clear();												// If error in reading in data occurs clear the input stream
			cin.ignore();												// Ignore any incoming data from corrupted stream
			cerr << "Invalid! Enter positive integer value: ";			// Prompt user to enter a new valid integer value
			valid = false;												// Notify system that invalid input was read into memory and iterate through loop
		}
		else if (val < 0) {												// Check if user input was negative integer value
			cerr << "Invalid! Enter positive integer value: ";			// If user input was negative value prompt user to enter new valid integer value
			valid = false;												// Notify system that invalid input was read into memory and iterate through loop
		}
		else if (val >(rows*columns)) {									// Check that user input is within bounds of game board area
			cerr << "Invalid! Enter value in given range [0-"			// If integer value is not within area of board prompt user for data within appropriate range
				<< rows*columns << "]: ";
			valid = false;												// Notify system that invalid input was read into memory and iterate through loop
		}
		else if (val > openBoard(rows, columns, board)) {				// Check that user input is within bounds of available positions on current board
			cerr << "Invalid! Enter value in given range [0-"			// If integer value is not within range of available positions prompt user for data within appropriate range
				<< openBoard(rows, columns, board) << "]: ";
			valid = false;												// Notify system that invalid input was read into memory and iterate through loop
		} // end if-else 

	} // end while

	return val;															// Return valid user input value held within specified memory location
} // end get_Integer()

  // Function : get_Init()
  // Purpose  : Get initial board settings for current game
int **get_Init(int rows, int columns, int **board) {
	pair <int, int> temp;												// Declare a pair of integers to hold temporary location values
	int wh_num, blk_num;												// Integer variable declarations to hold user input on number of occupied positions

	cerr << "Enter number of initial white pieces: ";					// Prompt user to enter number of initial white pieces occupying board
	wh_num = get_Integer(rows, columns, board);							// Function call to read integer value into memory and check for errors

	for (int i = 0; i < wh_num; i++) {									// For loop to iterate through for all white pieces
		temp = getUserInput(rows, columns, board);						// For each piece get the location on the board of the piece
		board[temp.first][temp.second] = 1;								// Mark the location on the board occupied by white piece
	} // end for

	cerr << "Enter number of initial black pieces: ";					// Prompt user to enter number of initial black pieces occupying board
	blk_num = get_Integer(rows, columns, board);						// Function call to read integer value into memory and check for errors

	for (int i = 0; i < blk_num; i++) {									// For loop to iterate through for all black pieces
		temp = getUserInput(rows, columns, board);						// For each piece get the location on the board of the piece
		board[temp.first][temp.second] = 2;								// Mark the location on the board occupied by black piece
	} // end for

	return board;														// Return the new modified board
} // end get_Init()

  // Function : get_WinScenario()
  // Purpose  : To obtain the number of like-valued moves in a row to indicate a win
int get_WinScenario() {
	int win_val;														// Declare integer variable to hold return value of user desired input
	bool valid = false;													// Boolean declaration used in error checking of input from user

	cerr << "Enter win scenario [larger than board = no winner] : ";	// Prompt the user to enter desired value for win scenario
	while (!valid) {													// While loop to collect input until valid input from user is stored in memory
		valid = true;													// Assume user entered appropriate input value
		cin >> win_val;													// Read input value from user and store in respective/appropriate memory location

		if ((cin.fail()) || (win_val < 3)) {							// If statement to check for errors or inconsistencies in user input
			cin.clear();												// If error or inconsistency occured, clear the input stream buffer
			cin.ignore();												// Ignore any data still occuring in input stream or after read character
			cerr << "Enter positive integer value >= 3 : ";				// Prompt user to enter new valid input value with appropriate constraints
			valid = false;												// Set valid to false to read in data again and check for error
		} // end if
	} // end while

	return win_val;														// Return valid user desired input value for win scenario
} // end get_WinScenario()

  // Function : get_Dimensions()
  // Purpose  : To obtain the game board's dimensions desired by the user
pair<int, int> get_Dimensions() {
	pair<int, int> m_n;													// Declare pair variable of integers used as return value holding user input for dimensions		
	bool valid = false;													// Boolean declaration used in error checking of input from user
	bool was_invalid = false;											// Boolean declaration used in determining whether previous input error occured

	cerr << "Enter number of rows    : ";								// Prompt user for desired number of rows for game board
	while (!valid) {													// While loop to perform operations as input is invalid
		valid = true;													// Assume correct input was entered by user for prompted value
		cin >> m_n.first;												// Read user input into respective/appropriate memory location

		if ((cin.fail()) || (m_n.first < 3)) {							// If statement to check for errors or inconsistencies in user input
			cin.clear();												// If error or inconsistency occured, clear the input stream buffer
			cin.ignore();												// Ignore any data still occuring in input stream or after read character
			was_invalid = true;											// Error has occured so set boolean to indicate an error occured previous to more input
			cerr << "Enter integer value rows [>= 3]    : ";			// Prompt user to input new value for rows with appropriate constraints
			valid = false;												// Set valid to false to read in data again and check for error
		} // end if
	} // end while

	valid = false;														// After first value has been successfule, reset boolean for next value input

	cerr << "Enter number of columns : ";								// Prompt user to enter value for number of columns for game board
	while (!valid) {													// While loop to check validity of input from user and continue as invalid input succeeds
		valid = true;													// Assume correct input was entered by user for prompted value
		cin >> m_n.second;												// Read user input into respective/appropriate memory lovation

		if ((cin.fail()) || (m_n.second < 3)) {							// If statement to check for errors or inconsistencies in user input
			cin.clear();												// If error or inconsistency occured, clear the input stream buffer
			cin.ignore();												// Ignore any data still occuring in input stream or after read character
			if (was_invalid == true) {									// Check to verify whether error occured in retrieval of previous content
				cerr << "Enter integer value rows [>= 3]    : "			// If error occured, output last prompt
					<< m_n.first << endl;								// Also output the value entered as if no error had occured
			}
			else {
				cerr << "Enter number of rows    : "					// Otherwise output previous prompt as if no error occured
					<< m_n.first << endl;								// Also output the value entered 
			} // end if-else
			cerr << "Enter integer value columns [>= 3] : ";			// Prompt user to input new value for columns with appropriate constraints
			valid = false;												// Set valid to false to read in data again and check for error
		} // end if
	} // end while

	return m_n;															// Return the dimensions as a pair of integers from the user input
} // end get_Dimensions

  // Function : get_Turn()
  // Purpose  : To prompt user with who goes first and return user selection
int get_Turn() {
	int  turn_val;														// Integer variable used to store user input and return from function call
	bool valid = false;													// Boolean variable used in testing input value and verifying if valid input

	cerr << "[0] - Player makes first move\n";							// Prompt user with selection for player makes first move
	cerr << "[1] - AI makes first move\n\n";							// Prompt user with selection for AI to make first move
	cerr << "Enter turn value : ";										// Prompt input

	while (!valid) {													// While loop that loops while input is not valid
		valid = true;													// Assume user enters valid data
		cin >> turn_val;												// Store user input into declared integer variable

		if ((cin.fail()) || (turn_val < 0) || (turn_val > 1)) {			// If statement to test if in bounds and that input is correct data type
			cin.clear();												// Clear the input buffer
			cin.ignore();												// Ignore any input or streams left from last input call
			cerr << "[0] - Player makes first move\n";					// Prompt player option
			cerr << "[1] - AI makes first move\n\n";					// Prompt AI option
			cerr << "Enter valid integer : ";							// Prompt user for input
			valid = false;												// Since prior input was incorrect loop again to test new input
		} // end if
	} // end while

	return turn_val;													// return user input
} // end get_Turn()

  // Function : player_First()
  // Purpose  : Calls functions and variables if user selected to go first
void player_First(int rows, int columns, int win_scenario) {
	int** board = new int*[rows];										// Declare two dimensional array to hold board information

	for (int i = 0; i < rows; i++) {									// For-loop used to dynamically allocate board data 
		board[i] = new int[columns];									// Specify that each row has row amount of columns
	} // end for

	for (int i = 0; i < rows; i++) {									// For-loop used to initialize game board
		for (int j = 0; j< columns; j++) {								// For-loop initializes game board by each position in board
			board[i][j] = EMPTY;										// Each location is initialized to global variable EMPTY
		} // end inner for
	} // end outer for

	board = get_Init(rows, columns, board);

	while (!board_is_full(rows, columns, board)) {						// While loop to perform is board positions are not all taken
		showBoard(board, rows, columns);								// Show last board state and environment
		pair<int, int> input = getUserInput(rows, columns, board);		// Get user input for desired position on board
		board[input.first][input.second] = 1;							// Set location value to indicate user selection
		showBoard(board, rows, columns);								// Display new updated screen and information

		if (winCheck(rows, columns, board, win_scenario)) {				// Check if user remove resulted in a winning scenario
			cerr << "Congratulations! You win!\n\n";					// Prompt user and notify that win has occured
			break;														// Since win occured break from loop to not allow anymore moves to be made
		} // end if

		if (!board_is_full(rows, columns, board)) {						// If board is not full and a win scenario is not present
			cerr << "Generating AI move...\n";							// Output prompt to show AI is devising move		
			pair<int, int> ai_input = randNum(rows, columns, board);	// Generate random number and represent as location on board
			board[ai_input.first][ai_input.second] = 2;					// Set location value to indicate AI position on board
		} // end if

		if (winCheck(rows, columns, board, win_scenario)) {				// Check if AI move resulted in a winning scenario
			showBoard(board, rows, columns);							// Display new updated board
			cerr << "Sorry! You lose!\n\n";								// If win occured prompt user that they lost
			break;														// Since AI won break from loop to not allow anymore moves to be made
		} // end if
	} // end while

	for (int i = 0; i < rows; i++) {									// For loop to delete each pointer to columns and rows of board
		delete[] board[i];
	} // end for
	delete[] board;														// Delete any remaining memory to prevent memory leaks
} // end player_First()

  // Function : ai_First
  // Purpose  : Calls function and variables if user prompted AI to perform first move
void ai_First(int rows, int columns, int win_scenario) {
	int** board = new int*[rows];										// Declare two dimensional array to hold board information

	for (int i = 0; i< rows; i++) {										// For-loop used to dynamically allocate board information
		board[i] = new int[columns];									// Specify that each row has row amount of columns
	} // end for

	for (int i = 0; i< rows; i++) {										// For-loop used to initialize game board
		for (int j = 0; j< columns; j++) {								// For-loop initializes board by each position on board
			board[i][j] = EMPTY;										// Each position is initialized to global variable EMPTY
		} // end inner for
	} // end outer for

	board = get_Init(rows, columns, board);

	while (!board_is_full(rows, columns, board)) {						// While to perform while board still consists of empty positions
		showBoard(board, rows, columns);								// Show current board state and environment
		cerr << "Generating AI move...\n";								// Prompt user that AI move is being generated
		pair<int, int> ai_input = randNum(rows, columns, board);		// Generate random number used as AI desired move
		board[ai_input.first][ai_input.second] = 1;						// Set location value to AI desired position
		showBoard(board, rows, columns);								// Display updated board and information to screen

		if (winCheck(rows, columns, board, win_scenario)) {				// Check if AI selected move resulted in a winning scenario
			cerr << "Sorry! You lose!\n\n";								// If AI won, prompt user that they lost 
			break;														// Break out of while loop so no more turns can be generated
		} // end if

		if (!board_is_full(rows, columns, board)) {						// Check to verify that board is not full
			pair<int, int> input = getUserInput(rows, columns, board);  // If board is not full, allow user to input desired position
			board[input.first][input.second] = 2;						// Set location value to user desired position
		} // end if

		if (winCheck(rows, columns, board, win_scenario)) {				// Check if user move resulted in a winning scenario
			showBoard(board, rows, columns);							// Show updated board and data from user input
			cerr << "Congratulations! You win!\n\n";					// Since user won, prompt user that win occured
			break;														// Break out of while loop so no more turns can be generated
		} // end if
	} // end while

	for (int i = 0; i < rows; i++) {									// For-loop to delete each pointer to columns and rows of board
		delete[] board[i];
	} // end for
	delete[] board;														// Delete any remaining memory to prevent memory leaks
} // end ai_First()

  // Function : board_is_full
  // Purpose  : Iterate through board positions and determine whether board is full or not
bool board_is_full(int rows, int columns, int **board) {
	for (int i = 0; i < rows; i++) {									// For-loop that iterates through each row
		for (int j = 0; j < columns; j++) {								// For-loop that iterates through each column value in each row
			if (board[i][j] == 0)										// If statement to determine whether current position is empty
				return false;											// If current position is empty, return that board is not full
		} // end inner for
	} // end outer for
	return true;														// Otherwise return that board is full for each position
} // end board_is_full()

  // Function : getUserInput()
  // Purpose  : To prompt user to enter position of next move and return as pair<int,int> value
pair<int, int> getUserInput(int rows, int columns, int **board) {
	bool valid = false;													// Declare boolean variable used in type checking of input
	pair<int, int> userInput;											// Declare pair variable to hold location of user input
	cerr << "Enter desired location : ";								// Prompt user to enter desired board position

	while (!valid) {													// While loop to test whether user enters valid data
		valid = true;													// Assume user entered valid data
		cin >> userInput.first >> userInput.second;						// Store user input in respective pair integer values

		if (cin.fail() || (userInput.first > (rows - 1))
			|| (userInput.second > (columns - 1))
			|| (userInput.first < 0) || (userInput.second < 0)) {		// If statement that tests if input in bounds and if correct input data type
			cin.clear();												// If invalid input, clear input buffer memory
			cin.ignore();												// Ignore any other streams or input within memory buffer
			showBoard(board, rows, columns);							// Show updated board and information
			cerr << "Invalid! Please enter valid location : ";			// Prompt user that input was invalid and prompt to enter new data
			valid = false;												// Set valid as false to loop again and read in new data from user
		}
		else if (board[userInput.first][userInput.second] != 0) {		// Test if user entered location that is not empty
			showBoard(board, rows, columns);							// Show updated board and information
			cerr << "Invalid! Please enter unoccupied location : ";		// Prompt user to enter location that is unoccupied
			valid = false;												// Set valid as false to loop again and read in new data from user
		} // end if-else
	} // end while
	return userInput;													// Return valid input position from user
} // end getUserInput

  // Function : randNum() 
  // Purpose  : Based off the current board generate a random number referencing empty position
pair<int, int> randNum(int rows, int columns, int **board) {
	int count = 0, randnum;												// Declare integer variables to hold count values and rand number
	pair<int, int> randLoc;												// Declare pair variable that stores location of symbolic random generated number 

	for (int i = 0; i < rows; i++) {									// For loop to iterate through each row of game board
		for (int j = 0; j < columns; j++) {								// For loop that iterates through each column of game board
			if (board[i][j] == 0)										// If current board position is empty
				count++;												// Increment count to determine number of available positions on board
		} // end inner for
	} // end outer for

	randnum = (rand() % count) + 1;										// Generate random number based off of number of empty positions on board
	count = 0;															// Reset count to be used in setting random position value

	for (int i = 0; i < rows; i++) {									// For loop to iterate through each row of game board
		for (int j = 0; j < columns; j++) {								// For loop to iterate through each column of game board
			if (board[i][j] == 0) {										// If current board position is empty
				count++;												// Increment count to determine which symbolic empty postion is currently accessed
				if (count == randnum) {									// If symbolic random number is equal to symbolic value of current position
					randLoc.first = i;									// Set first value of pair to value of current row
					randLoc.second = j;									// Set second value of pair to value of current column
				} // end inner if
			} // end outer if
		} // end inner if
	} // end outer if
	return randLoc;														// Return randomly generated position
} // end randNum()

  // Function : showBoard()
  // Purpose  : Output display of board to screen for user prompting
void showBoard(int ** board, int row, int column) {
	cerr << "The current board:\n\n";									// Prompt board display
	for (int i = 0; i < row; i++) {										// For loop to iterate through each row witin board environment
		for (int j = 0; j < column; j++) {								// For loop to iterate through each column within each row of board
			cerr << board[i][j] << " ";									// Display current board values to screen
		} // end inner for
		cerr << endl;
	} // end outer for
	cerr << "\n\n";
} // showBoard()

  // Function : winCheck()
  // Purpose  : Checks board to determine whether a win scenario exists in the current state
bool winCheck(int rows, int columns, int **board, int win_scenario) {
	int last_val = 0;

	// ************************VERTICAL CHECK************************
	for (int j = 0; j < columns; j++) {									// For loop to loop through by columns in increasing manner
		int count = 1;													// Integer variable declaration to hold the count of pieces in a row
		for (int i = 0; i < rows; i++) {								// Inner for loop to iterate through vertically within each current column
			if (i == 0) {												// Check if current element is first element in sequence
				last_val = board[i][j];									// Initiate last value if first element in sequence to avoid error
			}
			else {
				if ((last_val == board[i][j]) && (board[i][j] != 0)) {	// If the current element is equal to last element and the current element is not zero
					count++;											// Increment count variable since a pattern is occuring
					if (count == win_scenario) {						// Check if incrementation caused win scenario
						return true;									// If win scenario occured return true that a win has occured
					}
					else {
						last_val = board[i][j];							// If no win occured set last value to current value and continue
					} // end nested inner if-else
				}
				else {
					count = 1;											// If pattern is not occuring reset count to initial value
					last_val = board[i][j];								// Set last value to current value and continue through to next column
				} // end nested if-else
			} // end if-else
		} // end inner for
	} // end outer for

	  // ***********************HORIZONTAL CHECK***********************
	for (int i = 0; i < rows; i++) {									// For loop to loop through by rows in increasing manner
		int count = 1;													// Integer variable declaration to hold the count of pieces in a row
		for (int j = 0; j < columns; j++) {								// Inner for loop to iterate through horizontally within each current row
			if (j == 0) {												// Check if current element is first element in sequence
				last_val = board[i][j];									// Initiate last value if first element in sequence to avoid error
			}
			else {
				if ((last_val == board[i][j]) && (board[i][j] != 0)) {	// If the current element is equal to last element and the current element is not zero
					count++;											// Increment count variable since a pattern is occuring
					if (count == win_scenario) {						// Check if incrementation caused win scenario
						return true;									// If win scenario occured return true that a win has occured
					}
					else {
						last_val = board[i][j];							// If no win occured set last value to current value and continue
					} // end nested inner if-else
				}
				else {
					count = 1;											// If pattern is not occuring reset count to initial value
					last_val = board[i][j];								// Set last value to current value and continue through to next row
				} // end nested if-else
			} // end if-else
		} // end inner for
	} // end outer for

	  // *******************BOT_ROW to RGT_COL CHECK*******************
	for (int i = (rows - 1); i > -1; i--) {								// For loop to loop through by diagonals from bottom right to bottom left
		int count = 1;													// Integer variable declaration to hold the count of pieces in a row
		for (int j = i, k = 0; j < rows && k < columns;					// Inner for loop to iterate through diagonally as starting diagonal point changes
			j++, k++) {
			if ((j == i) && (k == 0)) {									// Check if current element is first element in sequence
				last_val = board[j][k];									// Initiate last value if first element in sequence to avoid error
			}
			else {
				if ((last_val == board[j][k]) && (board[j][k] != 0)) {	// If the current element is equal to last element and the current element is not zero
					count++;											// Increment count variable since pattern is occuring
					if (count == win_scenario) {						// Check if incrementation caused win scenario
						return true;									// If win scenario occured return true that a win has occured
					}
					else {
						last_val = board[j][k];							// If no win occured set last value to current value and continue
					} // end nested inner if-else
				}
				else {
					count = 1;											// If pattern is not occuring reset count to initial value
					last_val = board[j][k];								// Set last value to current value and continue through to next diagonal
				} // end nested if-else
			} // end if-else
		} // end inner for
	} // end outer for

	  // *******************TOP_ROW to RGT_COL CHECK*******************
	for (int i = 0; i < columns; i++) {									// For loop to loop through by diagonals from top left to top right
		int count = 1;													// Integer variable declaration to hold the count of pieces in a row
		for (int j = i, k = 0; j < columns && k < rows;					// Inner for loop to iterate through diagonally as starting diagonal point changes
			j++, k++) {
			if ((j == i) && (k == 0)) {									// Check if current element is first element in sequence
				last_val = board[k][j];									// Initiate last value if first element in sequence to avoid error
			}
			else {
				if ((last_val == board[k][j]) && (board[k][j] != 0)) {	// If the current element is equal to last element and the current element is not zero 
					count++;											// Increment count variable since pattern is occuring
					if (count == win_scenario) {						// Check if incrementation caused win scenario
						return true;									// If win scenario occured return true that a win has occured
					}
					else {
						last_val = board[k][j];							// If no win occured set last value to current value and continue
					} // end nested inner if-else
				}
				else {
					count = 1;											// If pattern is not occuring reset count to initial value
					last_val = board[k][j];								// Set last value to current value and continue through to next diagonal
				} // end nested if-else
			} // end if-else
		} // end inner for
	} // end outer for

	  // *******************BOT_ROW to LFT_COL CHECK*******************
	for (int i = (rows - 1); i > -1; i--) {								// For loop to loop through by diagonals from bottom right to bottom left
		int count = 1;													// Integer variable declaration to hold the count of pieces in a row
		for (int j = i, k = (columns - 1); j < rows && k > -1;			// Inner for loop to iterate through diagonally as starting diagonal point changes
			j++, k--) {
			if ((j == i) && (k == (columns - 1))) {						// Check if current element is first element in sequence
				last_val = board[j][k];									// Initiate last value if first element in sequence to avoid error
			}
			else {
				if ((last_val == board[j][k]) && (board[j][k] != 0)) {	// if the current element is equal to last element and the current element is not zero
					count++;											// Increment count variable since pattern is occuring
					if (count == win_scenario) {						// Check if incrementation caused win scenario
						return true;									// If win scenario occured return true that a win has occured
					}
					else {
						last_val = board[j][k];							// If no win occured set last value to current value and continue
					} // end nested inner if-else
				}
				else {
					count = 1;											// If pattern is not occuring reset count to initial value
					last_val = board[j][k];								// Set last value to current value and continue through to next diagonal
				} // end nested if-else
			} // end if-else
		} // end inner for
	} // end outer for

	  // *******************TOP_ROW to LFT_COL CHECK*******************
	for (int i = (columns - 1); i > -1; i--) {							// For loop to loop through by diagonals from top right to top left
		int count = 1;													// Integer variable declaration to hold the count of pieces in a row
		for (int j = i, k = 0; j > -1 && k < rows;						// Inner for loop to iterate through diagonally as starting diagonal point changes
			j--, k++) {
			if ((j == i) && (k == 0)) {									// Check if current element is first element in sequence
				last_val = board[k][j];									// Initiate last value if first element in sequence to avoid error
			}
			else {
				if ((last_val == board[k][j]) && (board[k][j] != 0)) {	// If the current element is equal to last element and the current element is not zero
					count++;											// Increment count variable since pattern is occuring 
					if (count == win_scenario) {						// Check if incrementation caused win scenario
						return true;									// If win scenario occured return true that a win has occured
					}
					else {
						last_val = board[k][j];							// If no win occured set last value to current value and continue
					} // end nested inner if-else
				}
				else {
					count = 1;											// If pattern is not occuring reset count to initial value
					last_val = board[k][j];								// Set last value to current value and continue through to next diagonal
				} // end nested if-else
			} // end if-else
		} // end inner for
	} // end outer for

	return false;
} // end winCheck()