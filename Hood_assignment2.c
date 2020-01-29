#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void strreplace(char string[], char search[], char replace[]); //function prototype for strreplace
void help(); //function prototype for help

void strreplace(char string[], char search[], char replace[]) { //the string replace function searches for a substring in a string and replaces that substring with another string
	char buffer[1000]; //buffer array of size 1000
	char*p = string; //sets pointer p to point to string passed in
	while ((p = strstr(p, search))) { //strstr finds first occurrencce of substring 'search' in string 'p'. p is set to pointer to first occurence
		strncpy_s(buffer, 100, string, p - string); //copy to buffer all values up to the point where first occurence is
		buffer[p - string] = '\0'; //append null character at the end
		strcat_s(buffer, 100, replace); //append replacement string to end of buffer
		strcat_s(buffer, 100, p + strlen(search)); //append the rest of the string after first occurence to end of buffer 
		strcpy_s(string, 100, buffer); //copy buffer into string array
		p++; //increment the pointer
	}
}

void help() { //This help function prints out calculator functions and examples

	printf("\n");

	printf("%-15sExits this program.\n", "EXIT");

	printf("%-15sDisplays information about this program.\n", "HELP");

	printf("%-15sAs part of a mathematical expression the term MEMORY\n", "MEMORY");

	printf("%-15sis substituted by the value stored in memory. Otherwise, \n", "");

	printf("%-15sthe value stored in memory is displayed on-screen.\n", "");

	printf("%-15sErases stored memory and returns calculator to its\n", "RESET");

	printf("%-15sinitial \'start-up\' mode.\n", "");

	printf("%-15sSaves current answer to memory.\n\n", "STORE");

	printf("%-15s%-15s%-15s\n", "OPERATOR", "DESCRIPTION", "SYNTAX");

	printf("%-15s%-15s%-15s\n", "+", "addition", "[a + b :+a]");

	printf("%-15s%-15s%-15s\n", "-", "subtraction", "[a - b :-a]");

	printf("%-15s%-15s%-15s\n", "*", "multiplication", "[a * b:*a]");

	printf("%-15s%-15s%-15s\n", "/", "division", "[a / b:/a]");

	printf("%-15s%-15s%-15s\n", "^", "sqr(x)", "a^");

	printf("%-15s%-15s%-15s\n", "#", "sqrt(x)", "a#");

	printf("\nExample:\ta^ +b# / MEMORY (spacing optional)\n");

}


