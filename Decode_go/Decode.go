package main

import (
	"bufio"
	"compress/gzip"
	"fmt"
	"io/ioutil"
	"os"
	"strconv"
	"strings"
)

type DecodeTime_t struct {
	src    []int
	relnum []int
	relval []int
}

type DecodeData_t struct {
	src    []float64
	relnum []int
	relval []float64
}

func read() string {
	file, _ := os.Open("compressed")
	defer file.Close()
	gr, _ := gzip.NewReader(file)
	defer gr.Close()
	data, _ := ioutil.ReadAll(gr)
	return string(data)
}

func inInit(time *DecodeTime_t, data *DecodeData_t) {
	buf := read() + "\n"
	l := 0
	var s strings.Builder
	readint := func() int {
		for buf[l] != ' ' && buf[l] != '\n' {
			s.WriteString(string(buf[l]))
			l++
		}
		val, _ := strconv.Atoi(s.String())
		l++
		s.Reset()
		return val
	}
	readfloat64 := func() float64 {
		for buf[l] != ' ' && buf[l] != '\n' {
			s.WriteString(string(buf[l]))
			l++
		}
		val, _ := strconv.ParseFloat(s.String(), 32)
		l++
		s.Reset()
		return val
	}
	timemapnum := readint()
	timemap := make(map[int]int)
	for timerank := 0; timerank < timemapnum; timerank++ {
		timemap[timerank] = readint()
	}
	srcnum := timemap[readint()]
	flag1 := timemap[readint()]
	if flag1 != 0 {
		var sum int
		for sum != srcnum {
			val := timemap[readint()]
			time.relnum = append(time.relnum, val)
			sum += val
		}
		Len := len(time.relnum)
		for i := 0; i < Len; i++ {
			time.relval = append(time.relval, timemap[readint()])
		}
	} else {
		for i := 0; i < srcnum; i++ {
			time.src = append(time.src, timemap[readint()])
		}
	}
	datamapnum := readint()
	datamap := make(map[int]float64)
	for datarank := 0; datarank < datamapnum; datarank++ {
		datamap[datarank] = readfloat64()
	}
	flag2 := datamap[readint()]
	if flag2 != 0 {
		var sum int
		for sum != srcnum {
			val := datamap[readint()]
			data.relnum = append(data.relnum, int(val))
			sum += int(val)
		}
		Len := len(data.relnum)
		for i := 0; i < Len; i++ {
			data.relval = append(data.relval, datamap[readint()])
		}
	} else {
		for i := 0; i < srcnum; i++ {
			data.src = append(data.src, datamap[readint()])
		}
	}
}

func DecodeTime(time *DecodeTime_t) {
	if len(time.src) == 0 {
		Len := len(time.relnum)
		for i := 0; i < Len; i++ {
			for j := 0; j < time.relnum[i]; j++ {
				time.src = append(time.src, time.relval[i])
			}
		}
	}
	Len := len(time.src)
	for i := 1; i < Len; i++ {
		time.src[i] += time.src[i-1]
	}
}

func DecodeData(data *DecodeData_t) {
	if len(data.src) == 0 {
		Len := len(data.relnum)
		for i := 0; i < Len; i++ {
			for j := 0; j < data.relnum[i]; j++ {
				data.src = append(data.src, data.relval[i])
			}
		}
	}
}

func SDTDecode(time *[]int, data *[]float64) {
	outputFile, outputError := os.OpenFile("ans.txt", os.O_WRONLY|os.O_CREATE, 0666)
	if outputError != nil {
		fmt.Printf("An error occurred with file opening or creation\n")
		return
	}
	defer outputFile.Close()
	outputWriter := bufio.NewWriter(outputFile)
	Len := len(*time) - 1
	for i := 0; i < Len; i++ {
		y_delta := (*data)[i+1] - (*data)[i]
		x_delta := (*time)[i+1] - (*time)[i]
		var y float64
		for j := (*time)[i]; j < (*time)[i+1]; j++ {
			s := strconv.FormatFloat((*data)[i]+y/float64(x_delta), 'f', -1, 32)
			outputWriter.WriteString(s + "\n")
			y += y_delta
		}
	}
	s := strconv.FormatFloat((*data)[Len], 'f', -1, 32)
	outputWriter.WriteString(s + "\n")
	outputWriter.Flush()
}

func main() {
	var time DecodeTime_t
	var data DecodeData_t
	inInit(&time, &data)
	DecodeTime(&time)
	DecodeData(&data)
	SDTDecode(&time.src, &data.src)
}
