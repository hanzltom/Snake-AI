(define (problem ball1)
    (:domain ball)
    (:requirements :strips)
    (:objects
        ball - ball  
        c0 c1 c2 - color
        top bottom - part
    )

    (:init
        (not (hat ball))
        (has-color ball c0 top)
        (has-color ball c0 bottom)
    )

    (:goal
        (and
            (has-color ball c1 top)
            (has-color ball c2 bottom)
        )
    )
)