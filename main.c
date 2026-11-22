#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// going back 

int go_back()
{
	if (getchar() == 'b' && getchar() == '\n') return 1;
	return 0;
}

// Handling errors

int is_digit(int res, int value)
{
	if (!res) 
	{ 
		while (getchar() != '\n');
		printf("\n!!!Please, Enter only integer numbers\n\n"); 
		return 0; 
	}
	if (value < 0)
	{
		printf("\n!!!Please, Enter positive numbers\n\n");
		return 0;
	}
	return 1;
}

int error_handle(void *value, char error[20])
{
	if (strcmp(error, "std_count") == 0) // handling error for students count
	{
		int *std_count = (int*)value;

		if (*std_count < 1 || *std_count > 40)
		{
			printf("\n!!!Error: number of Students must be between(1, 40)\n\n");
			return 0;
		}
	}
	else if (strcmp(error, "crs_count") == 0) // handling error for courses' count
	{
		int* crs_count = (int*)value;

		if (*crs_count < 1 || *crs_count > 9)
		{
			printf("\n!!!Error: number of Courses must be between(1, 9)\n\n");
			return 0;
		}
	}
	else if (strcmp(error, "crs_scale") == 0) // handling error for course's number
	{
		int* crs_scale = (int*)value;
		if (*crs_scale < 0)
		{
			printf("\n!!!Error: There is no Course with that number\n\n");
			return 0;
		}
	}
	else if (strcmp(error, "mark_scale") == 0) // handling error for mark scale
	{
		int* mark_scale = (int*)value;

		if (*mark_scale < 0 || *mark_scale > 100)
		{
			printf("\n!!!Error: Scale of mark must be between(0, 100)\n\n");
			return 0;
		}
	}
	else if (strcmp(error, "id_not_found") == 0) // handling error for invalid id
	{
		int* val = (int*)value;
		if (*val == -1)
		{
			printf("\n!!!Error: There is no Student with this id\n\n");
			return 0;
		}
	}
	else if (strcmp(error, "yes/no") == 0) // handling error for invalid id
	{
		int *val = (int*)value;

		if (*val > 1)
		{
			printf("\n!!!Error: Please enter 1 or 0\n\n");
			return 0;
		}
	}
	else if (strcmp(error, "memory") == 0) // handling error for memory allocation
	{
		int* memory = (int*)value;

		if (memory == NULL)
		{
			printf("\n!!!Error occured during memory allocation, which is not related to you.\nError will be fixed if next is showed, if error shows up continuesly, please, exit terminal\n\n");
			return 0;
		}
	}
	else if (strcmp(error, "total_mark") == 0) // checking whether total_mark is equal to sum of entered course marks
	{
		int* total_mark = (int*)value;

		if (*total_mark)
		{
			printf("\n!!!Error: Sum of entered course marks didnt match for the total mark entered for that student above.\n\n");
			return 0;
		}
	}
	else // handling error whether the entered id is duplicate
	{
		int cur_n1 = (int)*error;
		int *cur_id = (int*)value;
		int *p = cur_id - 1;
		
		for (p; p >= cur_id - cur_n1; p--)
		{
			if (*p == *cur_id)
			{
				printf("\n!!!Error: There is already the Student with that ID\n\n");
				return 0;
			}
		}
	}
	
	return 1;
}

// Home Menu definition

void show_menu(int *command)
{
	printf("\n                         <<Menu>>");
	printf("\n ----------------------------------------------------------------");
	printf("\n| 1.Add Student        2.Add Course          3.Edit Mark         |");
	printf("\n| 4.Display Student    5.Display Records     6.Display Report    |");
	printf("\n| 7.Delete Student     0.Exit                                    |");
	printf("\n ----------------------------------------------------------------");

	printf("\nEnter appropriate menu number(0-7): ");
	int res = scanf("%d", command);
	getchar();

	if (!is_digit(res, *command)) *command = 8;
}

// Finding student position in the array

int search_student(int *ids, int n1, int id)
{
	int id_pos = -1;
	for (int* p = ids; p < ids + n1; p++)
	{
		if (*p == id) { id_pos = p - ids; break; }
	}

	return id_pos;
}

// Adding new student

