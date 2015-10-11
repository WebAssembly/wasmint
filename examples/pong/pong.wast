(module
    (import $sleep "stdio" "sleep")
    (import $sdl_init "sdl" "init")
    (import $sdl_load_bmp "sdl" "load_bmp" (return i32))
    (import $sdl_render "sdl" "render" (param i32) (param i32) (param i32))
    (import $sdl_quit "sdl" "quit")

    (func main (local $timer i32) (local $x i32) (local $y i32)
        (set_local $timer (i32.const 180))
        (call $sdl_init)
        (call $sdl_load_bmp)
        (do_while
            (
                (call $sdl_render (i32.const 1) (get_local $x) (get_local $y))
                (call $sleep)
                (set_local $timer (int32.sub (get_local $timer) (i32.const 1)))
                (set_local $x (int32.add (get_local $x) (i32.const 6)))
                (set_local $y (int32.add (get_local $y) (i32.const 6)))
            )
            (get_local $timer)
        )
        (call $sdl_quit)
    )
)
