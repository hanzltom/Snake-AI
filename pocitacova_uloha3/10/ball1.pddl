(define (problem ball1)
    (:domain ball)
    (:requirements :strips)
    (:objects
        ball - ball  
        c0 black blue red - color
        topWhole bottomWhole topMask bottomMask topEyes bottomEyes - part
    )

    (:init
        (not (glassesOn ball))
        (not (maskOn ball))
        (not (hatOn ball))
        (has-color ball c0 topWhole)
        (has-color ball c0 bottomWhole)
        (has-color ball c0 topMask)
        (has-color ball c0 bottomMask)
        (has-color ball c0 topEyes)
        (has-color ball c0 bottomEyes)
    )

    (:goal
        (and
            (not (glassesOn ball))
            (not (maskOn ball))
            (not (hatOn ball))
            (has-color ball red topMask)
            (has-color ball red bottomMask)
            (has-color ball black topEyes)
            (has-color ball blue bottomEyes)
            (has-color ball blue topWhole)
            (has-color ball blue bottomWhole)
        )
    )
)