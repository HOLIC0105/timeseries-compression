package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
	"sort"
	"strconv"
)

const MAX_FLOAT32 = 3.4028231e38
const eps = 1e-7

var e_delta float32 //绝对误差的范围

type EncodeTime_t struct {
	src    []int
	relnum []int
	relval []int
}

type EncodeData_t struct {
	src    []float32
	relnum []int
	relval []float32
}

type Anstime_t struct {
	num []int
	val []int
}

type Ansdata_t struct {
	num []int
	val []float32
}

type TimeSortArray_t struct {
	val int
	num int
}
type TimeSortArraySlince []TimeSortArray_t

func (p TimeSortArraySlince) Len() int {
	return len(p)
}
func (p TimeSortArraySlince) Less(i, j int) bool {
	return p[i].num > p[j].num
}
func (p TimeSortArraySlince) Swap(i, j int) {
	p[i], p[j] = p[j], p[i]
}

type DataSortArray_t struct {
	val float32
	num int
}
type DataSortArraySlince []DataSortArray_t

func (p DataSortArraySlince) Len() int {
	return len(p)
}
func (p DataSortArraySlince) Less(i, j int) bool {
	return p[i].num > p[j].num
}
func (p DataSortArraySlince) Swap(i, j int) {
	p[i], p[j] = p[j], p[i]
}

func Sdtdoor(in *[]float32, x *[]int, y *[]float32) {
	delta := e_delta
	var up_gate float32 = -MAX_FLOAT32
	var down_gate float32 = MAX_FLOAT32
	var end int = int(len(*in))
	var tim int = 0
	*x = append(*x, tim)
	*y = append(*y, (*in)[tim])
	tim++
	for {
		if tim == end {
			break
		}
		x_delta := tim - (*x)[len(*x)-1]
		y_delta := (*in)[tim] - (*y)[len(*y)-1]
		up_gate = max(up_gate, (y_delta-delta)/float32(x_delta))
		down_gate = min(down_gate, (y_delta+delta)/float32(x_delta))
		if up_gate > down_gate+eps {
			*x = append(*x, tim-1)
			*y = append(*y, (*in)[tim-1])
			y_delta = (*in)[tim] - (*in)[tim-1]
			up_gate = (y_delta - delta)
			down_gate = (y_delta + delta)
		}
		tim++
	}
	*x = append(*x, tim-1)
	*y = append(*y, (*in)[tim-1])
}

func EncodeTime(data *EncodeTime_t) {
	len := len(data.src)
	for i := len - 1; i > 0; i-- {
		data.src[i] = data.src[i] - data.src[i-1]
	}
	var lst, num int = data.src[0], 0
	for _, u := range data.src {
		if u == lst {
			num++
		} else {
			data.relnum = append(data.relnum, num)
			data.relval = append(data.relval, lst)
			lst, num = u, 1
		}
	}
	data.relnum = append(data.relnum, num)
	data.relval = append(data.relval, lst)
}

func EncodeData(data *EncodeData_t) {
	var lst float32 = data.src[0]
	var num int = 0
	for _, u := range data.src {
		if u == lst {
			num++
		} else {
			data.relnum = append(data.relnum, num)
			data.relval = append(data.relval, lst)
			lst, num = u, 1
		}
	}
	data.relnum = append(data.relnum, num)
	data.relval = append(data.relval, lst)
}

func read(filename *string) string {
	f, err := os.Open(*filename)
	if err != nil {
		fmt.Println("read file fail", err)
		return ""
	}
	defer f.Close()
	fd, err := ioutil.ReadAll(f)
	if err != nil {
		fmt.Println("read to fd fail", err)
		return ""
	}
	return string(fd)
}

func inInit(filename *string, in *[]float32) {
	buf := read(filename)
	Len := len(buf)
	var s string
	for i := 0; i < Len; i++ {
		if buf[i] == ' ' || buf[i] == '\n' {
			f, _ := strconv.ParseFloat(s, 32)
			*in = append(*in, float32(f))
			s = ""
		} else {
			s += string(buf[i]) //复杂度有问题
		}
	}
	f, _ := strconv.ParseFloat(s, 32)
	*in = append(*in, float32(f))
}

