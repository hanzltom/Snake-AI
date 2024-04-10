(define (domain ball)
    (:requirements :strips :typing)  

    (:types
        ball
        color
        part
    )

    (:predicates
        (has-color ?b - ball ?c - color ?p - part) 
        (glasses ?ball - ball)
    )

    (:action paint
        :parameters (?b - ball ?fromWhole - color ?fromEyes - color ?to - color)  
        :precondition (and
                          (not (glasses ?b))
                          (has-color ?b ?fromWhole whole)
                          (has-color ?b ?fromEyes eyes)
                      )
        :effect (and
                    (not(has-color ?b ?fromWhole whole))
                    (not(has-color ?b ?fromEyes eyes))
                    (has-color ?b ?to whole)
                    (has-color ?b ?to eyes)
                )
    )

    (:action paintEyes
        :parameters (?b - ball ?fromWhole - color ?to - color) 
        :precondition (and
                          (glasses ?b)
                          (has-color ?b ?fromWhole whole)
                      )
        :effect (and
                    (not(has-color ?b ?fromWhole whole))
                    (has-color ?b ?to whole)
                )
    )

    (:action takeGlasses
        :parameters (?b - ball)
        :precondition (not (glasses ?b))
        :effect (glasses ?b)
    )

    (:action dropGlasses
        :parameters (?b - ball)
        :precondition (glasses ?b)
        :effect (not (glasses ?b))
    )
)