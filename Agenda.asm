%include "utility_macros.inc"
%include "queue.inc"

extern _scanf, _printf, _malloc, _realloc, _free
extern _strcpy, _strcmp, _strlen, _putchar, _gets
extern _strtok, _getchar
extern _addPerson, _LOGIN, _LIST, _printt, _getPerson, _UPDATE, _REMOVE_PERSON, _SEARCH_BY_NAME, _SEARCH_BY_PHONE
extern _LOAD, _SAVE, _LOGOUT, _EXIT_AGENDASM, _HELP, _AddGroup, _addPersontoGroup, _ListGroup, _groups, _removeGroup

section .data

; Statement
addd db "ADD", 0
remove db "REMOVE", 0
update db "UPDATE", 0
show db "SHOW", 0
list db "LIST", 0
save db "SAVE", 0
load db "LOAD", 0
addgroup db "ADDGROUP", 0
removegroup db "REMOVEGROUP", 0
addtogroup db "ADDTOGROUP", 0
listgroup db "LISTGROUP", 0
searchname db "SEARCHNAME", 0
searchphone db "SEARCHPHONE", 0
logout db "LOGOUT", 0
login db "LOGIN", 0
register db "REGISTER", 0
exit db "EXIT", 0
about db "ABOUT", 0
help db "HELP", 0
group db "GROUPS: ", 0


;Line
lineToRead times 10000 db 0
inEjecution dd 0
numberLine dd 0
countStatement dd 0
formatS db "%s", 0
formatD db "%d", 0
formatC dd "%c", 0
delimitSpace db " ", 0
delimitPtoComa db ";", 0
exitBool db 0
logoutBool db 0
begin db ": $ ", 0
intro db "", 0

viewHelp db "Run HELP; to display the help index", 10, 0
viewAbout db "Run ABOUT; to display the authors", 10, 0


openingText db "Welcome to AgendASM (version 1.0 - preview 20151208)", 10, 0
program db "AgendASM (version 1.0 - preview 20151208)", 10, 0

;About*******************************************

authors db "Developed by:", 10, 0

author1Name db "       Ivan Galban Smith", 10, 0
author1Major db "       2nd Computer Science", 10, 0
author1University db "       University of Havana", 10, 0
author1Email db "       i.galban@lab.matcom.uh.cu", 10, 0

author2Name db "       Raydel Alonso", 10, 0
author2Email db "       r.alonso@lab.matcom.uh.cu", 10, 0

license db "GNU Software Public License", 10, 0

;************************************************

;Special Operators
ptoComa db 59
dosPto db 58
comillaSimple db 39


;Structures
queue dd 0
queueToken dd 0
queueLine dd 0

; Errors
happenedError db 0

section .text


;bool isStatement(int* word)
; eax = 1 True
; eax = 0 False
isStatement:
    
    Get_ptrLine esi
        
    strcmp esi, addd
    cmp eax, 0
    JE isStatement.True

    strcmp esi, remove
    cmp eax, 0
    JE isStatement.True

    strcmp esi, update
    cmp eax, 0
    JE isStatement.True

    strcmp esi, show
    cmp eax, 0
    JE isStatement.True
    
    strcmp esi, list
    cmp eax, 0
    JE isStatement.True

    strcmp esi, save
    cmp eax, 0
    JE isStatement.True
    
    strcmp esi, load
    cmp eax, 0
    JE isStatement.True
    
    strcmp esi, addgroup
    cmp eax, 0
    JE isStatement.True
    
    strcmp esi, removegroup
    cmp eax, 0
    JE isStatement.True
    
    strcmp esi, addtogroup
    cmp eax, 0
    JE isStatement.True
    
    strcmp esi, listgroup
    cmp eax, 0
    JE isStatement.True
    
    strcmp esi, searchname
    cmp eax, 0
    JE isStatement.True
    
    strcmp esi, logout
    cmp eax, 0
    JE isStatement.True


    strcmp esi, help
    cmp eax, 0
    JE isStatement.True

    strcmp esi, about
    cmp eax, 0
    JE isStatement.True

    strcmp esi, exit
    cmp eax, 0
    JE isStatement.True

    .False:
        mov byte[happenedError], 0
        ret

    .True:
        mov byte[happenedError], 1
        ret



