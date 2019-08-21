	.data				; Pocetak data sekcije.

ulazni_format:						; String za formatiranje ulaza scanf funkcije
	.byte 0x25, 105, 045, 0x69, 0	; "%i%i\0"

izlazni_format:						; String za formatiranje izlaza printf funkcije
	.byte 0x25, 0x69, 0x20, 0x2B	; "%i +" +
	.byte 0x20, 0x25, 0x69, 0x20	; " %i " +
	.byte 0x3d, 0x20, 0x25, 0x69	; "= %i" +
	.byte 0x5C, 0x6e, 0x0			; "\n\0"  = "%i + %i = %i\n"
;============================================================================================
	.text				; pocetak sekcije sa izvrsnim kodom

	.extern write
	.extern write
	.extern scanf
	.extern printf
	.extern exit

.global	start			
start:
	call main
	push 0
	call exit			; ekvivalentno: exit(0);

saberi:					; Primjer funkcije koja koristi
					;   stdcall konvenciju pozivanja.
	push r5				; bp
	mov r5, sp			
	mov	r0, r1[4]
	add	r0, r1[6]		; Rezultat ostaje u r0.
	pop r5
	pop r4
	pop r4				; Pri izlasku, u slucaju fiksnog broja parametara,
	ret				;  funkcija je duzna da oslobodi prostor koji su
					;  argumenti zauzimali na steku.
	 
main:
	push r5				; bp
	mov r5, sp

	mov 	r0, r1
	sub	r0, 2		; Adresa prve pomocne promjenljive (ebp-4)
	push	r0			; Treci argument za scanf poziv.

	sub	r0, 0x2			; Adresa druge pomocne promjenljive (ebp-8)
	push	r0			; Drugi argument za scanf poziv

	push	$ulazni_format		; Prvi argument za scanf poziv

	call	scanf
	add	r6, 6			; Oslobadjanje prostora koji su zauzimali argumenti.

	push	r5[-4]			; Dohvatanje prvog ucitanog broja - 2. arg. za "saberi".
	push	r5[-2]			; Dodavanje drugog ucitanog broja - 1. arg. za "saberi".
	call	saberi			; Rezultat vraca u registru eax.

	push	r0			; 4. argument - izracunata suma
	push	r5[-2]			; 3. argument - drugi ucitani broj
	push	r5[-4]			; 2. argument - prvi ucitani broj
	push	$izlazni_format		; 1. argument - adresa stringa za formatiranje

	call	printf
	add	r6, 8

	pop r5
	ret
.end