(define (problem ball1)
    (:domain ball)
    (:requirements :strips)
    (:objects
        ball - ball  
        c0 black blue orange yellow - color
        top second third bottom - part
    )

    (:init
        (not (hat ball))
        (not (belt ball))
        (has-color ball c0 top)
        (has-color ball c0 second)
        (has-color ball c0 third)
        (has-color ball c0 bottom)
    )

    (:goal
        (and
            (not (hat ball))
            (not (belt ball))
            (has-color ball black top)
            (has-color ball yellow second)
            (has-color ball orange third)
            (has-color ball blue bottom)
        )
    )
)