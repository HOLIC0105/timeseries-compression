package main

import (
	"compress/gzip"
	"fmt"
	"io/ioutil"
	"os"
	"sort"
	"strconv"
)

const MAX_float64 = 3.4028231e38
const eps = 1e-7

var e_delta float64 //绝对误差的范围

type EncodeTime_t struct {
	src    []int
	relnum []int
	relval []int
}

type EncodeData_t struct {
	src    []float64
	relnum []int
	relval []float64
}

type Anstime_t struct {
	num []int
	val []int
}

type Ansdata_t struct {
	num []int
	val []float64
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
	val float64
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

func Sdtdoor(in *[]float64, x *[]int, y *[]float64) {
	delta := e_delta
	var up_gate float64 = -MAX_float64
	var down_gate float64 = MAX_float64
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
		up_gate = max(up_gate, (y_delta-delta)/float64(x_delta))
		down_gate = min(down_gate, (y_delta+delta)/float64(x_delta))
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
	var lst float64 = data.src[0]
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

func inInit(filename *string, in *[]float64) {
	buf := read(filename)
	Len := len(buf)
	var s string
	for i := 0; i < Len; i++ {
		if buf[i] == ' ' || buf[i] == '\n' {
			f, _ := strconv.ParseFloat(s, 32)
			*in = append(*in, float64(f))
			s = ""
		} else {
			s += string(buf[i]) //复杂度有问题
		}
	}
	f, _ := strconv.ParseFloat(s, 32)
	*in = append(*in, float64(f))
}

func main() {
	var filename string
	var in []float64
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
	file, _ := os.Create("compressed")
	defer file.Close()
	gw := gzip.NewWriter(file)
	defer gw.Close()
	gw.Write([]byte(strconv.Itoa(len(timesortarray)) + " "))
	var rank int
	for _, v := range timesortarray {
		gw.Write([]byte(strconv.Itoa(int(v.val)) + " "))
		timemap[v.val] = int(rank)
		rank++
	}
	for _, v := range anstime.num {
		gw.Write([]byte(strconv.Itoa(int(timemap[v])) + " "))
	}
	for _, v := range anstime.val {
		gw.Write([]byte(strconv.Itoa(int(timemap[v])) + " "))
	}
	var ansdata Ansdata_t
	datamap := make(map[float64]int)
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
			datamap[float64(v)]++
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
	gw.Write([]byte(strconv.Itoa(len(datasortarray)) + " "))
	rank = 0
	for _, v := range datasortarray {
		s1 := strconv.FormatFloat(float64(v.val), 'f', -1, 32)
		gw.Write([]byte(s1 + " "))
		datamap[v.val] = int(rank)
		rank++
	}
	for _, v := range ansdata.num {
		gw.Write([]byte(strconv.Itoa(datamap[float64(v)]) + " "))
	}
	for _, v := range ansdata.val {
		gw.Write([]byte(strconv.Itoa(datamap[v]) + " "))
	}
}
