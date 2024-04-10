(define (domain ball)
    (:requirements :strips :typing)  

    (:types
        ball
        color
        part
    )

    (:predicates
        (has-color ?b - ball ?c - color ?p - part) 
        (hatOn ?ball - ball)
        (glassesOn ?ball - ball)
        (maskOn ?ball - ball)
    )

    (:action paint
        :parameters (?b - ball ?fromTopWhole - color ?fromBottomWhole - color ?fromTopMask - color ?fromBottomMask - color ?fromTopEyes - color ?fromBottomEyes - color ?to - color)  
        :precondition (and
                          (not (hatOn ?b))
                          (not (glassesOn ?b))
                          (not (maskOn ?b))
                          (has-color ?b ?fromTopWhole topWhole)
                          (has-color ?b ?fromBottomWhole bottomWhole)
                          (has-color ?b ?fromTopMask topMask)
                          (has-color ?b ?fromBottomMask bottomMask)
                          (has-color ?b ?fromTopEyes topEyes)
                          (has-color ?b ?fromBottomEyes bottomEyes)
                      )
        :effect (and
                    (not(has-color ?b ?fromTopWhole topWhole))
                    (not(has-color ?b ?fromBottomWhole bottomWhole))
                    (not(has-color ?b ?fromTopMask topMask))
                    (not(has-color ?b ?fromBottomMask bottomMask))
                    (not(has-color ?b ?fromThird topEyes))
                    (not(has-color ?b ?fromBottom bottomEyes))
                    (has-color ?b ?to topWhole)
                    (has-color ?b ?to bottomWhole)
                    (has-color ?b ?to topMask)
                    (has-color ?b ?to bottomMask)
                    (has-color ?b ?to topEyes)
                    (has-color ?b ?to bottomEyes)
                )
    )

    (:action paintHat
        :parameters (?b - ball ?fromBottomWhole - color ?fromBottomMask - color ?fromBottomEyes - color ?to - color)
        :precondition (and
                          (hatOn ?b)
                          (not (glassesOn ?b))
                          (not (maskOn ?b))
                          (has-color ?b ?fromBottomWhole bottomWhole)
                          (has-color ?b ?fromBottomMask bottomMask)
                          (has-color ?b ?fromBottomEyes bottomEyes)
                      )
        :effect (and
                    (not(has-color ?b ?fromBottomWhole bottomWhole))
                    (not(has-color ?b ?fromBottomMask bottomMask))
                    (not(has-color ?b ?fromBottomEyes bottomEyes))
                    (has-color ?b ?to bottomWhole)
                    (has-color ?b ?to bottomMask)
                    (has-color ?b ?to bottomEyes)
                )
    )
    
    (:action paintMask
        :parameters (?b - ball ?fromTopWhole - color ?fromBottomWhole - color ?to - color) 
        :precondition (and
                          (maskOn ?b)
                          (not (hatOn ?b))
                          (has-color ?b ?fromTopWhole topWhole)
                          (has-color ?b ?fromBottomWhole bottomWhole)
                      )
        :effect (and
                    (not(has-color ?b ?fromTopWhole topWhole))
                    (not(has-color ?b ?fromBottomWhole bottomWhole))
                    (has-color ?b ?to topWhole)
                    (has-color ?b ?to bottomWhole)
                )
    )
    
    (:action paintGlasses
        :parameters (?b - ball ?fromTopWhole - color ?fromBottomWhole - color ?fromTopMask - color ?fromBottomMask - color ?to - color) 
        :precondition (and
                          (glassesOn ?b)
                          (not (hatOn ?b))
                          (not (maskOn ?b))
                          (has-color ?b ?fromTopWhole topWhole)
                          (has-color ?b ?fromBottomWhole bottomWhole)
                          (has-color ?b ?fromTopMask topMask)
                          (has-color ?b ?fromBottomMask bottomMask)
                      )
        :effect (and
                    (not(has-color ?b ?fromTopWhole topWhole))
                    (not(has-color ?b ?fromBottomWhole bottomWhole))
                    (not(has-color ?b ?fromTopMask topMask))
                    (not(has-color ?b ?fromBottomMask bottomMask))
                    (has-color ?b ?to topWhole)
                    (has-color ?b ?to bottomWhole)
                    (has-color ?b ?to topMask)
                    (has-color ?b ?to bottomMask)
                )
    )
    
    (:action paintHatGlasses
        :parameters (?b - ball ?fromBottomWhole - color ?fromBottomMask - color ?to - color) 
        :precondition (and
                          (hatOn ?b)
                          (glassesOn ?b)
                          (not(maskOn ?b))
                          (has-color ?b ?fromBottomWhole bottomWhole)
                          (has-color ?b ?fromBottomMask bottomMask)
                      )
        :effect (and
                    (not(has-color ?b ?fromBottomWhole bottomWhole))
                    (not(has-color ?b ?fromBottomMask bottomMask))
                    (has-color ?b ?to bottomWhole)
                    (has-color ?b ?to bottomMask)
                )
    )
    
    (:action paintHatMask
        :parameters (?b - ball ?fromBottomWhole - color ?to - color) 
        :precondition (and
                          (hatOn ?b)
                          (maskOn ?b)
                          (has-color ?b ?fromBottomWhole bottomWhole)
                      )
        :effect (and
                    (not(has-color ?b ?fromBottomWhole bottomWhole))
                    (has-color ?b ?to bottomWhole)
                )
    )

    (:action takeHat
        :parameters (?b - ball)
        :precondition (not (hatOn ?b))
        :effect (hatOn ?b)
    )

    (:action dropHat
        :parameters (?b - ball)
        :precondition (hatOn ?b)
        :effect (not (hatOn ?b))
    )
    
    (:action takeMask
        :parameters (?b - ball)
        :precondition (and
                        (not (maskOn ?b))
                        (not (hatOn ?b))
                    )
        :effect (maskOn ?b)
    )

    (:action dropMask
        :parameters (?b - ball)
        :precondition (and
                        (maskOn ?b)
                        (not (hatOn ?b))
                    )
        :effect (not (maskOn ?b))
    )
    
    (:action takeGlasses
        :parameters (?b - ball)
        :precondition (and
                        (not (glassesOn ?b))
                        (not (maskOn ?b))
                        (not (hatOn ?b))
                    )
        :effect (glassesOn ?b)
    )

    (:action dropGlasses
        :parameters (?b - ball)
        :precondition (and
                        (glassesOn ?b)
                        (not (maskOn ?b))
                        (not (hatOn ?b))
                    )
        :effect (not (glassesOn ?b))
    )
)