;bool Add_Compile()
Add_Compile:
    
	count_queue dword[queueToken]

	cmp eax, 2
	JNE Error_Invalid_Syntax
	; if(inEjecution.length != 2)
	;	goto Invalid_Syntax
	; Else:
    	pop_queue dword[queueToken]
    	mov esi, eax			;Name

    	strlen esi

    	cmp byte[esi + eax - 1], ':'
    	JNE Error_DosPtos
		; if(token[length-1] != ':')
		;	goto Error
		; Else:
    		
    		push esi
    		pop_queue dword[queueToken]
    		pop esi

    		mov edi, eax			; #Phone

    		strlen esi
    		mov byte[esi + eax - 1], 0

    		push esi
    		call isStatement
    		pop esi

    		cmp byte[happenedError], 1
    		JE Error_isStatement_Language

    		;printS edi
    		;endl
    		;printS esi

			ret

    Error_DosPtos:
    	mov byte[happenedError], 1
    	Throw_Error
    	Expected_Error [dosPto]
        ret

    Error_Invalid_Syntax:
    	mov byte[happenedError], 1
    	Throw_Error
    	Invalid_Syntax dword[inEjecution]
        ret
    
    Error_isStatement_Language:
    	Throw_Error
    	isStatement_Error esi
    	ret

;void Add_Execute()
Add_Execute:

    call Add_Compile
    
    cmp byte[happenedError], 1
    JE Add_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Añade contacto
        ;Add(esi, edi)
        push edi
        push esi
        call _addPerson
        add esp, 8
        ;Verificar que el contacto no exista

        cmp eax, 0
        JE Add_Execute.Error0

        cmp eax, 2
        JE Add_Execute.Error2

    .Exit:
        ret

    .Error0:
        Throw_Error
        printS esi
        printC ' '
        printS error0
        endl
        JMP Add_Execute.Exit

    .Error2:
        Throw_Error
        printS edi
        printC ' '
        printS error2
        endl
        JMP Add_Execute.Exit

    


;bool Remove_Compile()
Remove_Compile:
    
    call Show_Compile
    ret
        
;void Remove_Execute()
Remove_Execute:

    call Remove_Compile
    
    cmp byte[happenedError], 1
    JE Remove_Execute.Exit
    ;if happenedError == 1 :
    ;    goto Exit
    ; Else:
        ;Eliminar contacto de la persona entrada
        ;Remove(esi)
        push esi
        call _REMOVE_PERSON
        add esp, 4

        ;Si no existe Throw_Error no existe persona a eliminar
        cmp eax, 0
        JE Remove_Execute.Error0


    .Exit:
        ret

    .Error0:
        Throw_Error
        printS esi
        printC ' '
        printS error0
        endl
        JMP Remove_Execute.Exit

    
;void Update_Compile()
Update_Compile:

    call Add_Compile
    ret

;void Update_Execute()
Update_Execute:
    
    call Update_Compile
    
    cmp byte[happenedError], 1
    JE Update_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Actualiza
        ;Update(esi, edi)
        push edi
        push esi
        call _UPDATE
        add esp, 8
        ;Invoke2 update
        ;Verificar que exista el contacto
        

        cmp eax, 0
        JE Update_Execute.Error0

        cmp eax, 2
        JE Update_Execute.Error2

    .Exit:
        ret

    .Error0:
        Throw_Error
        printS esi
        printC ' '
        printS error0
        endl
        JMP Update_Execute.Exit

    .Error2:
        Throw_Error
        printS edi
        printC ' '
        printS error2
        endl
        JMP Add_Execute.Exit
    

;void Show_Compile()
Show_Compile:
    
    count_queue dword[queueToken]
    
    cmp eax, 1
    JNE Show_Compile.Error_Invalid_Syntax
    
    ; if(inEjecution.length != 2)
	;	goto Invalid_Syntax
	; Else:

	pop_queue dword[queueToken]
    mov esi, eax			;Name

    push esi
    call isStatement
    pop esi
    
    cmp byte[happenedError], 1
    JE Show_Compile.Error_isStatement
        ret
    
    .Error_Invalid_Syntax:
    	mov byte[happenedError], 1
        Throw_Error
        Invalid_Syntax dword[inEjecution]
        ret
        
    .Error_isStatement:
    	mov byte[happenedError], 1
        Throw_Error
        isStatement_Error esi
        ret
    

    
;void Show_Execute()
Show_Execute:

    call Show_Compile
    
    cmp byte[happenedError], 1
    JE Show_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Mostrar # phone persona
        ;Show(esi)
        push esi
        call _getPerson

        ;Invoke1 show

        ;Verificar que exista la persona
        cmp eax, 0
        JE Show_Execute.Error0

        push eax
        call Print_List
        add esp, 4

        printC ' '

        call _groups
        call Print_Group

    
    .Exit:
        add esp, 4
        ret

    .Error0:
        Throw_Error
        printS esi
        printC ' '
        printS error0
        endl
        JMP Show_Execute.Exit



