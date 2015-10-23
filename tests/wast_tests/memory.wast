(module
    (memory 4 4)
    (import $print_i32 "stdio" "print" (param i32))
    (func $print
        (i32.assert_return (i32.load8_u (i32.const 0)) (i32.const 8))
    )
    (func main
        (i32.store (i32.const 0) (i32.const 8))
        (call $print)

        (i32.store (i32.const 0) (i32.const 9))
        (i32.assert_return (i32.load8_u (i32.const 0)) (i32.const 9))
        (i32.assert_return (i32.load8_s (i32.const 0)) (i32.const 9))

        (i32.store8 (i32.const 0) (i32.const 8))
        (i32.assert_return (i32.load8_u (i32.const 0)) (i32.const 8))
        (i32.assert_return (i32.load8_s (i32.const 0)) (i32.const 8))
    )
)
