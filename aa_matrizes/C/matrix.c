#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

const int cross = 64;

Matrix create(int n){
    int length = n*n;
    Matrix matrix = (Matrix) malloc(sizeof(int)*length);
    return matrix;
}
Matrix createRandom(int n){
    int length = n*n;
    Matrix matrix = (Matrix) malloc(sizeof(int)*length);
    if(matrix != NULL){
        for(int i = 0; i < length; i++){
            matrix[i] = rand()%10;
        }
    }
    return matrix;
}
void deleteMatrix(Matrix *matrix){
    free(*matrix);
    *matrix = NULL;
}
int reshape(Matrix *matrix, int nIn, int nOut){
    int length = nIn*nIn;
    int newLength = nOut*nOut;

    Matrix newData = (Matrix) malloc(newLength * sizeof(int));
    if(newData == NULL) return 0;

    Matrix aux = *matrix;
    int index = 0, indexnew = 0;
    for(int i = 0; i < nIn && i < nOut; i++){
        for(int j = 0; j < nIn && j < nOut; j++){
            newData[indexnew++] = aux[index++];
        }
        for(int j = nIn; j < nOut; j++){
            newData[indexnew++] = 0;
        }
    }
    while(indexnew < newLength){
        newData[indexnew++] = 0;
    }
    free(*matrix);
    *matrix = newData;
    return 1;
}

int addMatrix(Matrix a, Matrix b, Matrix out, int n){
    if(a == NULL || b == NULL || out == NULL ) return 0;

    int length = n * n;
    for(int i = 0; i < length; i++){
        out[i] = a[i] + b[i];
    }
    return 1;
}
int subMatrix(Matrix a, Matrix b, Matrix out, int n){
    if(a == NULL || b == NULL || out == NULL ) return 0;

    int length = n * n;
    for(int i = 0; i < length; i++){
        out[i] = a[i] - b[i];
    }
    return 1;
}

int compose(Matrix out, Matrix a, Matrix b, Matrix c, Matrix d, int N){
   if(out == NULL || a == NULL || b == NULL || c == NULL || d == NULL) return 0;

    int n = N/2;
    int index = 0;
    int iA = 0, iB = 0, iC = 0, iD = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            out[index++] = a[iA++];
        }
        for(int j = 0; j < n; j++){
            out[index++] = b[iB++];
        }
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            out[index++] = c[iC++];
        }
        for(int j = 0; j < n; j++){
            out[index++] = d[iD++];
        }
    }
    return 1;
}
int divide(Matrix in, Matrix a, Matrix b, Matrix c, Matrix d, int N){
    if(in == NULL || a == NULL || b == NULL || c == NULL || d == NULL) return 0;

    int n = N/2;
    int index = 0;
    int iA = 0, iB = 0, iC = 0, iD = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            a[iA++] = in[index++];
        }
        for(int j = 0; j < n; j++){
            b[iB++] = in[index++];
        }
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            c[iC++] = in[index++];
        }
        for(int j = 0; j < n; j++){
            d[iD++] = in[index++];
        }
    }
    return 1;
}

int multiplyClassic(Matrix a, Matrix b, Matrix out, int n){
    if(a == NULL || b == NULL || out == NULL) return 0;

    int indexOut = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int indexA = i*n;
            int indexB = j;
            int sum = 0;
            for(int k = 0; k < n; k++){
                sum += a[indexA]*b[indexB];
                indexA++;
                indexB += n;
            }
            out[indexOut++] = sum;
        }
    }
    return 1;
}
int multiplyStrassen(Matrix a, Matrix b, Matrix out, int n){
    if(a == NULL || b == NULL || out == NULL) return 0;
    if(n == 1){
        out[0] = a[0]*b[0];
        return 1;
    }
    /**
        Alteração para otimizar o algoritmo ultilizando a multiplicacao
        classica para matrizes pequuenas.
    **/
    /*if(n <= cross){
        return multiplyClassic(a,b,out,n);
    }*/

    int n_2 = n / 2;
    Matrix a11 = create(n_2), a12 = create(n_2), a21 = create(n_2), a22 = create(n_2);
    Matrix b11 = create(n_2), b12 = create(n_2), b21 = create(n_2), b22 = create(n_2);

    divide(a, a11, a12, a21, a22, n);
    divide(b, b11, b12, b21, b22, n);

    Matrix aux1 = create(n_2), aux2 = create(n_2);
	// Calculate P1
    Matrix P1 = create(n_2);
    subMatrix(b12, b22, aux1, n_2);
    multiplyStrassen(a11, aux1, P1, n_2);

    // Calculate P2
    Matrix P2 = create(n_2);
    addMatrix(a11, a12, aux1, n_2);
    multiplyStrassen(aux1, b22, P2, n_2);

    // Calculate P3
    Matrix P3 = create(n_2);
    addMatrix(a21, a22, aux1, n_2);
    multiplyStrassen(aux1, b11, P3, n_2);

    // Calculate P4
    Matrix P4 = create(n_2);
    subMatrix(b21, b11, aux1, n_2);
    multiplyStrassen(a22, aux1, P4, n_2);

    // Calculate P5
    Matrix P5 = create(n_2);
    addMatrix(a11, a22, aux1, n_2);
    addMatrix(b11, b22, aux2, n_2);
    multiplyStrassen(aux1, aux2, P5, n_2);

    // Calculate P6
    Matrix P6 = create(n_2);
    subMatrix(a12, a22, aux1, n_2);
    addMatrix(b21, b22, aux2, n_2);
    multiplyStrassen(aux1, aux2, P6, n_2);

    // Calculate P7
    Matrix P7 = create(n_2);
    subMatrix(a11, a21, aux1, n_2);
    addMatrix(b11, b12, aux2, n_2);
    multiplyStrassen(aux1, aux2, P7, n_2);

    deleteMatrix(&a11);deleteMatrix(&a12);deleteMatrix(&a21);deleteMatrix(&a22);
    deleteMatrix(&b11);deleteMatrix(&b12);deleteMatrix(&b21);deleteMatrix(&b22);

    Matrix c11 = create(n_2), c12 = create(n_2), c21 = create(n_2), c22 = create(n_2);
    //Calculate c11
    addMatrix(P5,P4, aux1, n_2);
    addMatrix(aux1, P6, aux2, n_2);
    subMatrix(aux2, P2, c11, n_2);

    //Calculate c12
    addMatrix(P1, P2, c12, n_2);

    //Calculate c21
	addMatrix(P3, P4, c21, n_2);

    //Calculate c22
    addMatrix(P1, P5, aux1, n_2);
    addMatrix(P3, P7, aux2, n_2);
    subMatrix(aux1, aux2, c22, n_2);

    deleteMatrix(&P1);deleteMatrix(&P2);deleteMatrix(&P3);
    deleteMatrix(&P4);deleteMatrix(&P5);deleteMatrix(&P6);
    deleteMatrix(&P7);deleteMatrix(&aux1);deleteMatrix(&aux2);

    compose(out, c11, c12, c21, c22, n);
    deleteMatrix(&c11);deleteMatrix(&c12);deleteMatrix(&c21);deleteMatrix(&c22);
    return 1;
}

void printMatrix(Matrix a, int n){
    printf("\n[");
    int index = 0;
    for(int i = 0; i < n; i++){
        printf("\n\t[ %d", a[index++]);
        for(int j = 1; j < n; j++){
            printf(", %d", a[index++]);
        }
        printf("]");
    }
    printf("\n]");
}
