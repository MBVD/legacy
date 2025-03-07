;Задача. Считать строку и записать количество вхождений символа х(он в секции ДАТА)
%include "io64.inc"   ;работаем в 64-разрядной системе
extern scanf   ;подключаем шаблон функции из С scanf
extern printf    ;подключаем шаблон функции из С printf

section .bss     ;раздел изменяемых данных(переменных)
 y resb 200   ;создаём переменную(строку) у, сотсоящую из 200 байт (resb)
 
section .data  ;раздел неизменяемых данных(констант)
    x db 'e'      ;создаём неизменяемый байтовый(db) символ х, который хранит в данном случаем букву "е"
    scanf_format db "%s", 0  ;вводим формат вызова функции из С scanf, он имеет такой вид, %s это  тип данных - строки(что считываем - строку), 0 это переход на новую строку с нажатием Enter(или пустая строка?, не знаю) 
    printf_format db " End of y %d", 10, 0 ;формат вызова функции из С printf, %d - это  тип данных число int (что печатаем - количество вхожднений символа х) , 10 - это десятичная система счисления,  0 это переход на новую строку с нажатием Enter(или пустая строка?, не знаю) 
section .text   ;секция текста программы
global CMAIN   ;глобальная основная функция CMAIN


CMAIN:
    mov rbp, rsp; for correct debugging
enter 0, 0    ;тоже самое, что и push rbp, mov rbp, rsp; нужно для профилактики ошибок (пролог функции), чтобы прога аварийно не завершалась
mov rbp, rsp; for correct debugging

scan:   ;начинаем  процедуру сканирования введённой строки
    lea rdi, [scanf_format]  ;принято соглашение (порядок регистров) о передаче ПАРАМЕТРОВ функций В РЕГИСТРЫ на языке Ассемблера, первый регистр (по соглашению) rdi получает сам формат вызываемой функции сканф(а если точнее, то первый аргумент - %s)
    lea rsi, [y] ;второй по соглашению регистр rsi принимает второй аргумент - адрес вводимой строки, так как функция в Си scanf("%s", &y), амперсанд & - это адрес вводимой строки y (%s - строка)
    call scanf  ;вызываем(call) функцию сканф, только после передачи в неё параметров , функция сканф сканирует всю строку вплоть до перехода на новую строку Enter
    ;команда lea, точно не знаю, load effective address, загружает в регистр адрес переменной
    
mov edx, 0   ;обнуляем регистр edx, это то же самое что и xor edx, edx
mov esi, y   ; теперь помещаем в еси именно первый элемент, голову, который называется как вся строка, но на самом деле он указывает на первый символ
mov bl, [x]  ; загружаем символ x , то есть "е" в регистр бл
xor rcx, rcx ; обнуляем rcx, это будет счетчик вхождений символа х в строке
xor rax, rax ;обнуляем рах


    

search:   ;начинаем поиск символа х в строке
lodsb       ;строковая команда, считывает то, что находится в регистре esi, помещает в eax(или ax/ al) и после этого увеличивает esi, то есть esi будет указывать уже на следующий символ в строке
cmp al, 0  ; Сравнение, Проверка того, что lodsb поместил в al, на конец строки 
je done      ; !!Если они равны(jump equal), т.е. мы достигли конца строки, то прыгаем в done(выполнено)
cmp al, bl ;  Опять Сравнение текущего символа в al с символом х
jne search  ;    Если они не равны, не совпадают, то прыгаем в начало search, и начинаем цикл заново : lodsb>...>...
inc rcx ; Здесь мы увеличиваем счётчик rсх на 1 (инкрементируем- обратная операция дикрементирование), т.е. если было совпадение, мы не прыгаем в начало, а просто идём дальше увеличиваем rcx
jmp search ; Прыгаем опять в начало  search, переходя ко второму символу строки

done: ;начинаем процедуру печати 
lea rdi, [printf_format] ; опять по соглашению о передаче в первый регистр записываем сам формат вызываемой функции (точнее текст с ПЕРВЫМ АРГУМЕНТОМ %d)
mov rsi, rcx ;по соглашению rsi - это второй регистр, в него передаётся второй аргумент функции printf("%d", rcx), то есть у нас теперь это не адрес(амперсанд), а само интовое число, которое есть rcx(в рсх хранится кол-во вхождений сивола х в строке, его и надо напечатать). 
mov rax, 1 ;это означает то же, что и return 1 , т.е. возращаемся, для успешного выхода из функции
call printf; вызываем функцию принтф
xor rax, rax ;обнуляем рах
leave ; это эпилог функции (для выравнивания стека), то же самое что и mov rsp, rbp, pop rbp, для корректного выхода из функции
ret ;return , возвращаемся из функции done


leave ;эпилог для CMAIN
ret  ;реторн (возвращаемся из функции CMAIN)
