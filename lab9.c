#include <stdio.h>
#include <string.h>

typedef void(*print)(struct Matrix *);
typedef float(*computeSum)(struct Matrix *, struct Matrix *);
typedef float(*computeProduct)(struct Matrix *, struct Matrix *);
typedef void(*initializeVector)(struct Vector *, int rows); //constructor of Vector class which in turn calls constructor of matrix class with column = 1
typedef void(*computeL1Norm)(struct Matrix *);
typedef void(*initializeMatrix)(struct Matrix *, int rows, int columns);//constructor of matrix class


//-------Matrix Class-----------
typedef struct Matrix{
	int **array;
	computeL1Norm *vtable_ptr;
	int rows;
	int columns;
	computeProduct computeproduct;
	computeSum computesum;
	initializeMatrix init;
	print printthis;
}MN_Matrix;

//-----------function for printing matrix------------
void printMatrix(struct Matrix *A){
	int i , j;
	for (i = 0; i < A->rows ; i++)
	{
		for (j = 0; j < A->columns ; j++)
			printf("%d\t", A->array[i][j]);
		printf("\n");
	}
}

//-----------function for matrix L1Norm---------
void matrixL1Norm (struct Matrix *A){
    int c, d, k, sum = 0;
	int larg = 0;
    int i;
	int L1[20];

	if ((A->rows == 0) || (A->columns == 0))
		printf("Cant find L1Norm for Matrix\n");
	for (c = 0; c < A->columns; c++) {
		L1[c] = 0;
		for (d = 0; d < A->rows; d++) {
			L1[c] += A->array[d][c];
		}
	}

	for (i = 0; i < A->columns; i++){
		if (L1[i] > larg)
			larg = L1[i];
	}

	printf("Matrix L1Norm called: %d\n", larg);
    return;
}

//--------------function for matrix addition------------
float matrixAddtion(struct Matrix *A, struct Matrix *B){
	if ((A->rows != B->rows) && (A->columns != B->columns))
		printf("Matrices with specified orders can't be added.\n");
	int i, j;
	for (i = 0; i < A->rows; i++)
	{
		for (j = 0; j < A->columns; j++)
			printf("%d\t", A->array[i][j] + B->array[i][j]);
		printf("\n");
	}
}


//-------------function for matrix product-------------
float matrixProduct(struct Matrix *A, struct Matrix *B)
{
	int c, d, k, sum = 0;
	int multiply[20][20];

	if (A -> columns != B -> rows)
		printf("Matrices with specified orders can't be multiplied.\n");
	for (c = 0; c < A -> rows; c++) {
		for (d = 0; d < B -> columns; d++) {
			for (k = 0; k < B -> rows; k++) {
				sum = sum + (A->array[c][k]) * (B->array[k][d]);
			}

			multiply[c][d] = sum;
			sum = 0;
		}
	}

	for (c = 0; c < A->rows ; c++) {
		for (d = 0; d < B -> columns; d++)
			printf("%d\t", multiply[c][d]);

		printf("\n");
	}
}

//-----------definition of constructor for matrix class----------------
void initMatrix(struct Matrix *A, int rows, int columns){
	int i;
	MN_Matrix temp;
	temp.rows = rows;
	temp.computeproduct = matrixProduct;
	temp.printthis = printMatrix;
	temp.computesum = matrixAddtion;
	temp.columns = columns;
	temp.array = malloc(rows * sizeof(int *));

	for (i = 0; i < rows; i++)
	{
		temp.array[i] = malloc(columns * sizeof(int));
		if (temp.array[i] == NULL)
		{
			fprintf(stderr, "out of memory\n");
			return;
		}
	}
	for (i = 0; i < 3; i++)
	{
		int j;
		for (j = 0; j < 3; j++)
			temp.array[i][j] = 1;
	}

	*A = temp;
}

//------------Vector class which inherits Matrix class------------
typedef struct Vector{
	MN_Matrix vec;
	initializeVector init;
}N_Vector;

void vectorL1Norm (struct Matrix *A){
    int i , sum=0;
    for(i = 0 ; i< A->rows; i++){
        sum+=A->array[i][0];
    }
	printf("Vector L1Norm called: %d\n", sum);
    return;
}

//-------------definition of constructor for vector class-----------
void initVector(struct Vector *A, int rows){
	N_Vector nvec;
	nvec.vec.init = initMatrix;
	nvec.vec.init(&nvec.vec, rows, 1);
	*A = nvec;
}

int main() {
	computeL1Norm *vtable_matrix_class = malloc(sizeof(computeL1Norm));
    vtable_matrix_class[0] = &matrixL1Norm;

	computeL1Norm *vtable_vector_class = malloc(sizeof(computeL1Norm));
	vtable_vector_class[0] = &vectorL1Norm;

	MN_Matrix *m = (MN_Matrix*)malloc(sizeof(N_Vector));
	m->init = initMatrix;
	m->init (m, 3, 3);
	printf("\nMatrix:\n");
	m->printthis(m);
	printf("\nAddition:\n");
	m->computesum(m, m);
	printf("\nMultiplication:\n");
	m->computeproduct(m, m);
	m->vtable_ptr = vtable_matrix_class;
	printf("\nL1Norm:\n");
	m->vtable_ptr[0](m);
	
	

	

	return 0;
}
