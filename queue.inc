
;PARAMETERS
;    1. Value
;RETURN:
;    POINTER TO NEW LINKED NODE
_init_node:
    mov ebp, esp
    mov ebx, [ebp + 4]
    MALLOC 8
    mov dword[eax], ebx        ; node.value = value
    mov dword[eax + 4], 0      ; node.next = 0
    ret

_init_queue:
    MALLOC 12
    mov dword[eax], 0       ;Ptr to Front
    mov dword[eax + 4], 0   ;Ptr to Back
    mov dword[eax + 8], 0    ;Count
    ret


; PARAMETERS:
;    2. Value
;    1. Ptr to queue
; RETURNS:
;    Void
_push:
    mov ebp, esp
    mov esi, [ebp + 4]    ; Ptr to queue
    lea edi, [esi + 4]  ; Ptr to Back

    mov ebx, [ebp + 8]    ; Value
    add dword[esi + 8], 1 ;Count++

    INIT_NODE ebx
    
    cmp dword[esi], 0
    JE _push.empty_queue
    ; if ptrFirst == 0 :
    ;    goto empty_queue
    ; Else:
        mov edi, dword[esi + 4]
        mov dword[edi + 4], eax    ; back.next = new node
        mov dword[esi + 4], eax  ; ptrBack = new node
        ret
    .empty_queue:
        mov dword[esi], eax        ;ptrFist = new node
        mov dword[esi + 4], eax    ;ptrLast = new node
    ret

_count:
    mov ebp, esp
    mov esi, [ebp + 4]
    mov eax, dword[esi + 8]
    ret


_pop:
    mov ebp, esp
    mov esi, [ebp + 4]
    mov eax, -1
    ;Si no contiene elementos
    cmp dword[esi + 8], 0
    JE _pop.exit
    
    mov edi, [esi]        ; First Node
    mov eax, [edi]        ; Value to Return
    push eax
    
    mov ebx, [edi + 4]    ; second Node
    mov dword[esi], ebx   ; First = Second
    sub dword[esi + 8], 1 ; Count--
    FREE edi
    
        
    ;Si habia un solo elemento
    cmp dword[esi + 8], 0
    JE _pop.one_item
    pop eax
    
    .exit:
        ret
        
    .one_item:
        mov dword[esi + 4], 0    ; Last = null
        pop eax
        ret


Print_Queue:
    mov ebp, esp
    mov esi, [ebp + 4]
    
    printD dword[esi + 8]
    endl
    
    ;Count
    cmp dword[esi + 8], 0
    JE while.END
    
    mov esi, [esi]
    
    ;First Item
    printS dword[esi]
    endl

    while:
        cmp dword[esi + 4], 0
        JE while.END
            mov esi, dword[esi + 4]
            printS dword[esi]
            endl

        JMP while
    
    .END:
        ret
