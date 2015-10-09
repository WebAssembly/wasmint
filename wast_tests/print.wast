(module
    (import $print_i32 "stdio" "print" (param i32))
    (func $print (param $i i32)
        (call $print_i32 (get_local $i))
    )
    (func main
        (call $print (i32.const 13))
    )
)
