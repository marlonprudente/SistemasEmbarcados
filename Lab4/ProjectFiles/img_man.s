B1 EQU 0x000000FF
DIR EQU 0
IMG_LENGTH EQU 0x00004800
			
        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
		EXPORT f_asm
        IMPORT bus
		IMPORT car
		IMPORT airplane
		IMPORT buffalo
		EXTERN flags
			
f_asm
	push {r0-r11}
	ldr r3, =flags
	ldr r4, [r3, #12]
	cmp r4, #0
	beq load_bus
	cmp r4, #1
	beq load_plane

load_bus
	ldr r0, =bus
	b setup

load_plane
	ldr r0, =airplane
	b setup


setup
	mov r9, #0
	mov r10, r0
	ldrb r2, [r0, #0]!
	mov r1, #0
	mov r5, #IMG_LENGTH
	ldrb r4, [r3, #0]
	cmp r4, #1
	beq revert_color
	ldrb r4, [r3, #12]
	cmp r4,#0
	beq setup_car
	cmp r4,#1
	beq setup_airplane
	
setup_car
	ldrb r4, [r3, #8]
	cmp r4, #0
	beq finish
	cmp r4, #1
	bne speed_up
	b finish
setup_airplane

	ldrb r4, [r3, #8]
	cmp r4, #0
	beq finish
	cmp r4, #1
	bne speed_up_airplane
	b finish
	
revert_color
	cmp r1, r5
	bgt finish
	eor r2, B1
	strb r2, [r0]
	ldrb r2, [r0, #1]!
	add r1, r1, #1
	b revert_color


speed_up
	mov r0, r10
	ldr r3, =flags
	ldrb r7, [r3, #4]
	cmp r7, #0
	addne r0, r0, r5
	subne r0, r0, #3
	ldrbeq r2, [r0]
	moveq r1, #0
	movne r1, #1
	cmp r9, r4
	beq finish
	add r9, r9, #1
	b line_setup
	
line_setup
	cmp r7, #0
	ldrbeq r2, [r0, #285]
	movne r6, #0
	subne r6, r6, #285
	ldrbne r2, [r0, r6]

	mov r8, #285
	mov r6, #0
	b move

move
	cmp r1, r5
	bgt speed_up
	cmp r6, r8
	bgt line_setup
	ldrb r3, [r0]
	strb r2, [r0]
	mov r2, r3
	cmp r7, #0
	addeq r0, r0, #3
	subne r0, r0, #3
	add r1, r1, #3
	add r6, r6, #3
	b move
	
speed_up_airplane
	mov r0, r10
	ldr r3, =flags
	ldrb r7, [r3, #4]

	ldrbeq r2, [r0]
	moveq r1, #0
	cmp r9, r4
	beq finish
	add r9, r9, #1
	mov r11,#0
	b row_setup_airplane
	
row_setup_airplane
	
	mov r0,r10 ;VOLTA PARA O INICIO DA IMAGEM
	cmp r7, #0
	movne r5, #0x46e0
	addne r0, r0, r5
	add r0,r0,r11 ;DISLOCA O PONTEIRO NA QUANTIDADE DE COLUNAS JA TERMINADAS
	moveq r6, #0x46e0 ;ADICIONA O NUMERO DE ELEMENTOS ENTRE O ELEMENTO ATUAL E O ULTIMO DA COLUNA
	ldreq r2, [r0,r6]
	movne r6, #0
	movne r8, #0x46e0
	subne r6, r6, r8
	ldrbne r2, [r0, r6]
	mov r8, #63 ;NUMERO DE LINHAS DEPOIS DA PRIMEIRA 
	moveq r6, #0
	movne r6, #1
	b move_airplane
;36576 - 18144
move_airplane
	cmp r11, #288 ;VERIFICA SE TODAS AS COLUNAS JA FORAM EFETUADAS 
	bgt speed_up_airplane
	cmp r6, r8
	addgt r11,r11,#3
	bgt row_setup_airplane
	ldrb r3, [r0]
	strb r2, [r0]
	mov r2, r3
	cmp r7, #0
	addeq r0, r0, #288
	subne r0, r0, #288
	add r6, r6, #1 ;ADICIONA 1 PARA O CONTADOR DE LINHAS 
	b move_airplane

finish
	pop{r0-r11}
	bx lr
	
	ALIGN
	END