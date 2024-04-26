package main

const MAX_FLOAT32 = 3.4028231e38
const eps = 1e-8

var e_delta float32 //绝对误差的范围

type EncodeTime_t struct {
	src    []int32
	relnum []int32
	relval []int32
}

type EncodeData_t struct {
	src    []float32
	relnum []int
	relval []float32
}

func Sdtdoor(in *[]float32, x *[]int32, y *[]float32) {
	delta := e_delta
	var up_gate float32 = -MAX_FLOAT32
	var down_gate float32 = MAX_FLOAT32
	var end int32 = int32(len(*in))
	var tim int32 = 1
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
	for i := len; i >= 0; i-- {
		data.src[i] = data.src[i] - data.src[i-1]
	}
	var lst, num int32 = data.src[0], 0
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
	var lst, num = data.src[0], 0
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

func main() {
	var in []float32
	var time EncodeTime_t
	var data EncodeData_t
	Sdtdoor(&in, &time.src, &data.src)
	EncodeTime(&time)
	EncodeData(&data)

}
