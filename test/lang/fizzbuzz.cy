func print(s: String) {}

func fizzbuzz(n: Int)
{
    var i = 1
    while i < n {
        var str = ""
        if i % 3 == 0 {
            str = str + "Fizz"
        }
        if i % 5 == 0 {
            str = str + "Buzz"
        }
        if str == "" {
            str = "" + i
        }
        print(str)
        i++
    }
}

fizzbuzz(5)
