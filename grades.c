
#include "linked-list.h"
#include "grades.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct course_elem* create_course(int grade, char *name);
struct student_elem* create_student(const char *name, int id);



struct grades
{
	struct list *students;
};

struct student_elem
{
	int student_id;
	char *student_name;
	struct list *courses;

};

void elem_destroy_student(void *element){
	struct student_elem *student = (struct student_elem*)element;
	list_destroy(student->courses);
	free(student->student_name);
	free(student);
}

int elem_clone_student(void *element, void **output){  
	struct student_elem *new_student = (struct student_elem*)element;
	struct student_elem **output_new = (struct student_elem**)(output);
	*output_new = (struct student_elem*)malloc(sizeof(struct student_elem));
	if(output_new == NULL) {
		free(output_new);
		return -1;
	}	
	(*output_new)->student_id = new_student->student_id;
	char *temp_name = (char*)malloc(sizeof(char)*(strlen(new_student->student_name)));
    strcpy(temp_name, new_student->student_name);
	(*output_new)->student_name = temp_name;
	(*output_new)->courses = new_student->courses;
	free(new_student);
	return 0;
}

struct course_elem
{
	int course_grade;
	char *course_name;
};

void elem_destroy_course(void *element){
	struct course_elem *course = (struct course_elem*)element;
	free(course->course_name);
	free(course);
}

int elem_clone_course(void *element, void **output){ 
	struct course_elem *new_course = (struct course_elem*)element;
	struct course_elem **output_new = (struct course_elem**)(output);
	*output_new = (struct course_elem*)malloc(sizeof(struct course_elem));
	(*output_new)->course_grade = new_course->course_grade;
	char *temp_name = (char*)malloc(sizeof(char)*(strlen(new_course->course_name)));
    strcpy(temp_name, new_course->course_name);
	(*output_new)->course_name = temp_name;
	free(new_course);
	if(output_new != NULL){
		return 0;
	}
	free(output_new);
	return -1;	
}

struct course_elem* search_course(struct student_elem *student,  const char *name){
	if(!(student->courses) || !name){
		return 	NULL;
	} 
	struct node *curser;
	for (curser=list_begin(student->courses); curser; curser = list_next(curser)){
			struct course_elem *current = (struct course_elem*)list_get(curser);
			if( 0 == strcmp(name,current->course_name) ){
					return current;
			}

	}
	return NULL;
}

struct student_elem* students_search(struct grades *grades, int id){
	if(!(grades->students) || !grades){
		return 	NULL;
	} 
	struct node *curser = list_begin(grades->students);
	size_t len=list_size(grades->students);
	for (size_t i=0; i<len; i++){
		struct student_elem *current = (struct student_elem* )list_get(curser);
		if(current->student_id == id){
			return current;
		}
		curser = list_next(curser);

	}
	return NULL;
}

struct grades* grades_init(){
	struct grades* new_grades = (struct grades*)malloc(sizeof(struct grades));
	new_grades->students = list_init(elem_clone_student,elem_destroy_student);
	return new_grades;
}

void grades_destroy(struct grades *grades){
	list_destroy(grades->students);
	free(grades);
}

struct student_elem* create_student(const char *name, int id){
	struct student_elem *new_student;
	new_student = (struct student_elem*)malloc(sizeof(struct student_elem));
	if(!new_student){
		return NULL;
	}
	new_student->student_id = id;
	new_student->student_name = (char *)name;
	new_student->courses = list_init(elem_clone_course, elem_destroy_course);
	return new_student;	
}

struct course_elem* create_course(int grade, char *name){
	struct course_elem *new_course;
	new_course = (struct course_elem*)malloc(sizeof(new_course));
	if(!new_course){
		return NULL;
	}
	new_course->course_grade = grade;
	new_course->course_name = name;
	return new_course;
}

int grades_add_student(struct grades *grades, const char *name, int id){
	if(students_search(grades,id)){
		return 1;
	} 
	struct student_elem *new_student = create_student(name,id);
	list_push_back(grades->students, new_student);
	return 0;
}

int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){
	if (grades==NULL){
		return -1;
	}
	struct student_elem *student_to_grade = students_search(grades, id);
	if(student_to_grade==NULL || grade<0 || grade>100 || search_course(student_to_grade, name)){
		return -1;
	}
	struct course_elem *new_course = create_course(grade,(char*)name);
	list_push_back(student_to_grade->courses, new_course);
	return 0;
}

float grades_calc_avg(struct grades *grades, int id, char **out){
	if (grades==NULL){
		return -1;
	}
	struct student_elem *student_to_calc = students_search(grades, id);
	if(student_to_calc == NULL){
		return -1;
	}
	struct node *curser;
	float sum = 0;
	float devidor = 0;
	for (curser=list_begin(student_to_calc->courses); curser; curser=list_next(curser)){
			struct course_elem *current = (struct course_elem*)list_get(curser);
			sum += current->course_grade;
			devidor++;
	}
	char *tmp_name = (char*)malloc(sizeof(char)*(strlen(student_to_calc->student_name)));
    strcpy(tmp_name,student_to_calc->student_name);
	(*out) = tmp_name;
	return devidor==0 ? 0 : sum/devidor;
}

int grades_print_student(struct grades *grades, int id){
	if (grades==NULL){
		return -1;
	}
	struct student_elem *student_to_print = students_search(grades, id);
	if(student_to_print == NULL){
		return -1;
	}
	struct node *curser;
	printf("%s %d:",student_to_print->student_name,student_to_print->student_id);
	for (curser=list_begin(student_to_print->courses); curser; curser=list_next(curser)){
			struct course_elem *current = (struct course_elem*)list_get(curser);
			printf(" %s %d",current->course_name,current->course_grade);
			if(list_next(curser)){
				printf("," );
			}
	}
	printf("\n");
	return 0;
}

int grades_print_all(struct grades *grades){
	if (grades==NULL){
		return -1;
	}
	struct node *curser;
	for (curser=list_begin(grades->students); curser; curser=list_next(curser)){
			struct student_elem *current = (struct student_elem*)list_get(curser);
			grades_print_student(grades, current->student_id);
	}
	return 0;
}
