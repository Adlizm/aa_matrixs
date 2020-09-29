package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	lengths := []int{2, 4, 8, 16, 32, 64, 128, 256}
	repeats := 30

	file_default, err_d := os.Create("default.txt")
	file_strassen, err_s := os.Create("strassen.txt")

	if err_d != nil {
		panic(err_d)
	}
	if err_s != nil {
		panic(err_s)
	}

	defer file_default.Close()
	defer file_strassen.Close()

	for _, N := range lengths {
		var totalTimeDefault int64 = 0
		var totalTimeStrassen int64 = 0

		for i := 0; i < repeats; i++ {
			A := Random(N, N)
			B := Random(N, N)

			init := time.Now()
			MultiplyMatrixsStrassen(A, B)
			end := time.Now()
			duration := end.Sub(init)

			totalTimeStrassen += duration.Milliseconds()
			file_strassen.Write([]byte(fmt.Sprint(N, duration, "\n")))

			init = time.Now()
			MultiplyMatrixsDefault(A, B)
			end = time.Now()
			duration = end.Sub(init)

			totalTimeDefault += duration.Milliseconds()
			file_default.Write([]byte(fmt.Sprint(N, duration, "\n")))
		}

		file_strassen.Write([]byte(
			fmt.Sprintf(
				"\n%d milliseconds to finsh %d matrixs muliplications \nMean: %d milliseconds\n\n",
				totalTimeStrassen, repeats, totalTimeStrassen/int64(repeats))))
		file_default.Write([]byte(
			fmt.Sprintf(
				"\n%d milliseconds to finsh %d matrixs muliplications \nMean: %d milliseconds\n\n",
				totalTimeDefault, repeats, totalTimeDefault/int64(repeats))))
	}
}
