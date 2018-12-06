//Bowling game calculator
//Uses either string input or file input
//Created by Claycot, 2017-2018

#include <iostream> //User input 
#include <stdio.h> //This was quicker than <iomanip> for outputting score
#include <string> //Used for manipulating string input, as a result of notation
#include <fstream> //Used for reading from files
#include <deque> //Used for scoring each frame

using namespace std;

enum Ball {	
	BALL_SPARE = -1,
	BALL_MISS,
	BALL_ONE,
	BALL_TWO,
	BALL_THREE,
	BALL_FOUR,
	BALL_FIVE,
	BALL_SIX,
	BALL_SEVEN,
	BALL_EIGHT,
	BALL_NINE,
	BALL_STRIKE
};

string processInput(string rawInput, int ballsInFrame[]);
int scoreFrame(Ball gameBalls[], int strikeShift, int index);
Ball charToBall(char inputChar);
void printScore(int scoreFrames[], int totScore);

int main (int argc, char* argv[]) {
	//Greet user once on program run
	cout << "Enter the bowling line of the player.\n"
		<< "Add a space between each frame.\n"
		<< "Strikes are denoted by X and spares by /\n";
		
	//Test cases
	string testCase1 = "8- 7- 53 9/ 9/ X 8- 51 3/ 9-"; // = 122
	string testCase2 = "8/ 9- 44 72 9- X X 8- 35 9/7"; // = 133
	string testPerfect = "X X X X X X X X X XXX"; // = 300
	
	string userInput = "q";
	deque<string> fileInput = {};
	
	if (argc == 2) {
		//Get bowling strings from file
		ifstream inputFile;
		string fileName = argv[1];
		inputFile.open(fileName);	
		string tempLine = "";
		while (!inputFile.eof()) {
			inputFile >> ws;	
			getline(inputFile, tempLine);
			fileInput.push_back(tempLine);
		}
		inputFile.close();
	}
		
	do {
		int score = 0;
		int frameScore[10] = {};
		int ballsInFrame[10] = {};
		int indexFrame[10] = {};		
		Ball gameBalls[21] = {};	
		
		if (fileInput.empty()) {
			//Get bowling line from the user		
			cout << "Enter the bowling line, or 'q' to quit.\n";
			userInput = "q";
			cin >> ws;
			getline(cin, userInput);
		}
		else {
			//Get bowling line from filled deque
			userInput = fileInput.front();
			fileInput.pop_front();
		}
				
		if (userInput != "q" && userInput != "Q"){
			//Get a good input
			string goodInput = processInput(userInput, ballsInFrame);
			
			//Index frames (eg. Frame X starts at Ball Y)
			int numBalls = 0;
			for (int index = 0; index < 10 ; index++){
				indexFrame[index] = numBalls;
				numBalls += ballsInFrame[index];
			}
			
			//Turn the input string into an array of Balls
			for (int index = 0; index < goodInput.length(); index++) {				
				gameBalls[index] = charToBall(goodInput[index]);
			}
			
			//Calculate the bowling score
			for (int index = 0; index < 10; index++) {	
				frameScore[index] = scoreFrame(gameBalls, indexFrame[index], index);
				score += frameScore[index];
			}
			
			//If a valid score was calculated, output that score
			if (score > -1) {
				printScore(frameScore, score);
			}
		}
	} while (userInput != "q" && userInput != "Q");
		
	return 0;
}

//Process the input to remove any rough edges
string processInput(string rawInput, int ballsInFrame[]) {
	string matchString = "/-123456789Xx";
	string newInput;
	bool charMatched = false;
	int prevFrameIndex = 0;
	int frameNum = 0;
	
	//Loop through each character in the input
	for (int index = 0; index < rawInput.length(); index++) {
		charMatched = false;
		//If the character isn't a space, go forth
		if (rawInput[index] != ' ') {
			//Loop through acceptable characters to check validity of input
			for (int strIndex = 0; strIndex < matchString.length() + 1; strIndex++) {
				//If the character matches, add it to new string
				if (rawInput[index] == matchString[strIndex]) {
					newInput += rawInput[index];
					charMatched = true;
				}
			}
			//If the character doesn't match, output an error message
			if (!charMatched) {
				cout << "Bad character found... " << rawInput[index] << "\n";
				newInput += '0';
			}
		}
		//If the character was a space, the frame ended
		else {
			ballsInFrame[frameNum] = index - prevFrameIndex;
			frameNum++;
			prevFrameIndex = index + 1;
		}
	}
	ballsInFrame[9] = rawInput.length() - prevFrameIndex;
	
	//Make sure a good number of frames was entered
	if (frameNum != 9) {
		cout << "Bad number of frames entered! Use a space between frames.\n";
		newInput = "---------------------";
	}
	
	return newInput;
}

//Return the score from each individual frame
int scoreFrame(Ball gameBalls[], int strikeShift, int index) {
	int score = 0;
	
	Ball ballOne = gameBalls[strikeShift];
	Ball ballTwo = gameBalls[strikeShift + 1];
	
	//Logic to score
	if (ballOne != BALL_STRIKE) {
		if (ballTwo != BALL_SPARE) {			
			score += static_cast<int>(ballOne);
			score += static_cast<int>(ballTwo);
		}
		else if (ballTwo == BALL_SPARE) {
			Ball ballThree = gameBalls[strikeShift + 2];
			score += (10 + ballThree);
		}
	}
	else if (ballOne == BALL_STRIKE) {
		Ball ballThree = gameBalls[strikeShift + 2];
		if (ballThree == BALL_SPARE) {
			ballThree = static_cast<Ball>(10 - static_cast<int>(ballTwo));
		}
		score += (10 + ballTwo + ballThree);
	}
	
	//Output score for each frame?
	//cout << "Frame #" << index + 1 << " score: " << score << "\n";
	
	return score;
}

//Convert an input char to an enum Ball
Ball charToBall(char inputChar) {
	string matchString = "/-123456789X";
	Ball outputBall = BALL_MISS;
	
	//Attempt to match each ball to the enumeration
	for (int n = -1; n <= 10; n++){
		if (matchString[n + 1] == inputChar) {
			outputBall = static_cast<Ball>(n);
		}
	}
	
	//Additional check in case strikes were lower-case
	if (inputChar == 'x') {
		outputBall = BALL_STRIKE;
	}
	
	return outputBall;
}

void printScore(int scoreFrames[], int totScore){
	//Display the frame numbers
	cout << "|Frame";
	for (int i = 1; i <= 10; i++) {
		printf("|%3d", i);
	}
	cout << "|TOTAL|\n";
	
	//Divider...
	//cout << "|=====|---|---|---|---|---|---|---|---|---|---|=====|\n";
	cout << "|=====|===|===|===|===|===|===|===|===|===|===|=====|\n";
	
	//Display the scores for each frame, including total
	cout << "|Score";
	for (int i = 1; i <= 10; i++) {
		printf("|%3d", scoreFrames[i - 1]);
	}
	printf("| %3d |\n\n", totScore);
	
	return;
}