int main(void)
{
	char prev = '\0'; //variable to hold the previous character in the input string
	size_t top = 0; //variable to keep track of location in the operand array
	size_t len; //variable keep track of location in the hold array

	char input[100]; //array of 100 elements to hold the input string
	double memory = 0; //variable to hold memory variable, default is 0
	char * search = "memory"; //pointer to a string containing the word "memory". Used for the replace string function.
	char buffer[100] ="\0"; //buffer array of 100 elements intialized to null

	double operand[100] = { '\0' }; //double array of 100 elements to hold the operands from the input string
	char operation[100] = "\0"; //char array of 100 elements to hold the operations ( + , - , *, / )
	int count_decimal = 0; //integer variable to keep track of the number of decimal placements in a number
	unsigned char isValid = 1; //variable to keep track of validity (whether the input string is valid of calculations)
	double answer = 0; //variable to hold the current answer, default 0

	printf("Type \"HELP\" or enter a mathematical expression"); //beginning message

	while (1) //This program will enter an infinite loop and only break when the user exits
	{
		/* GET INPUT */

		printf("\n\nCalc:\> ");

		fgets(input, 100, stdin); //Use the fgets function to get input from user. Store the string in the input array,
								//allow a max of 100 characters to be read (including null) and choose the standard input stream

		/*CHECK THE MODE INPUTS*/

		if (!strcmp(input, "help\n") || !strcmp(input, "HELP\n")) { //strcmp compares strings. Returns 0 if the strings are equal
																	//if the user enters 'help' or 'HELP' then call the 'help' function
																	//which prints out calculator functions and examples

			help();
			continue; //return to the start of the while(1) loop and await new input from user
		}

		if (!strcmp(input, "store\n") || !strcmp(input, "STORE\n")) //if the user enters 'store' or 'STORE' then store the current
		{															//answer in the memory variable
			memory = answer; //
			continue;  //return to the start of the while(1) loop and await new input from user
		}

		if (!strcmp(input, "reset\n") || !strcmp(input, "RESET\n")) //if the user enters 'reset' or 'RESET' then refresh
		{															//variables to set calculator back to start-up
			memory = 0; //set memory to deafault value
			answer = 0; //set answer to default value
			printf("Type \"HELP\" or enter a mathematical expression"); //print the start message again
			continue; //return to the start of the while(1) loop and await new input from user
		}

		if (!strcmp(input, "exit\n") || !strcmp(input, "EXIT\n"))//if the user enters 'exit' or 'EXIT' break out of the while loop											
		{														
			break;
		}

		/* CHECK WHETHER USER INPUT "MEMORY"*/

		strreplace(input, search, "?"); //call the string replace function and replace every "memory" string with the placeholder character '?'
										//When parsing the input string this character will be used to indicate that the value stored in memory
										//should be added to the operand array for later calculations



		/****PARSE THE INPUT STRING CHARACTER BY CHARACTER USING A FOR LOOP****/


		for (int i = 0; i < strlen(input) - 1; i++) //for loop iterates through each character of the string. strlen gets the lengths of the string (excluding null character)
		{											//NOTE that fgets appends the '\n' character at the end of each input therefore we take 1 away from the length
			if (input[i] == 32) //32 is the ascii character value for space
			{					//if there is a space in the string ignore it and continue to next character
				continue;
			}
			else if (isdigit(input[i])) //if the character is a digit (0-9) append the character to the end of the buffer array
			{				
				len = strlen(buffer); //get the length of the string in the buffer
				buffer[len++] = input[i]; //append the character to the next element in the array
				buffer[len] = '\0'; //append the null character at the end
				prev = input[i]; //prev holds the current character for next iteration
			}
			else if (input[i] == '.') //if character is a decimal
			{
				count_decimal++; //increment decimal counter
				if ((i == 0) || (prev == '+' || prev == '-' || prev == '*' || prev == '/')) //if decimal is first character or comes after and operation character
				{
					len = strlen(buffer);
					buffer[len++] = '0'; //append 0 onto end of buffer
					buffer[len++] = input[i]; //append the decimal
					buffer[len] = '\0'; //append null character
					prev = input[i];
				}
				else if (isdigit(prev) && count_decimal <= 1) //if character before decimal is a digit and the decimal counter is less than or equal to 1
				{
					len = strlen(buffer); 
					buffer[len++] = input[i]; //append decimal onto end of number
					buffer[len] = '\0'; //append null character
					prev = input[i];
				}
				else //else incorrect decimal placement. Indicate invalid input and break.
				{
					printf("Sorry, incorrect decimal placement.");
					memset(&buffer[0], 0, sizeof(buffer));
					prev = input[i];
					isValid = 0;
					count_decimal = 0;
					break;
				}
			}
			else if ((input[i] == '+') || (input[i] == '-') || (input[i] == '*') || (input[i] == '/')) //if the character is plus, minus, multiplication or divide
			{
				if (i == 0) //if it is the first valid character entered then it indicates that the operation is to be performed on the last answer
				{
					operand[top++] = answer; //add last answer to the operand array
					operation[strlen(operation)] = input[i]; //add the operation character to the operation array
					operation[strlen(operation) + 1] = '\0'; //append null to the end
					prev = input[i]; //prev holds current character for the next iteration

				}
				else if (prev == '+' || prev == '-' || prev == '*' || prev == '/') //if the previous character is another operator character
				{																//then this is an invalid input
					printf("Sorry, invalid input. Make sure to only enter one operator between operands"); //cant have 2 operators in a row (only a standard calculator)
					prev = input[i];
					isValid = 0; //set isValid to 0 to indicate not a valid input string. Wont go through calcuation process
					count_decimal = 0;
					break; //break out of the for loop
				}
				else if (prev == '?') //if the previous character before the operation character was placeholder for memory
				{					//This means the operation is to be performed on the value stored in memory
					operand[top++] = memory; //add memory value to next element in operand array
					operation[strlen(operation)] = input[i]; //add the operation character to the operation array
					operation[strlen(operation) + 1] = '\0'; //append null
					prev = input[i]; //prev holds current character
				}
				else if (prev != 94 && prev != 35) //if the previous character is not # or * (94 and 35 is ascii value for ^ and #) 
				{								//then we need to add the value stored in the buffer array to the operand array
					operand[top++] = strtod(buffer, NULL); //strtod converts a string value to a float. The second parameter is a pointer value that points
															//to the next character after the decimal value. In this case we only have a double value so we can set the pointer
															//as null. Convert the string held in buffer to float and store in next element of operator array
					memset(&buffer[0], 0, sizeof(buffer)); //once the value in buffer has been stored, clear the array. This is what memset does
					operation[strlen(operation)] = input[i]; 
					operation[strlen(operation) + 1] = '\0';
					prev = input[i];
					count_decimal = 0;
				}
				else //else the previous characters were # or *. In this case the value has already been added to operand array (see below). We only need to add the operation characters to the appropriate array
				{
					operation[strlen(operation)] = input[i];
					operation[strlen(operation) + 1] = '\0';
					prev = input[i];
				}
				count_decimal = 0;
				
			}
			else if (input[i] == 94) //if the character is '^' indicating the square functionality
			{
				if (isdigit(prev)) //if the previous character was a digit
				{					//then we need to square the value held in the buffer and store in the next element of the operand array. 
					operand[top++] = strtod(buffer, NULL) * strtod(buffer, NULL); //convert buffer string to double and times it by itself, storing in operand array
					memset(&buffer[0], 0, sizeof(buffer)); //clear the buffer array
					prev = input[i];
				}
				else if (prev == '?') //else if the previous character is the placeholder for memory then the memory value is squared and stored in the operand array
				{
					operand[top++] = memory * memory; 
					prev = input[i];
				}
				else //else the user placed ^ in an incorrect place. Set invalid string and break out of the for loop
				{
					printf("Sorry, incorrect square operation placement");
					prev = input[i];
					isValid = 0;
					break;
				}
			}
			else if (input[i] == 35) //if the character input is '#' indicating the square root functionality
			{
				if (isdigit(prev)) //if the previous character was a digit, then we need to square root the value held in the buffer and store in next element of the operand array
				{
					operand[top++] = sqrt(strtod(buffer, NULL)); //sqrt is the math.h header function for square root
					memset(&buffer[0], 0, sizeof(buffer)); //clear the buffer array
					prev = input[i];
				}
				else if (prev == '?') //else if the previous character is the placeholder for memory then the memory value is square rooted and stored in the operand array
				{
					operand[top++] = sqrt(memory);
					prev = input[i];
				}
				else //else the user placed # in an incorrect place. Set invalid string and break out of the for loop
				{
					printf("Sorry, incorrect sqr placement");
					prev = input[i];
					isValid = 0;
					break;
				}
			}
			else if (input[i] == '?') //if the character input is the placeholder for memory value, the only thing to do is set prev to the current character ?
			{
				prev = input[i];
			}
			else //if the character input is any other character, set invalid string and break out of the foor loop
			{
				printf("Sorry, the expression contains an invalid character. Please try again. Type \"HELP\" for a list of valid functions");
				prev = input[i];
				isValid = 0;
				break;
			}
		}

		/**** PERFORM CALCULATIONS USING THE VALUES STORED IN OPERATION AND OPERAND ARRAYS  ******/
		/**** ONLY TO BE PERFORMED ON INPUT STRINGS THAT WERE DETERMINED VALID IN THE PREVIOUS FOR LOOP ****/

		if (isValid) //if valid string
		{
			if (prev != 94 && prev != 35 && prev != '?') // if the string ended in a number
														//then we need to store the last value in the string in the buffer array
			{
				operand[top] = strtod(buffer, NULL); //last element of operand array = the last number held in the buffer array
				memset(&buffer[0], 0, sizeof(buffer)); //clear the buffer array
			}
			else if (prev == '?') //if the string ended with the placeholder for memory then the final element in the operand array
			{						// is the value held in memory
				operand[top] = memory;
			}
			else //if the string ended with '#' or '^'
			{
				top--; //the last value is already stored in the operand array and as such we need to set top back to the last element in the array (because top was set to next with top++)
			}

			/* THIS IS THE MAIN CALCULATION STEP. THE SWITCH STATEMENT CHECKS WHICH OPERATION IS STORED AND ADDS THE VALUES SEQUENTIALLY IN THE OPERANDS ARRAY UNTIL THE FINAL VALUE IS STORED
			   IN THE LAST ELEMENT IN THE OPERANDS ARRAY. THIS IS THE ANSWER. NOTE THAT THIS PROCESS DEPENDS ON THE PARSING STEP AND THE OPERANDS ARRAY SHOULD ALWAYS HAVE 1 MORE ELEMENT STORED THAN
			   THE OPERATIONS ARRAY.*/

			if (prev != '+' && prev != '-' && prev != '*' && prev != '/') //if the string did not end with an operation character
			{
				for (int i = 0; i < strlen(operation); i++) //loop through the operation array until null is reached
				{
					switch (operation[i])
					{
					case '+': //if the operation is a plus
						operand[i + 1] = operand[i] + operand[i + 1]; //plus the number stored in ith element of operand array with number stored in the i + 1 position and store value in i + 1 position
						break;
					case '-': //if operation is a minus
						operand[i + 1] = operand[i] - operand[i + 1]; //same process with '-'
						break;
					case '*':
						operand[i + 1] = operand[i] * operand[i + 1]; //same process with '*'
						break;
					case '/':
						operand[i + 1] = operand[i] / operand[i + 1]; //same process with '/'
						break;
					}
				}
			}
			else //if string ended with an operation character then the string is invalid. Continue to the top of while loop.
			{
				printf("\n\nSorry, invalid expression. Make sure expression ends with an operand");
				memset(&input[0], 0, sizeof(input)); //before beginning while 1 loop again, clear input array ready for next input
				memset(&operand[0], 0, sizeof(operand)); //clear operand array
				memset(&operation[0], 0, sizeof(operation)); //clear operation array
				top = 0; //set top back to 0
				prev = '\0'; //set prev back to null
				continue;
			}

			answer = operand[top]; //after calculation, the answer is held in the last element of the operand array
			printf("\nAnswer:\> %f", answer); //print answer
		}
		memset(&input[0], 0, sizeof(input)); //before continuing to next input clear the input array, operand array and operation array
		memset(&operand[0], 0, sizeof(operand));
		memset(&operation[0], 0, sizeof(operation));
		top = 0; //set top to 0
		prev = '\0'; //set prev back to null
		isValid = 1; //set isValid back to one, ready for next input
	}


	printf("Exited Program"); //program exited
	getchar();



}