;void List_Compile()
List_Compile:
    
	count_queue dword[queueToken]

	cmp eax, 0
	JNE Error
    mov byte[happenedError], 0
    
    ret
    
    Error:
        mov byte[happenedError], 1
        Throw_Error
        Invalid_Syntax dword[inEjecution]
    	ret


Print_List:
    
    push eax
    mov ebx, [eax]
    xor ecx, ecx


    .While:
        cmp ecx, ebx
        JE Print_List.Exit
        ;if(i == length)
        ;   goto break;
        ;Else:
            mov esi, [eax + ecx * 8 + 4]
            
            printS esi
            printC ' '
            
            mov esi, [eax + ecx * 8 + 8]

            printS esi
            endl
            
            inc ecx
        JMP Print_List.While

    .Exit:
        pop eax
        FREE eax
        ret

;void List_Execute()
List_Execute:
    
    call List_Compile

    cmp byte[happenedError], 1
    JE List_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;call Trie.List
        ;Invoke0 list
        call _LIST
        call Print_List
        ;Mostrar todos los contactos guardados
    
    .Exit:
        ret


;void Save_Compile()
Save_Compile:
	
	call Show_Compile
	ret


;void Save_Execute()
Save_Execute:

	call Save_Compile

	cmp byte[happenedError], 1
	JE Save_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
    	;call Metodo Save in C
    	;Save(esi)
        push esi
        call _SAVE
        add esp, 4

        ;Controlar Errores
        cmp eax, 3
        JE Save_Execute.Error3

    .Exit:
        ret
    
    .Error3:
        Throw_Error
        printS error3
        endl
        JMP Save_Execute.Exit



;void Load_Compile()
Load_Compile:
	
	call Show_Compile
	ret


;void Load_Execute()
Load_Execute:

    call Load_Compile

	cmp byte[happenedError], 1
	JE Load_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
    	;call Metodo Load in C
    	;Load(esi)
        push esi
        call _LOAD
        add esp, 4

    	;Controlar Errores
        cmp eax, 3
        JE Load_Execute.Error3

    .Exit:
    	ret
    
    .Error3:
        Throw_Error
        printS error3
        endl
        JMP Load_Execute.Exit


;void AddGroup_Compile()
AddGroup_Compile:
    
    call Show_Compile
    ret


;void Add_Execute()
AddGroup_Execute:
    
    call AddGroup_Compile
    
    cmp byte[happenedError], 1
    JE AddGroup_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Añadir grupo
        ;AddGroup(esi)
        push esi
        call _AddGroup
        add esp, 4
        ;Verificar que exista el grupo
        cmp eax, 4
        JE AddGroup_Execute.Error4

    .Exit:
        ret

    .Error4:
        Throw_Error
        printS error4
        endl
        JMP AddGroup_Execute.Exit



;RemoveGroup_Compile()
RemoveGroup_Compile:
    
    call Show_Compile
    ret
    
    
    
;void Add_Execute()
RemoveGroup_Execute:

    call RemoveGroup_Compile
    
    cmp byte[happenedError], 1
    JE RemoveGroup_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Eliminar grupo
        ;RemoveGroup(esi)
        push esi
        call _removeGroup
        add esp, 4

        ;Verificar que el grupo exista
        cmp eax, 5
        JE RemoveGroup_Execute.Error5
    
    .Exit:
        ret
    
    .Error5:
        Throw_Error
        printS error5
        endl
        JMP RemoveGroup_Execute.Exit
    
;AddtoGroup_Compile()
AddtoGroup_Compile:
    
    call Add_Compile
    ret
    

;void Add_Execute(int* line)    
AddtoGroup_Execute:


    call AddtoGroup_Compile
    
    cmp byte[happenedError], 1
    JE AddtoGroup_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Añade una persona al grupo
        ;AddtoGroup(esi, edi)
        push edi
        push esi
        call _addPersontoGroup
        add esp, 8
        
        ;Verificar que exista el grupo

        cmp eax, 5
        JE AddtoGroup_Execute.Error5

        cmp eax, 6
        JE AddtoGroup_Execute.Error6

        cmp eax, 0
        JE AddtoGroup_Execute.Error0
        
    .Exit:
        ret

    .Error0:
        Throw_Error
        printS esi
        printC ' '
        printS error0
        endl
        JMP Add_Execute.Exit

    .Error5:
        Throw_Error
        printS error5
        endl
        JMP AddGroup_Execute.Exit

    .Error6:
        Throw_Error
        printS esi
        printS error6
        endl
        JMP AddGroup_Execute.Exit
    

