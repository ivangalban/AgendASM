errorLine db "Error Line ", 0
invalidStatement db " is an invalid statement", 0
expected db " expected", 0
invalidSyntax db "Invalid Syntax in ", 0
statementLanguage db " is a statement of language", 0

error0 db "is an invalid name", 0                ;0
error2 db "Invalid Number", 0                    ;2
error3 db "File not Found", 0                    ;3
error4 db "The group already exist", 0           ;4
error5 db "The group not exist", 0               ;5
error6 db " already exist", 0                    ;6

;*************************************

%macro Invoke2 1
    printS %1
    printC '('
    printS esi
    printC ','
    printS edi
    printC ')'
    endl
%endmacro

%macro Invoke1 1
    printS %1
    printC '('
    printS esi
    printC ')'
    endl
%endmacro

%macro Invoke0 1
        printS %1
        printC '('
        printC ')'
        endl
%endmacro

;**************************************

;Error Macro***************************************
%macro Throw_Error 0
    printS errorLine
    printD dword[numberLine]
    print_char [dosPto]
    print_char ' '
%endmacro

%macro InvalidStatement_Error 1
    printS dword %1
    printS invalidStatement
    endl
%endmacro

%macro Expected_Error 1
    print_char [comillaSimple]
    print_char %1
    print_char [comillaSimple]
    printS expected
    endl
%endmacro

%macro Invalid_Syntax 1
    printS invalidSyntax
    printS %1
    endl
%endmacro

%macro isStatement_Error 1
    printS %1
    printS statementLanguage
    endl
%endmacro
    

;***************************************************

;***********Macros IO***********
%macro printC 1
    pushad
    push %1
    push formatC
    call _printf
    add esp, 8
    popad
%endmacro

%macro printS 1
    pushad
    push %1
    push formatS
    call _printf
    add esp, 8
    popad
%endmacro

%macro printD 1
    pushad
    push %1
    push formatD
    call _printf
    add esp, 8
    popad
%endmacro

%macro print_char 1
    pushad
    push dword %1
    call _putchar
    add esp, 4
    popad
%endmacro
    
%macro endl 0
    pushad
    print_char 10
    popad
%endmacro
    
%macro scan 1
    push %1
    push formatS
    call _scanf
    add esp, 8
%endmacro

%macro scanner 1
    push %1
    call _gets
    add esp, 4
%endmacro

;*******************************

%macro Get_ptrLine 1
    mov ebp, esp
    mov %1, [ebp + 4]
%endmacro


;Queue Macro
; %1 -> line %2 -> delimit
%macro strtok 2
    push %2
    push %1
    call _strtok
    add esp, 8
%endmacro

; %1 -> *ptrQueue %2 -> Value
%macro push_queue 2
    push %2
    push %1
    call _push
    add esp, 8
%endmacro

%macro MALLOC 1
    push dword %1
    call _malloc
    add esp, 4
%endmacro

%macro FREE 1
    push dword %1
    call _free
    add esp, 4
%endmacro

%macro INIT_NODE 1
    push dword %1
    call _init_node
    add esp, 4
%endmacro

;*************************


;Queue Macro**************
%macro pop_queue 1
    push %1
    call _pop
    add esp, 4
%endmacro

%macro count_queue 1
    push %1
    call _count
    add esp, 4
%endmacro

;String Macro****************

%macro strcmp 2
    push %1
    push %2
    call _strcmp
    add esp, 8
%endmacro

%macro strlen 1
    push dword %1
    call _strlen
    add esp, 4
%endmacro

;void Split(char* line, char* delimit, int* queue)
%macro Split 3
    push %3
    push %2
    push %1
    call _Split
    add esp, 12
%endmacro

;****************************