int add_student(int **ids_addr, int *n1, int ***course_marks_addr, int n2, int **total_marks_addr)
{
memory1:
	int *ids = (int*)realloc(*ids_addr, ((*n1)+1) * sizeof(int));
memory2:
	int **course_marks = (int**)realloc(*course_marks_addr, ((*n1) + 1) * sizeof(int*));
memory3:
	int *total_marks = (int*)realloc(*total_marks_addr, ((*n1) + 1) * sizeof(int));

	if (!error_handle(ids, "memory")) goto memory1;
	if (!error_handle(course_marks, "memory")) goto memory2;
	if (!error_handle(total_marks, "memory")) goto memory3;
input1:
	printf("\nEnter new Student's ID: ");
	int res = scanf("%d", ids + *n1);

	if (go_back()) return 8;
	if (!is_digit(res, *(ids + *n1))) goto input1;
	int cur_n1 = *n1;
	if (!error_handle(ids + *n1, &(char)cur_n1)) goto input1;

	printf("\nEnter %d course marks for Student ID >> #%d\n", n2, *(ids + *n1));
	int sum = 0;
	*(course_marks + *n1) = (int*)calloc(n2, (sizeof(int)));
	for (int* p = *(course_marks + *n1); p < *(course_marks + *n1) + n2; p++)
	{
input2:
		printf("Course %d: ", p - *(course_marks + *n1) + 1);
		res = scanf("%d", p);

		if (go_back()) return 8;
		if (!is_digit(res, *p)) goto input2;
		if (!error_handle(p, "mark_scale")) goto input2;
		sum += *p;
	}

	*(total_marks + *n1) = sum;

	*ids_addr = ids;
	*course_marks_addr = course_marks;
	*total_marks_addr = total_marks;

	printf("\nNew Student Successfully added!");
	(*n1)++;

	return 1;
}

// Adding new Course

void add_course(int **course_marks, int *n2, int n1)
{
	int** p_mark;
	
	for (p_mark = course_marks; p_mark < course_marks + n1; p_mark++)
	{
	memory1:
		int *tmp_p_mark = (int*)realloc(*p_mark, (*n2 + 1) * sizeof(int));
		if (!error_handle(tmp_p_mark, "memory")) goto memory1;
		*p_mark = tmp_p_mark;
		*((*p_mark) + *n2) = 0;
	}
	printf("\nNew Course Successfully added!");
	printf("\nNow Each Student has a new course with initial of 0");
	printf("\nYou can update marks by option of 'Edit Mark' from menu");
	(*n2)++;
}

// Updating Student's existed course marks

int edit_mark(int* ids, int** course_marks, int* total_marks, int n1, int n2)
{
	int id, c_num, mark;
	printf("\n<Enter Student ID and Course number that you want to edit>\n");
input1:
	printf("\nStudent ID: ");
	int res = scanf("%d", &id);

	if (go_back()) return 8;
	if (!is_digit(res, id)) goto input1;

	int id_pos = search_student(ids, n1, id);
	if (!error_handle(&id_pos, "id_not_found")) goto input1;
input2:	
	printf("Course number: ");
	res = scanf("%d", &c_num);

	if (go_back()) return 8;
	if (!is_digit(res, c_num)) goto input2;
	
	int crs_scale = n2 - c_num;
	if (!error_handle(&crs_scale, "crs_scale")) goto input2;

	int* current_course = *(course_marks + id_pos) + c_num - 1;
	int current_mark = *current_course;
input3:
	printf("Current mark: %d", current_mark);
	printf("\nNew mark: ");
	res = scanf("%d", current_course);

	if (go_back()) return 8;
	if (!is_digit(res, *current_course)) goto input3;
	if (!error_handle(current_course, "mark_scale")) goto input3;

	*(total_marks + id_pos) += -current_mark + *current_course;
	printf("\nSuccessfully edited!");

	return 1;
}

// Removing Student from class

