(define (problem ball1)
    (:domain ball)
    (:requirements :strips)
    (:objects
        ball - ball  
        c0 yellow black - color
        whole eyes - part
    )

    (:init
        (not (glasses ball))
        (has-color ball c0 whole)
        (has-color ball c0 eyes)
    )

    (:goal
        (and
            (has-color ball yellow whole)
            (has-color ball black eyes)
            (not(glasses ball))
        )
    )
)