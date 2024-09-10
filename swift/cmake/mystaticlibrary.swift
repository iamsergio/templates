@_cdecl("fibo_static")
public func fibonacci_static(_ n: Int) -> Int {
    if n <= 1 {
        return n
    }

    var a = 0
    var b = 1

    for _ in 2...n {
        let next = a + b
        a = b
        b = next
    }

    return b
}