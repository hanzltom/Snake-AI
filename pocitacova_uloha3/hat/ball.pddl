(define (domain ball)
    (:requirements :strips :typing)  

    (:types
        ball
        color
        part
    )

    (:predicates
        (has-color ?b - ball ?c - color ?p - part) 
        (hat ?ball - ball)
    )

    (:action paint
        :parameters (?b - ball ?fromTop - color ?fromBottom - color ?to - color)  
        :precondition (and
                          (not (hat ?b))
                          (has-color ?b ?fromTop top)
                          (has-color ?b ?fromBottom bottom)
                      )
        :effect (and
                    (not(has-color ?b ?fromTop top))
                    (not(has-color ?b ?fromBottom bottom))
                    (has-color ?b ?to top)
                    (has-color ?b ?to bottom)
                )
    )

    (:action paintHat
        :parameters (?b - ball ?fromBottom - color ?to - color)
        :precondition (and
                          (hat ?b)
                          (has-color ?b ?fromBottom bottom)
                      )
        :effect (and
                    (not(has-color ?b ?fromBottom bottom))
                    (has-color ?b ?to bottom)
                )
    )

    (:action takeHat
        :parameters (?b - ball)
        :precondition (not (hat ?b))
        :effect (hat ?b)
    )

    (:action dropHat
        :parameters (?b - ball)
        :precondition (hat ?b)
        :effect (not (hat ?b))
    )
)