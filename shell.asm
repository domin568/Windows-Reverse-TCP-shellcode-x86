[bits 32]

mov eax, [fs:0x30] ; PEB
mov eax,[eax+0xC] ; Peb->Ldr
mov eax,[eax+0x14] ; Ldr.List
mov esi ,eax
mov esi, [esi] 
mov esi, [esi]
mov ebp, [esi+0x10]
mov edi, [ebp+0x3C] ; offset to PE Header
add edi, 0x4 + 0x14 + 0x60 ; signature + file header + export table offset
add edi,ebp ; VA convert
mov edi,[edi]
add edi,ebp ; EDI = VA IMAGE EXPORT DIRECTORY
mov eax,[edi+0x20] 
add eax,ebp ; adres w tej tablicy ze stringami

mov esi,eax

mov eax,[edi+0x1C] ; 
add eax,ebp ; adres tablicy adresow

mov edx, eax
xor ecx,ecx

; ESI tablica stringow VA   EDX tablica ordinali   VA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

find_getproc:

inc ecx ; zwiekszamy ordinal funkcji

lodsd ; w eax jest adres stringa aktualnie sprawdzanego a esi = esi + 4

add eax,ebp ; VA convert

cmp dword [eax], 0x50746547 ; GetP

jne find_getproc    

cmp dword [eax+0x4], 0x41636f72 ;rocA

jne find_getproc

cmp dword [eax+0x8], 0x65726464 ; ddre

jne find_getproc

found: ; TODO : TEST ON WIN 10 

mov esi, [edx + ecx * 4]
add esi,ebp

push ebp
push esi
push 0 ; adres 0x00000000 na ktory ma wrocic, czyli sie wylozy

