typedef int *Matrix;

Matrix create(int n);
Matrix createRandom(int n);
void deleteMatrix(Matrix *matrix);
int reshape(Matrix *matrix, int nIn, int nOut);

int addMatrix(Matrix a, Matrix b, Matrix out, int n);
int subMatrix(Matrix a, Matrix b, Matrix out, int n);

int divide(Matrix in, Matrix a, Matrix b, Matrix c, Matrix d, int N);
int compose(Matrix out, Matrix a, Matrix b, Matrix c, Matrix d, int N);

int multiplyClassic(Matrix a, Matrix b, Matrix out, int n);
int multiplyStrassen(Matrix a, Matrix b, Matrix out, int n);

void printMatrix(Matrix a, int n);
