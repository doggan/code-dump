; author @ shyam m guthikonda
; date   @ 03 april 2006
; desc   @ a function that determines the # of words/tokens in a given string.

(define tokenCounter 0)

(define (countStrTokens aStr)
  (cond
    ; current is char and length is 1. increment. done.
    ((and (not (char=? (string-ref aStr 0) #\space))
          (eqv? (string-length aStr) 1))
     (set! tokenCounter (+ 1 tokenCounter))
     tokenCounter)
    
    ; current is space and length is 1. done.
    ((and (char=? (string-ref aStr 0) #\space)
          (eqv? (string-length aStr) 1))
     tokenCounter)
    
    ; current is char and next is space. increment.
    ((and (not (char=? (string-ref aStr 0) #\space))
          (char=? (string-ref aStr 1) #\space))
     (set! tokenCounter (+ 1 tokenCounter))
     (countStrTokens (substring aStr 1)))
    
    ; current is char and next is char. skip over.
    ; current is space and next is space. skip over.
    ; current is space and next is char. skip over.
    (true (countStrTokens (substring aStr 1)))
    )
  )

(countStrTokens "hello")(set! tokenCounter 0)
(countStrTokens " hello")(set! tokenCounter 0)
(countStrTokens "   hello   ")(set! tokenCounter 0)
(countStrTokens "hello ")(set! tokenCounter 0)
(countStrTokens "Hello  ")(set! tokenCounter 0)
(countStrTokens "Hello there")(set! tokenCounter 0)
(countStrTokens "Hello there ")(set! tokenCounter 0)
(countStrTokens "Hello    there ")(set! tokenCounter 0)
(countStrTokens "Hello there dood!")(set! tokenCounter 0)