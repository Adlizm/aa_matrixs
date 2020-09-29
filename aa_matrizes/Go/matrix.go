package main

import (
	"math/rand"
)

type Matrix struct {
	rows, cols int
	data       [][]int
}

// Retorna se a matriz é quadrada
func (mat *Matrix) IsSquare() bool {
	if mat.rows == mat.cols {
		return true
	}
	return false
}

// Cria matrix An,m com números aleatórios
func Random(rows, cols int) *Matrix {
	if rows <= 0 {
		rows = 1
	}
	if cols <= 0 {
		cols = 1
	}

	data := make([][]int, rows)
	for i := 0; i < rows; i++ {
		data[i] = make([]int, cols)
		for j := 0; j < cols; j++ {
			data[i][j] = rand.Int() % 10
		}
	}
	matrix := Matrix{rows, cols, data}
	return &matrix
}

// Retorna uma matriz com a soma de outras duas
func Add(mat, other *Matrix) *Matrix {
	if mat.rows != other.rows || mat.cols != other.cols {
		return nil
	}

	data := make([][]int, mat.rows)
	for i := 0; i < mat.rows; i++ {
		data[i] = make([]int, mat.cols)
		for j := 0; j < mat.cols; j++ {
			data[i][j] = mat.data[i][j] + other.data[i][j]
		}
	}
	return &Matrix{mat.rows, mat.cols, data}
}

// Retorna uma matriz com a subtração de uma por outra
func Sub(mat, other *Matrix) *Matrix {
	if mat.rows != other.rows || mat.cols != other.cols {
		return nil
	}

	data := make([][]int, mat.rows)
	for i := 0; i < mat.rows; i++ {
		data[i] = make([]int, mat.cols)
		for j := 0; j < mat.cols; j++ {
			data[i][j] = mat.data[i][j] - other.data[i][j]
		}
	}
	return &Matrix{mat.rows, mat.cols, data}
}

// Algortmo Padrão de Multiplicação de Matrizes O(N^3)
func MultiplyMatrixsDefault(mat, other *Matrix) *Matrix {
	if mat.cols != other.rows {
		return nil
	}
	data := make([][]int, mat.rows)
	for i := 0; i < mat.rows; i++ {
		data[i] = make([]int, other.cols)
		for j := 0; j < other.cols; j++ {
			var sum int
			for k := 0; k < mat.cols; k++ {
				sum += mat.data[i][k] * other.data[k][j]
			}
			data[i][j] = sum
		}
	}

	return &Matrix{mat.rows, other.cols, data}
}

// Algorítmo de Multiplicação de Matrizes de Straseen O(N^Log2(7))
// Matrizes devem ser de dimenção 2^n x 2^n , n >= 0
func MultiplyMatrixsStrassen(mat, other *Matrix) *Matrix {
	if !mat.IsSquare() || !other.IsSquare() {
		return nil
	}
	if mat.rows == 1 {
		value := mat.data[0][0] * other.data[0][0]
		return &Matrix{1, 1, [][]int{{value}}}
	}

	A, B, C, D := mat.SplitMatrix()
	E, F, G, H := other.SplitMatrix()

	P1 := MultiplyMatrixsStrassen(A, Sub(F, H))
	P2 := MultiplyMatrixsStrassen(Add(A, B), H)
	P3 := MultiplyMatrixsStrassen(Add(C, D), E)
	P4 := MultiplyMatrixsStrassen(D, Sub(G, E))
	P5 := MultiplyMatrixsStrassen(Add(A, D), Add(E, H))
	P6 := MultiplyMatrixsStrassen(Sub(B, D), Add(G, H))
	P7 := MultiplyMatrixsStrassen(Sub(A, C), Add(E, F))

	result := UnionMatrixs(
		Sub(Add(Add(P5, P4), P6), P2), Add(P1, P2),
		Add(P3, P4), Sub(Add(P1, P5), Add(P3, P7)))

	return result
}

// Divide uma Matriz em 4 Matrizes
func (mat *Matrix) SplitMatrix() (A, B, C, D *Matrix) {
	rowsTop := int(mat.rows / 2)
	rowsBottom := mat.rows - rowsTop

	colsLeft := int(mat.cols / 2)
	colsRight := mat.cols - colsLeft

	dataA := make([][]int, rowsTop)
	dataB := make([][]int, rowsTop)
	for i := 0; i < rowsTop; i++ {
		dataA[i] = mat.data[i][:colsLeft]
		dataB[i] = mat.data[i][colsLeft:]
	}
	dataC := make([][]int, rowsBottom)
	dataD := make([][]int, rowsBottom)
	for i := 0; i < rowsBottom; i++ {
		dataC[i] = mat.data[i+rowsTop][:colsLeft]
		dataD[i] = mat.data[i+rowsTop][colsLeft:]
	}

	A = &Matrix{rowsTop, colsLeft, dataA}
	B = &Matrix{rowsTop, colsRight, dataB}
	C = &Matrix{rowsBottom, colsLeft, dataC}
	D = &Matrix{rowsBottom, colsRight, dataD}

	return A, B, C, D
}

// Une 4 matrizes em uma matriz maior
func UnionMatrixs(A, B, C, D *Matrix) *Matrix {
	if A.rows != B.rows || C.rows != D.rows || C.cols != A.cols || D.cols != B.cols {
		return nil
	}

	unionRows := A.rows + C.rows
	unionCols := A.cols + B.cols
	data := make([][]int, unionRows)
	for i := 0; i < A.rows; i++ {
		data[i] = make([]int, unionCols)
		for j := 0; j < A.cols; j++ {
			data[i][j] = A.data[i][j]
		}
		for j := 0; j < B.cols; j++ {
			data[i][j+A.cols] = B.data[i][j]
		}
	}
	for i := 0; i < C.rows; i++ {
		data[i+A.rows] = make([]int, unionCols)
		for j := 0; j < C.cols; j++ {
			data[i+A.rows][j] = C.data[i][j]
		}
		for j := 0; j < D.cols; j++ {
			data[i+A.rows][j+C.cols] = D.data[i][j]
		}
	}

	return &Matrix{unionRows, unionCols, data}
}
