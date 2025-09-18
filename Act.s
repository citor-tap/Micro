; Registros
n  rn r0     ; número n (0-47)
LP   rn r1     ; ciclo (i)
RDIR    rn r2     ; puntero a memoria

; Programa
; ----------------
        area    l_fibo, code, readonly
        entry
        export  __main

__main
        ; Dirección de SRAM
        ldr     RDIR, =0x20000FFF

        ; Valor de n.
        mov     n, #1

 ;Independiente 0 Y 1

        ; Comprobar n DIFERENTE 0
        cmp     n, #0
        blt     fin             ; 0, terminar.

        ; Manejar F(0)
        mov     r3, #0
        str     r3, [RDIR], #4  ;
        
        cmp     n, #0
        beq     fin             ; cuando n = 0, fin

        ; Manejar F(1)
        mov     r3, #1
        str     r3, [RDIR], #4  ; 

        cmp     n, #1
        beq     fin             ; cuando n = 1, fin

        ; para n mayor/igual 2
        mov     LP, #2

ciclo
        ldr     r3, [RDIR, #-4] ; n-1)
        ldr     r4, [RDIR, #-8] ; n-2)
        add     r3, r3, r4      ; F(i) = n-1 + n-2)
        str     r3, [RDIR], #4  ; Guardar F(i)
        add     LP, LP, #1
        cmp     LP, n
        ble     ciclo

fin
        b       fin
        end
