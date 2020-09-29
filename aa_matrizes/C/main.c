#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "matrix.h"

int main(){
    int interations = 30;
    int N[10] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    FILE *output;
    output = fopen("testes.txt", "w");
    if(output == NULL){
        printf("Error ao abrir arquivo");
        return 0;
    }

    int sum;
    Matrix a, b, c;
    for(int i = 0; i < 10; i++){
        sum = 0;
        fprintf(output, "\nAlgoritmo Classico %dx%d\n",N[i],N[i]);
        printf("\nAlgoritmo Classico %dx%d\n",N[i],N[i]);
        for(int j = 0; j < interations; j++){
            a = createRandom(N[i]);
            b = createRandom(N[i]);
            c = createRandom(N[i]);

            int ini = (int) clock();
            multiplyClassic(a,b,c, N[i]);
            int fim = (int) clock();
            sum += fim - ini;
            fprintf(output,"%d\n", fim - ini);
            printf("%d\n", fim - ini);
            deleteMatrix(&a);
            deleteMatrix(&b);
            deleteMatrix(&c);
        }
        printf("Media: %f",(float) sum / interations);
        fprintf(output, "Media: %f",(float) sum / interations);

        sum = 0;
        fprintf(output, "\nAlgoritmo Strassen %dx%d\n",N[i],N[i]);
        printf("\nAlgoritmo Strassen %dx%d\n",N[i],N[i]);
        for(int j = 0; j < interations; j++){
            a = createRandom(N[i]);
            b = createRandom(N[i]);
            c = createRandom(N[i]);

            int ini = (int) clock();
            multiplyStrassen(a,b,c, N[i]);
            int fim = (int) clock();
            fprintf(output,"%d\n", fim - ini);
            printf("%d\n", fim - ini);
            sum += fim - ini;
            deleteMatrix(&a);
            deleteMatrix(&b);
            deleteMatrix(&c);
        }
        printf("Media: %f",(float) sum / interations);
        fprintf(output, "Media: %f",(float) sum / interations);
    }
    fclose(output);
    return 0;
}
