
# Div `/`

Makes a division of it's arguments, takes a quotient. Supports such types and their combinations: `i64`, `I64`, `f64`, `F64`.

``` clj
↪ (/ 1 2)
0
↪ (/ 1.2 2.2)
0
↪ (/ 1 3.4)
0
↪ (/ [-1 -2 -3] 3)
[-1 -1 -1]
↪ (/ [10 -10 3] 2.1)
[4 -5 1]
↪ (/ 3.1 [1 2 -3])
[3.00 1.00 -2.00]
```
