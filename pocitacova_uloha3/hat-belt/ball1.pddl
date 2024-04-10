(define (problem ball1)
    (:domain ball)
    (:requirements :strips)
    (:objects
        ball - ball  
        c0 yellow black blue - color
        top middle bottom - part
    )

    (:init
        (not (hat ball))
        (not (belt ball))
        (has-color ball c0 top)
        (has-color ball c0 middle)
        (has-color ball c0 bottom)
    )

    (:goal
        (and
            (not (hat ball))
            (not (belt ball))
            (has-color ball blue top)
            (has-color ball yellow middle)
            (has-color ball black bottom)
        )
    )
)