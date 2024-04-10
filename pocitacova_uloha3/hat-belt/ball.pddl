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
        (belt ?ball - ball)
    )

    (:action paint
        :parameters (?b - ball ?fromTop - color ?fromMiddle - color ?fromBottom - color ?to - color)  
        :precondition (and
                          (not (hat ?b))
                          (not (belt ?b))
                          (has-color ?b ?fromTop top)
                          (has-color ?b ?fromMiddle middle)
                          (has-color ?b ?fromBottom bottom)
                      )
        :effect (and
                    (not(has-color ?b ?fromTop top))
                    (not(has-color ?b ?fromMiddle middle))
                    (not(has-color ?b ?fromBottom bottom))
                    (has-color ?b ?to top)
                    (has-color ?b ?to middle)
                    (has-color ?b ?to bottom)
                )
    )

    (:action paintHat
        :parameters (?b - ball ?fromMiddle - color ?fromBottom - color ?to - color) 
        :precondition (and
                          (hat ?b)
                          (not (belt ?b))
                          (has-color ?b ?fromMiddle middle)
                          (has-color ?b ?fromBottom bottom)
                      )
        :effect (and
                    (not(has-color ?b ?fromMiddle middle))
                    (not(has-color ?b ?fromBottom bottom))
                    (has-color ?b ?to middle)
                    (has-color ?b ?to bottom)
                )
    )
    
    (:action paintBelt
        :parameters (?b - ball ?fromTop - color ?fromBottom - color ?to - color) 
        :precondition (and
                          (belt ?b)
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
    
    (:action paintHatBelt
        :parameters (?b - ball ?fromBottom - color ?to - color) 
        :precondition (and
                          (belt ?b)
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
    
    (:action takeBelt
        :parameters (?b - ball)
        :precondition (not (belt ?b))
        :effect (belt ?b)
    )

    (:action dropBeltt
        :parameters (?b - ball)
        :precondition (belt ?b)
        :effect (not (belt ?b))
    )
)