int delete_student(int *ids, int **course_marks, int *total_marks, int *n1)
{
	int id, del;
	printf("\n<Enter ID of the Student you want to delete>");
input1:
	printf("\nStudent ID: ");
	int res = scanf("%d", &id);

	if (go_back()) return 8;
	if (!is_digit(res, id)) goto input1;

	int id_pos = search_student(ids, *n1, id);
	if (!error_handle(&id_pos, "id_not_found")) goto input1;
input2:
	printf("\nAre you sure to delete Student ID >> %d\nYes/No (1/0): ", id);
	res = scanf("%d", &del);

	if (go_back()) return 8;
	if (!is_digit(res, del)) goto input2;
	if (!error_handle(&del, "yes/no")) goto input2;

	if (del)
	{
		int* p_id = ids + id_pos, ** p_mark = course_marks + id_pos, * p_total = total_marks + id_pos;
		free(*(course_marks + id_pos));
		for (p_id; p_id < ids + (*n1) - 1; p_id++, p_mark++, p_total++)
		{
			*p_id = *(p_id + 1);
			*p_mark = *(p_mark + 1);
			*p_total = *(p_total + 1);
		}
		(*n1)--;

		int new_size = *n1;
		if (new_size == 0) new_size = 1;

		int* tmp_ids, ** tmp_course_marks, * tmp_total_marks;
memory1:
		tmp_ids = (int*)realloc(ids, new_size * sizeof(int));
		if (!error_handle(tmp_ids, "memory")) goto memory1;
		ids = tmp_ids;
memory2:
		tmp_course_marks = (int**)realloc(course_marks, new_size * sizeof(int*));
		if (!error_handle(tmp_course_marks, "memory")) goto memory2;
		course_marks = tmp_course_marks;
memory3:
		tmp_total_marks = (int*)realloc(total_marks, new_size * sizeof(int));
		if (!error_handle(tmp_total_marks, "memory")) goto memory3;
		total_marks = tmp_total_marks;

		
		printf("\nStudent deleted!");
	}
	else printf("\nDeletion cancelled!");

	return 1;
}

// Displaying specific student's info

int display_student(int *ids, int **course_marks, int *total_marks, int n1, int n2)
{
	int id, id_pos;
	printf("\n<Enter ID of the Student you want to display>");
input1:
	printf("\nStudent ID: ");
	int res = scanf("%d", &id);

	if (go_back()) return 8;
	if (!is_digit(res, id)) goto input1;

	id_pos = search_student(ids, n1, id);
	if (!error_handle(&id_pos, "id_not_found")) goto input1;

	int* courses = *(course_marks + id_pos), *p;
	
	printf("\n-------------Student #%d Info-------------", id);
	printf("\nCourses  |  Marks  |  Total Mark  | Average Mark");
	printf("\n------------------------------------------------");
	for (p = courses; p < courses + n2; p++)
	{
		if (p - courses == 0)
		{
			printf("\nCourse-%d |  %-6d |  %-11d | %.2f", p-courses+1, *p, *(total_marks + id_pos), (float)*(total_marks + id_pos) / n2);
		}
		else
		{
			printf("\nCourse-%d |  %-6d |  %-11s |", p - courses + 1, *p, " ");
		}
	}
	printf("\n------------------------------------------------");

	return 1;
}

// Displaying whole records of all students 

void display_records(int *ids, int **course_marks, int *total_marks, int n1, int n2)
{
	int *p_id = ids;
	int **p_course_mark = course_marks;
	int *p_total_mark = total_marks;

	printf("\n------------------------All Records----------------------------");
	printf("\nStudent ID  |  Course-wise Marks  |  Total Mark  | Average Mark");
	printf("\n---------------------------------------------------------------");
	for (p_id; p_id < ids + n1; p_id++, p_course_mark++, p_total_mark++)
	{
		int *mark = *p_course_mark;
		for (mark; mark < *(p_course_mark) + n2; mark++)
		{
			if (mark - (*p_course_mark) == 0)
			{
				printf("\n#%-10d |  %-18d |  %-11d | %.2f", *p_id, *mark, *p_total_mark, (float)*p_total_mark / n2);
			}
			else
			{
				printf("\n%-11s |  %-18d |  %-11s |", " ", *mark, " ");
			}
		}

		printf("\n---------------------------------------------------------------");
	}
}

// Displaying report for class

void display_report(int *ids, int **course_marks, int *total_marks, int n1, int n2)
{
	int h_total = 0, h_total_id;
	int l_total = *total_marks, l_total_id = *ids;
	int total_marks_sum = 0, passed_stds_cnt = 0;

	for (int i = 0; i < n1; i++)
	{
		if (total_marks[i] > h_total)
		{
			h_total = total_marks[i];
			h_total_id = ids[i];
		}

		if (total_marks[i] < l_total)
		{
			l_total = total_marks[i];
			l_total_id = ids[i];
		}
		
		int *courses = course_marks[i];
		int is_passed = 1;
		for (courses; courses < course_marks[i] + n2; courses++)
		{
			if (*courses < 50) { is_passed = 0; break; }
		}

		if (is_passed) passed_stds_cnt++;

		total_marks_sum += total_marks[i];
	}

	printf("\n<<Class Report>>\n");

	printf("\nThe student with highest total: \n\tID: #%d, Total Mark: %d", h_total_id, h_total);
	printf("\nThe student with lowest total: \n\tID: #%d, Total Mark: %d", l_total_id, l_total);
	printf("\n\nAverage mark of class: %.2f", (float)total_marks_sum / n2);
	printf("\nNumber of students passed for all courses(pass criteria is, mark>[50]): %d", passed_stds_cnt);
}

