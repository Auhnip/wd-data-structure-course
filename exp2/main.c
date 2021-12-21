#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 128

#define INPUT_FILE "data.txt"
#define OUTPUT_FILE "output.txt"

#define ARRIVE_COMMAND "arrives"
#define LEAVE_COMMAND "departs"

#define PARK_SIZE 5

void car_arrive(STACK *park, const char *car_name, FILE *fout);

void car_leave(STACK *park, const char *car_name, FILE *fout);

void all_car_leave(STACK *park, FILE *fout);

void print_sorry_message(const CAR *car, FILE *file);

void print_leave_message(const CAR *car, FILE *file);

char *get_line(char *buff, int len, FILE *file);

int main(void)
{
	STACK park;

	char buff_line[BUFF_SIZE];
	char car_name[BUFF_SIZE];
	char command[BUFF_SIZE];

	init_stack(&park, PARK_SIZE);

	// open input file
	FILE *fin = fopen(INPUT_FILE, "r");
	if (!fin)
	{
		fprintf(stderr, "open file \"%s\" failed\n", INPUT_FILE);
		exit(EXIT_FAILURE);
	}

	// open output file
	FILE *fout = fopen(OUTPUT_FILE, "w");
	if (!fout)
	{
		fprintf(stderr, "open file \"%s\" failed\n", OUTPUT_FILE);
		exit(EXIT_FAILURE);
	}

	// read file
	while (get_line(buff_line, BUFF_SIZE, fin))
	{
		// get car name and command
		printf("==> get a statement: \"%s\"\n", buff_line);
		if (sscanf(buff_line, "%s %s", car_name, command) != 2)
		{
			fprintf(stderr, "statement invalid\n");
			continue; // while (getline(buff_line, BUFF_SIZE, fin))
		}

		printf("==> analyzed: car_name = \"%s\", action = \"%s\"\n", car_name, command);

		// if a car arrived
		if (!strcmp(command, ARRIVE_COMMAND))
		{
			car_arrive(&park, car_name, fout);
		}
		// else if a car leaved
		else if (!strcmp(command, LEAVE_COMMAND))
		{
			car_leave(&park, car_name, fout);
		}
		// if command invalid
		else
		{
			fprintf(stderr, "command \"%s\" invalid\n", command);
		}

		print_stack(&park);
	}

	all_car_leave(&park, fout);

	if (fclose(fin) || fclose(fout))
	{
		fprintf(stderr, "close files failed\n");
		exit(EXIT_FAILURE);
	}

	clear_stack(&park);

	return 0;
}

void car_arrive(STACK *park, const char *car_name, FILE *fout)
{
	static CAR car;
	strncpy(car.name, car_name, CAR_NAME_LENGTH);
	car.times = 0;

	// if stack is full
	if (!push_stack(park, &car))
	{
		// print message to file
		print_sorry_message(&car, fout);
	}
}

void car_leave(STACK *park, const char *car_name, FILE *fout)
{
	static STACK temp;

	init_stack(&temp, PARK_SIZE);

	int find = 0;
	CAR *ptr_car;

	while (!is_empty_stack(park))
	{
		ptr_car = top_stack(park);
		if (!strcmp(car_name, ptr_car->name))
		{
			find = 1;
			break; // while (!stack_is_empty(&park))
		}
		top_move_stack(&temp, park);
	}

	// if do not find this car
	if (!find)
	{
		fprintf(stderr, "car \"%s\" not found\n", car_name);

		while (!is_empty_stack(&temp))
			top_move_stack(park, &temp);

		return;
	}

	// print message to file
	print_leave_message(ptr_car, fout);

	// car leaves
	pop_stack(park);

	// other cars come back
	while (!is_empty_stack(&temp))
	{
		ptr_car = top_stack(&temp);
		++ptr_car->times;
		top_move_stack(park, &temp);
	}
}

void all_car_leave(STACK *park, FILE *fout)
{
	const CAR *ptr_car;
	while (!is_empty_stack(park))
	{
		ptr_car = top_stack(park);

		// print message to file
		print_leave_message(ptr_car, fout);

		pop_stack(park);
		print_stack(park);
	}
}

void print_sorry_message(const CAR *car, FILE *file)
{
	static char msg[BUFF_SIZE];

	sprintf(msg, "Sorry %s, the lot is full\n", car->name);

	// print message to screen
	printf("print message to file : ");
	fprintf(stdout, msg);

	// print message to output file
	fprintf(file, msg);
}

void print_leave_message(const CAR *car, FILE *file)
{
	static char msg[BUFF_SIZE];

	sprintf(msg, "%s was moved %u times while it was here\n", car->name, car->times);

	// print message to screen
	printf("print message to file : ");
	fprintf(stdout, msg);

	// print message to output file
	fprintf(file, msg);
}

char *get_line(char *buff, int len, FILE *file)
{
	char *ret = fgets(buff, len, file);
	char *find;
	int ch;

	if (ret)
	{
		find = strchr(buff, '\n');

		if (find)
		{
			*find = '\0';
		}
		else
		{
			while (ch = fgetc(file), ch != '\n' && ch != EOF)
				continue;
		}
	}

	return ret;
}