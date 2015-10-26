(module
    ; currently not used because dynamic linking (and therefore memory sharing) is missing.
    (memory 100 (segment 0 "player.bmp") (segment 11 "ball.bmp"))

    (import $sleep "stdio" "sleep")
    
    (import $sdl_init "sdl" "init")
    (import $sdl_load_bmp "sdl" "load_bmp" (return i32))
    (import $sdl_render "sdl" "render" (param i32) (param i32) (param i32))
    (import $sdl_quit "sdl" "quit")
    (import $sdl_key_down "sdl" "key_down")
    (import $sdl_clear "sdl" "clear")
    (import $sdl_present "sdl" "present")

    (import $print_i32 "stdio" "print" (param i32))

    (func main
        (local $ballX i32)
        (local $bally i32)
        (local $vx i32)
        (local $vy i32)
        (local $ball_tex i32)
        (local $p1y i32)
        (local $p2y i32)
        (local $startTimer i32)

        (set_local $ballX (i32.const 0))
        (set_local $bally (i32.const 0))
        (set_local $vx (i32.const 5))
        (set_local $vy (i32.const 5))
        (set_local $p1y (i32.const 290))
        (set_local $p2y (i32.const 290))

        (set_local $startTimer (i32.const 500))

        (call $sdl_init (i32.const 100) (i32.const 100) (i32.const 800) (i32.const 600))
        (set_local $ball_tex (call $sdl_load_bmp))

        (forever
            (
                (set_local $ballX (i32.add (get_local $ballX) (get_local $vx)))
                (set_local $bally (i32.add (get_local $bally) (get_local $vy)))

                ; collision bottom
                (if (i32.gt_s (get_local $bally) (i32.const 580))
                    (
                        (set_local $vy (i32.sub (i32.const 0) (get_local $vy)))
                        (set_local $bally  (i32.const 580))
                    )
                )
                (if (i32.gt_s (get_local $ballX) (i32.const 780))
                    (
                        (set_local $vx (i32.sub (i32.const 0) (get_local $vx)))
                        (set_local $ballX  (i32.const 780))

                        (if (i32.or
                                (i32.lt_s (get_local $bally) (get_local $p2y))
                                (i32.gt_s (get_local $bally) (i32.add (get_local $p2y) (i32.const 80)))
                            )
                            (
                                (break)
                            )
                        )
                    )
                )
                (if (i32.lt_s (get_local $bally) (i32.const 0))
                    (
                        (set_local $vy (i32.sub (i32.const 0) (get_local $vy)))
                        (set_local $bally  (i32.const 0))
                    )
                )
                (if (i32.lt_s (get_local $ballX) (i32.const 0))
                    (
                        (set_local $vx (i32.sub (i32.const 0) (get_local $vx)))
                        (set_local $ballX  (i32.const 0))

                        (if (i32.or
                                (i32.lt_s (get_local $bally) (get_local $p1y))
                                (i32.gt_s (get_local $bally) (i32.add (get_local $p1y) (i32.const 80)))
                            )
                            (
                                (break)
                            )
                        )
                    )
                )

                (if (call $sdl_key_down (i32.const 1))
                    (
                        (set_local $p1y (i32.add (get_local $p1y) (i32.const 5)))
                    )
                )
                (if (call $sdl_key_down (i32.const 0))
                    (
                        (set_local $p1y (i32.sub (get_local $p1y) (i32.const 5) ))
                    )
                )

                (set_local $p2y (i32.sub (get_local $bally) (i32.const 40)))

                (call $sdl_clear)

                (call $sdl_render (get_local $ball_tex) (get_local $ballX) (get_local $bally) (i32.const 20) (i32.const 20))
                (call $sdl_render (get_local $ball_tex) (i32.const -5) (get_local $p1y) (i32.const 10) (i32.const 80))
                (call $sdl_render (get_local $ball_tex) (i32.const 795) (get_local $p2y) (i32.const 10) (i32.const 80))

                (call $sdl_present)

                (call $sleep (i32.const 66))

                (if (i32.gt_s (get_local $startTimer) (i32.const 0))
                    (set_local $startTimer (i32.sub (get_local $startTimer) (i32.const 66)))
                )

                (if (call $sdl_key_down (i32.const 2))
                    (break)
                )
            )
        )
        (call $sdl_quit)
    )
)