Print_Group:
    
    printS group

    push eax

    mov ebx, [eax]
    xor ecx, ecx

    .While:
        cmp ecx, ebx
        JE Print_Group.Exit
        ;if(i == length)
        ;   goto break;
        ;Else:
            mov esi, [eax + ecx * 4 + 4]
            
            printS esi
            printC ' '
            inc ecx

        JMP Print_Group.While

    .Exit:
        pop eax
        FREE eax
        endl
        ret


;void ListGroup_Execute()
ListGroup_Compile:
    
    call Show_Compile
    ret
    
    
;void Add_Execute()    
ListGroup_Execute:
    
    call ListGroup_Compile
    
    cmp byte[happenedError], 1
    JE ListGroup_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Mostrar todos los contactos que pertenecen al grupo  
        ;ListGroup(esi)

        push esi
        call _ListGroup
        add esp, 4

        ;Verificar que exista el grupo
        cmp eax, 0
        JE ListGroup_Execute.Error5

        call Print_List
    
    .Exit:
        ret
    
    .Error5:
        Throw_Error
        printS error5
        endl
        JMP ListGroup_Execute.Exit

;void SearchName_Compile()
SearchName_Compile:
    
    call Show_Compile
    ret
    
    
;void SearchName_Execute()
SearchName_Execute:

    call SearchName_Compile
    
    cmp byte[happenedError], 1
    JE SearchName_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Mostrar # phone todas las personas que tienen como prefijo a cadena
        ;SearchName(esi)
        push esi
        call _SEARCH_BY_NAME
        add esp, 4

        cmp eax, 0
        JE SearchName_Execute.Exit
        call Print_List
    
    .Exit:
        ret
    

;void SearchPhone_Compile()
SearchPhone_Compile:
    
    call Show_Compile
    ret
    
;void Add_Execute(void)
SearchPhone_Execute:

    call SearchPhone_Compile

    
    cmp byte[happenedError], 1
    JE SearchPhone_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        ;Mostrar todos los nombres de las personas que sus #s 
        ;tienen como prefijo a la cadena telefonica de entrada
        ;SearchPhone(esi)
        push esi
        call _SEARCH_BY_PHONE
        add esp, 4

        cmp eax, 0
        JE SearchPhone_Execute.Exit
        call Print_List
    
    .Exit:
        ret


;void Split(char* line, char* delimit, int* queue)
_Split:
    mov ebp, esp
    mov esi, [ebp+4]        ;line
    mov edi, [ebp + 8]      ;delimit
    
    mov eax, [ebp + 12]     
    mov dword[queue], eax   ;queue

    strtok esi, edi
    
    .While:
        cmp eax, 0
        JE _Split.Exit
        ; if token == null
        ;    goto Exit
        ;Else:
            ;Añadir token a la cola
            pusha
            push_queue dword[queue], eax
            popa
            strtok 0, edi
     JMP _Split.While

    .Exit:
        ret


;void Exit_Compile()
Exit_Compile:

    call List_Compile
    ret


;void Exit_Executive()
Exit_Execute:

    call Exit_Compile

    cmp byte[happenedError], 1
    JE Exit_Execute.Exit
    ; if(happenedError == true)
    ;       goto Exit
    ; Else:
        mov byte[exitBool], 1
        call _EXIT_AGENDASM

    .Exit:
        ret

    ret

;void Logout_Compile()
Logout_Compile:

    call List_Compile
    ret


;void Logout_Execute()
Logout_Execute:
    
    call Logout_Compile
    
    cmp byte[happenedError], 1
    JE Logout_Execute.Exit
    ; if(happenedError == true)
    ;       goto Exit
    ; Else:
        mov byte[logoutBool], 1
        call _LOGOUT

    .Exit:
        ret



Help_Compile:
    
    call List_Compile
    ret
    

Help_Execute:
    
    call Help_Compile

    cmp byte[happenedError], 1
    JE Help_Execute.Exit
    ; if(happenedError == true)
    ;   goto Exit
    ; Else:
        ;Display Help
        call _HELP

    .Exit:
    ret