// Releasing allocated memories

void release_memory(int **ids, int ***course_marks, int **total_marks, int *n1, int *n2)
{
	free(*ids);
	for (int** p = *course_marks; p < (*course_marks) + (*n1); p++) free(*p);
	free(*course_marks);
	free(*total_marks);
	*n1 = 0;
	*n2 = 0;

	*ids = NULL;
	*course_marks = NULL;
	*total_marks = NULL;
}

// main function

int main()
{

input1:
	int n1, n2; // n1 - number of students in the class, n2 - number of courses of each student(note: every student has the number of courses)
	int res; // that holds whether scanf was successfully scanned input or not
	printf("Enter initial number of students: ");
	res = scanf("%d", &n1); // getting students' count

	if (!is_digit(res, n1)) goto input1;
	if (!error_handle(&n1, "std_count")) goto input1;
memory1:
	int *p_id, *ids = (int *)malloc(n1 * sizeof(int)); // allocating memory for students' ids
	if (!error_handle(ids, "memory")) goto memory1;
memory2:
	int *p_total_mark, *total_marks = (int *)malloc(n1 * sizeof(int)); // allocating memory for total marks of courses of students
	if (!error_handle(total_marks, "memory")) goto memory2;

	// getting id and total mark for each student
	p_total_mark = total_marks;
	for (p_id = ids; p_id < ids + n1; p_id++, p_total_mark++) 
	{
input2:
		printf("Student-%d ID: ", p_id - ids + 1);
		res = scanf("%d", p_id);

		if (!is_digit(res, *p_id)) goto input2; 
		int cur_n1 = p_id - ids;
		if (!error_handle(p_id, &(char)cur_n1)) goto input2;
input3:
		printf("Total Mark: "); 
		res = scanf("%d", p_total_mark);

		if (!is_digit(res, *p_total_mark)) goto input3;
	}

input4:
	int **p_course_mark, **course_marks = (int **)malloc(n1 * sizeof(int*)); // allocating memory for each student's marks for their each course
	printf("\nEnter initial number of courses: ");
	res = scanf("%d", &n2);

	if (!is_digit(res, n2)) goto input4;
	if (!error_handle(&n2, "crs_count")) goto input4;

	// getting marks for each courses of each student
	p_id = ids;
	p_total_mark = total_marks;
	for (p_course_mark = course_marks; p_course_mark < course_marks + n1; p_course_mark++, p_id++, p_total_mark++)
	{
		*p_course_mark = (int *)calloc(n2, sizeof(int));

		printf("\nEnter %d course marks for Student ID >> #%d\n", n2, *p_id);
input5:
		int total_mark = *p_total_mark;
		for (int *p_mark = *p_course_mark; p_mark < (*p_course_mark) + n2; p_mark++)
		{
input6:
			printf("Course %d: ", p_mark - (*p_course_mark) + 1);
			res = scanf("%d", p_mark);

			if (!is_digit(res, *p_mark)) goto input6;
			if (!error_handle(p_mark, "mark_scale")) goto input6;
			total_mark -= *p_mark;
		}

		if (!error_handle(&total_mark, "total_mark")) goto input5;
	}

	display_records(ids, course_marks, total_marks, n1, n2);

	// navigating to different sections of home menu

	int command = 1;
	while (command)
	{
		if (command != 8) while (getchar() != '\n');

		show_menu(&command);
		
		switch (command)
		{
		case 1:
			command = add_student(&ids, &n1, &course_marks, n2, &total_marks);
			break;
		case 2:
			add_course(course_marks, &n2, n1);
			break;
		case 3:
			command = edit_mark(ids, course_marks, total_marks, n1, n2);
			break;
		case 4:
			command = display_student(ids, course_marks, total_marks, n1, n2);
			break;
		case 5:
			display_records(ids, course_marks, total_marks, n1, n2);
			break;
		case 6:
			display_report(ids, course_marks, total_marks, n1, n2);
			break;
		case 7:
			command = delete_student(ids, course_marks, total_marks, &n1);
			break;
		case 0:
			release_memory(&ids, &course_marks, &total_marks, &n1, &n2);
			printf("\nProgram finished!");
			return 0;
		default:
			printf("\n!!!Please enter correct number(0-7)!\n");
		}
	}
}