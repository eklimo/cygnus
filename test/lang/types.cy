func print(x: String) {}

func fizzbuzz(n: Int)
{
	var i = 1
	while(i < n)
	{
		var str = ""
		if(i % 3 == 0) { str = str + "Fizz" }
		if(i % 5 == 0) { str = str + "Buzz" }
		if(i % 3 != 0 && i % 5 != 0) { str = str + i }
		print(str)
		i++
	}
}

fizzbuzz(print(5))