;void Execute(int* line)
_Execute:
    
    mov byte[happenedError], 0		;happenedError = false
    mov ebp, esp
    mov esi, [ebp + 4]

    call _init_queue
    mov dword[queueToken], eax


    ;Tokenizing
    Split esi, delimitSpace, dword[queueToken]

    pop_queue dword[queueToken]

    cmp eax, -1
    JE _Execute.False
    
    mov esi, eax
    mov dword[inEjecution], esi

    strcmp esi, addd
    cmp eax, 0
    JE Add_Execute

    strcmp esi, remove
    cmp eax, 0
    JE Remove_Execute

    strcmp esi, update
    cmp eax, 0
    JE Update_Execute

    strcmp esi, show
    cmp eax, 0
    JE Show_Execute
    
    strcmp esi, list
    cmp eax, 0
    JE List_Execute

    strcmp esi, save
    cmp eax, 0
    JE Save_Execute
    
    strcmp esi, load
    cmp eax, 0
    JE Load_Execute
    
    strcmp esi, addgroup
    cmp eax, 0
    JE AddGroup_Execute
    
    strcmp esi, removegroup
    cmp eax, 0
    JE RemoveGroup_Execute
    
    strcmp esi, addtogroup
    cmp eax, 0
    JE AddtoGroup_Execute
    
    strcmp esi, listgroup
    cmp eax, 0
    JE ListGroup_Execute
    
    strcmp esi, searchname
    cmp eax, 0
    JE SearchName_Execute
    
    strcmp esi, searchphone
    cmp eax, 0
    JE SearchPhone_Execute
    
    strcmp esi, logout
    cmp eax, 0
    JE Logout_Execute

    strcmp esi, help
    cmp eax, 0
    JE Help_Execute

    strcmp esi, about
    cmp eax, 0
    JE About_Execute

    strcmp esi, exit
    cmp eax, 0
    JE Exit_Execute

    .False:
        mov byte[happenedError], 1
        Throw_Error
        InvalidStatement_Error esi
        
    ret

; Process the line to analyse
;void Process(int* line)
_Process:
    
    Get_ptrLine esi
        
    strlen esi

    cmp byte[esi + eax - 1], ';'
    JNE Error_PtoComa
    ; if(line[length - 1] != ';')
	; 	goto Error
	; Else:
	    ;Determinar instrucciones de una misma linea. Separadas por ';'
    	Split esi, delimitPtoComa, dword[queueLine] 
    	 
      While: 
    	  pop_queue dword[queueLine]

		  cmp eax, -1
		  JE _Process.Exit
		  ; if queueLine.empty :
		  ;		goto CONTINUE
		  ; Else:
		  	  mov dword[inEjecution], eax
		   	  push eax
		   	  call 	_Execute
		   	  add esp, 4

		   	  cmp byte[exitBool], 1
		   	  JE _Process.Exit
              ;if(logoutBool == true)
              ;     goto _Process.Exit

	   JMP While
    
    Error_PtoComa:
    	Throw_Error
    	Expected_Error [ptoComa]		; Throw_Error
        ret

    _Process.Exit:
    	FREE dword[queueLine]
		FREE dword[queueToken]
		ret


ReadLine:
    
    mov byte[logoutBool], 0
    mov byte[exitBool], 0
	add dword[numberLine], 1
	printD dword[numberLine]
	printS begin

	scanner lineToRead

	strcmp lineToRead, intro
	cmp eax, 0
	JE ReadLine
	; if(line == enter)
	;	goto ReadLine
	; Else: 
		call _init_queue
		mov dword[queueLine], eax

		push lineToRead
		call _Process
		add esp, 4

        cmp byte[exitBool], 1
        JE ReadLine.Exit

        cmp byte[logoutBool], 1
        JE ReadLine.Exit

	JMP ReadLine

	.Exit:
		ret

;void _About()
_About:
    
    printS program
    endl

    printS authors
    printS author1Name
    printS author1Major
    printS author1University
    printS author1Email

    endl

    printS author2Name
    printS author1Major
    printS author1University
    printS author2Email

    ret


;void About_Compile()
About_Compile:
    
    call List_Compile
    ret


;void About()
About_Execute:

    call About_Compile

    cmp byte[happenedError], 1
    JE Update_Execute.Exit
    ;if happenedError == true :
    ;    goto Exit
    ; Else:
        call _About
        
    .Exit:
        ret



;void Opening()
Opening:
    printS openingText
    endl
    printS viewHelp
    endl

    ret

global _main
_main:
    
    
    call Opening

    .Begin:

        mov dword[numberLine], 0
        call _LOGIN
    
        push eax
        call _getchar
        add esp, 4

	    call ReadLine
        cmp byte[exitBool], 1
        JE _main.Exit

    JMP _main.Begin
    
   .Exit:
        xor eax, eax
        ret