func main() {
	var filename string
	var in []float32
	fmt.Scanf("%s", &filename)
	inInit(&filename, &in)
	var time EncodeTime_t
	var data EncodeData_t
	Sdtdoor(&in, &time.src, &data.src)
	EncodeTime(&time)
	EncodeData(&data)
	num := len(time.src)
	var anstime Anstime_t
	timemap := make(map[int]int)
	anstime.num = append(anstime.num, int(num))
	timemap[anstime.num[0]]++
	if len(time.relnum)*3 < num*2 {
		anstime.num = append(anstime.num, 1)
		timemap[1]++
	} else {
		anstime.num = append(anstime.num, 0)
		timemap[0]++
	}
	if anstime.num[1] != 0 {
		for _, v := range time.relnum {
			anstime.num = append(anstime.num, v)
			timemap[v]++
		}
		for _, v := range time.relval {
			anstime.val = append(anstime.val, v)
			timemap[v]++
		}
	} else {
		for _, v := range time.src {
			anstime.val = append(anstime.val, v)
			timemap[v]++
		}
	}
	timesortarray := make([]TimeSortArray_t, 0, len(timemap))
	for k, v := range timemap {
		timesortarray = append(timesortarray, TimeSortArray_t{k, v})
	}
	sort.Sort(TimeSortArraySlince(timesortarray))
	outputFile, outputError := os.OpenFile("output.dat", os.O_WRONLY|os.O_CREATE, 0666)
	if outputError != nil {
		fmt.Printf("An error occurred with file opening or creation\n")
		return
	}
	defer outputFile.Close()
	outputWriter := bufio.NewWriter(outputFile)
	outputWriter.WriteString(strconv.Itoa(len(timesortarray)) + " ")
	var rank int
	for _, v := range timesortarray {
		outputWriter.WriteString(strconv.Itoa(int(v.val)) + " ")
		timemap[v.val] = int(rank)
		rank++
	}
	for _, v := range anstime.num {
		outputWriter.WriteString(strconv.Itoa(int(timemap[v])) + " ")
	}
	for _, v := range anstime.val {
		outputWriter.WriteString(strconv.Itoa(int(timemap[v])) + " ")
	}
	var ansdata Ansdata_t
	datamap := make(map[float32]int)
	if len(data.relnum)*3 < num*2 {
		ansdata.num = append(ansdata.num, 1)
		datamap[1]++
	} else {
		ansdata.num = append(ansdata.num, 0)
		datamap[0]++
	}
	if ansdata.num[0] != 0 {
		for _, v := range data.relnum {
			ansdata.num = append(ansdata.num, v)
			datamap[float32(v)]++
		}
		for _, v := range data.relval {
			ansdata.val = append(ansdata.val, v)
			datamap[v]++
		}
	} else {
		for _, v := range data.src {
			ansdata.val = append(ansdata.val, v)
			datamap[v]++
		}
	}
	datasortarray := make([]DataSortArray_t, 0, len(datamap))
	for k, v := range datamap {
		datasortarray = append(datasortarray, DataSortArray_t{k, v})
	}
	sort.Sort(DataSortArraySlince(datasortarray))
	outputWriter.WriteString(strconv.Itoa(len(datasortarray)) + " ")
	rank = 0
	for _, v := range datasortarray {
		s1 := strconv.FormatFloat(float64(v.val), 'f', -1, 32)
		outputWriter.WriteString(s1 + " ")
		datamap[v.val] = int(rank)
		rank++
	}
	for _, v := range ansdata.num {
		outputWriter.WriteString(strconv.Itoa(datamap[float32(v)]) + " ")
	}

	for _, v := range ansdata.val {
		outputWriter.WriteString(strconv.Itoa(datamap[v]) + " ")
	}
	outputWriter.Flush()
	println(len(timemap), len(datamap), len(time.src))
}
