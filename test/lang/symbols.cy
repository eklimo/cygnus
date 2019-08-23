func print() {}

var global = 5
var unused = ()

func sum(a: Int, b: Int) -> Int
{
	var sum = a + b
	return sum
}

if global > 3
{
	var msg = global + " > 3"
	print(msg)
}
