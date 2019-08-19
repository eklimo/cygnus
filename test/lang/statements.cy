print("hello, world")
var x = f(5, "abc")
x = -2
var y: Int
var unit: () = ()

func add(a: Int, b: Int) -> Int
{
	var sum = a + b
	print(sum)
	return sum
}

func max(a: Int, b: Int) -> Int
{
	return if a > b { a } else { b }
}

func count(n: Int)
{
	var x = 0
	while x < n
	{
		print(x)
		x++
	}
}
