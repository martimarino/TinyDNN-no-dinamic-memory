	.arch armv8.4-a+crc+sve
	.file	"test.cpp"
	.text
	.align	2
	.p2align 4,,11
	.global	_Z7sv_maddSt6vectorIfSaIfEES1_
	.type	_Z7sv_maddSt6vectorIfSaIfEES1_, %function
_Z7sv_maddSt6vectorIfSaIfEES1_:
.LFB2213:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE2213:
	.size	_Z7sv_maddSt6vectorIfSaIfEES1_, .-_Z7sv_maddSt6vectorIfSaIfEES1_
	.align	2
	.p2align 4,,11
	.global	_Z5benchSt6vectorIfSaIfEES1_i
	.type	_Z5benchSt6vectorIfSaIfEES1_i, %function
_Z5benchSt6vectorIfSaIfEES1_i:
.LFB2214:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2214
	stp	x29, x30, [sp, -96]!
	.cfi_def_cfa_offset 96
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -80
	.cfi_offset 20, -72
	mov	x20, x0
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -64
	.cfi_offset 22, -56
	mov	x21, x1
	mov	w22, 0
	stp	x23, x24, [sp, 48]
	.cfi_offset 23, -48
	.cfi_offset 24, -40
	mov	x23, 9223372036854775804
	stp	x25, x26, [sp, 64]
	.cfi_offset 25, -32
	.cfi_offset 26, -24
	mov	w25, w2
	bl	_ZNSt6chrono3_V212steady_clock3nowEv
	cmp	w25, 0
	mov	x26, x0
	ble	.L14
	str	x27, [sp, 80]
	.cfi_offset 27, -16
	.p2align 3,,7
.L4:
	ldp	x1, x2, [x20]
	sub	x0, x2, x1
	mov	x24, x0
	cbz	x0, .L19
.L33:
	cmp	x0, x23
	bhi	.L30
.LEHB0:
	bl	_Znwm
.LEHE0:
	mov	x19, x0
	ldp	x1, x2, [x20]
	sub	x0, x2, x1
.L5:
	cmp	x1, x2
	beq	.L7
	mov	x2, x0
	mov	x0, x19
	bl	memcpy
.L7:
	ldp	x1, x0, [x21]
	subs	x2, x0, x1
	beq	.L31
	cmp	x2, x23
	mov	x27, x2
	bhi	.L32
	mov	x0, x2
.LEHB1:
	bl	_Znwm
.LEHE1:
	ldp	x1, x2, [x21]
	mov	x3, x0
	cmp	x1, x2
	beq	.L12
	sub	x2, x2, x1
.L9:
	mov	x0, x3
	bl	memcpy
	mov	x3, x0
.L12:
	mov	x1, x27
	mov	x0, x3
	bl	_ZdlPvm
.L10:
	cbz	x19, .L13
	mov	x1, x24
	mov	x0, x19
	add	w22, w22, 1
	bl	_ZdlPvm
	cmp	w25, w22
	bne	.L4
.L28:
	ldr	x27, [sp, 80]
	.cfi_restore 27
.L14:
	bl	_ZNSt6chrono3_V212steady_clock3nowEv
	sub	x26, x0, x26
	mov	x0, 13531
	movk	x0, 0xd7b6, lsl 16
	movk	x0, 0xde82, lsl 32
	movk	x0, 0x431b, lsl 48
	ldp	x19, x20, [sp, 16]
	smulh	x0, x26, x0
	ldp	x21, x22, [sp, 32]
	asr	x0, x0, 18
	sub	x26, x0, x26, asr 63
	ldp	x23, x24, [sp, 48]
	scvtf	d0, x26
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L13:
	.cfi_def_cfa_offset 96
	.cfi_offset 19, -80
	.cfi_offset 20, -72
	.cfi_offset 21, -64
	.cfi_offset 22, -56
	.cfi_offset 23, -48
	.cfi_offset 24, -40
	.cfi_offset 25, -32
	.cfi_offset 26, -24
	.cfi_offset 27, -16
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	add	w22, w22, 1
	cmp	w25, w22
	beq	.L28
	ldp	x1, x2, [x20]
	sub	x0, x2, x1
	mov	x24, x0
	cbnz	x0, .L33
.L19:
	mov	x19, 0
	b	.L5
	.p2align 2,,3
.L31:
	cmp	x0, x1
	beq	.L10
	mov	x3, 0
	mov	x27, 0
	b	.L9
.L30:
.LEHB2:
	bl	_ZSt17__throw_bad_allocv
.LEHE2:
.L32:
.LEHB3:
	bl	_ZSt17__throw_bad_allocv
.LEHE3:
.L21:
	mov	x20, x0
	cbz	x19, .L17
	mov	x1, x24
	mov	x0, x19
	bl	_ZdlPvm
.L17:
	mov	x0, x20
.LEHB4:
	bl	_Unwind_Resume
.LEHE4:
	.cfi_endproc
.LFE2214:
	.global	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA2214:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2214-.LLSDACSB2214
.LLSDACSB2214:
	.uleb128 .LEHB0-.LFB2214
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB2214
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L21-.LFB2214
	.uleb128 0
	.uleb128 .LEHB2-.LFB2214
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB3-.LFB2214
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L21-.LFB2214
	.uleb128 0
	.uleb128 .LEHB4-.LFB2214
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSE2214:
	.text
	.size	_Z5benchSt6vectorIfSaIfEES1_i, .-_Z5benchSt6vectorIfSaIfEES1_i
	.section	.rodata._ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"vector::_M_realloc_insert"
	.section	.text._ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_,"axG",@progbits,_ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_,comdat
	.align	2
	.p2align 4,,11
	.weak	_ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_
	.type	_ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_, %function
_ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_:
.LFB2711:
	.cfi_startproc
	stp	x29, x30, [sp, -96]!
	.cfi_def_cfa_offset 96
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	stp	x25, x26, [sp, 64]
	.cfi_offset 21, -64
	.cfi_offset 22, -56
	.cfi_offset 25, -32
	.cfi_offset 26, -24
	ldp	x25, x22, [x0]
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -80
	.cfi_offset 20, -72
	mov	x20, x0
	stp	x23, x24, [sp, 48]
	mov	x19, 2305843009213693951
	stp	x27, x28, [sp, 80]
	.cfi_offset 23, -48
	.cfi_offset 24, -40
	.cfi_offset 27, -16
	.cfi_offset 28, -8
	sub	x0, x22, x25
	cmp	x19, x0, asr 2
	beq	.L49
	asr	x3, x0, 2
	mov	x24, x1
	cmp	x3, 0
	mov	x23, x2
	csinc	x0, x3, xzr, ne
	sub	x26, x1, x25
	adds	x3, x3, x0
	bcs	.L46
	cbnz	x3, .L39
	mov	x19, 0
	mov	x21, 0
.L45:
	ldr	s0, [x23]
	add	x23, x26, 4
	sub	x22, x22, x24
	add	x23, x21, x23
	cmp	x26, 0
	add	x27, x23, x22
	str	s0, [x21, x26]
	ldr	x28, [x20, 16]
	bgt	.L50
	cmp	x22, 0
	bgt	.L41
	cbnz	x25, .L44
.L42:
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	stp	x21, x27, [x20]
	str	x19, [x20, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 27
	.cfi_restore 28
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L50:
	.cfi_restore_state
	mov	x2, x26
	mov	x1, x25
	mov	x0, x21
	bl	memmove
	cmp	x22, 0
	ble	.L44
.L41:
	mov	x2, x22
	mov	x1, x24
	mov	x0, x23
	bl	memcpy
	cbz	x25, .L42
.L44:
	sub	x1, x28, x25
	mov	x0, x25
	bl	_ZdlPvm
	b	.L42
	.p2align 2,,3
.L39:
	cmp	x3, x19
	csel	x3, x3, x19, ls
	lsl	x19, x3, 2
.L38:
	mov	x0, x19
	bl	_Znwm
	mov	x21, x0
	add	x19, x0, x19
	b	.L45
	.p2align 2,,3
.L46:
	mov	x19, 9223372036854775804
	b	.L38
.L49:
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	_ZSt20__throw_length_errorPKc
	.cfi_endproc
.LFE2711:
	.size	_ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_, .-_ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC1:
	.string	"\r"
	.align	3
.LC2:
	.string	"\\"
	.align	3
.LC3:
	.string	"\rTime (ms): "
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 4,,11
	.global	main
	.type	main, %function
main:
.LFB2224:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2224
	stp	x29, x30, [sp, -224]!
	.cfi_def_cfa_offset 224
	.cfi_offset 29, -224
	.cfi_offset 30, -216
	mov	w2, 10
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -192
	.cfi_offset 22, -184
	mov	x21, x1
	mov	x1, 0
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -208
	.cfi_offset 20, -200
	mov	w19, 0
	ldr	x0, [x21, 8]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	d8, [sp, 80]
	.cfi_offset 23, -176
	.cfi_offset 24, -168
	.cfi_offset 25, -160
	.cfi_offset 26, -152
	.cfi_offset 72, -144
	bl	strtol
	mov	x23, x0
	mov	w2, 10
	ldr	x0, [x21, 16]
	mov	x1, 0
	mov	w20, w23
	bl	strtol
	mov	x22, x0
	ldr	x0, [x21, 24]
	mov	w2, 10
	mov	x1, 0
	bl	strtol
	mov	x24, x0
	movi	v0.4s, 0
	mov	w0, 134
	str	xzr, [sp, 112]
	mov	w21, w24
	str	xzr, [sp, 144]
	str	q0, [sp, 96]
	str	q0, [sp, 128]
	bl	srand
	cmp	w23, 0
	bgt	.L52
	b	.L58
	.p2align 2,,3
.L114:
	str	s0, [x1], 4
	str	x1, [sp, 104]
	bl	rand
	scvtf	s0, w0
	ldp	x1, x0, [sp, 136]
	str	s0, [sp, 192]
	cmp	x1, x0
	beq	.L56
.L115:
	str	s0, [x1], 4
	add	w19, w19, 1
	str	x1, [sp, 136]
	cmp	w19, w20
	beq	.L58
.L52:
	bl	rand
	scvtf	s0, w0
	ldp	x1, x0, [sp, 104]
	str	s0, [sp, 192]
	cmp	x1, x0
	bne	.L114
	add	x2, sp, 192
	add	x0, sp, 96
.LEHB5:
	bl	_ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_
	bl	rand
	scvtf	s0, w0
	ldp	x1, x0, [sp, 136]
	str	s0, [sp, 192]
	cmp	x1, x0
	bne	.L115
	.p2align 3,,7
.L56:
	add	x2, sp, 192
	add	x0, sp, 128
	bl	_ZNSt6vectorIfSaIfEE17_M_realloc_insertIJfEEEvN9__gnu_cxx17__normal_iteratorIPfS1_EEDpOT_
	add	w19, w19, 1
	cmp	w19, w20
	bne	.L52
	.p2align 3,,7
.L58:
	adrp	x23, .LC1
	adrp	x20, _ZSt4cout
	movi	d8, #0
	add	x23, x23, :lo12:.LC1
	add	x20, x20, :lo12:_ZSt4cout
	mov	w19, 0
	b	.L53
	.p2align 2,,3
.L118:
	mov	x0, 9223372036854775804
	cmp	x25, x0
	bhi	.L116
	mov	x0, x25
	bl	_Znwm
.LEHE5:
	mov	x3, x0
	ldp	x1, x0, [sp, 96]
	sub	x26, x0, x1
.L60:
	dup	v0.2d, x3
	add	x25, x3, x25
	str	x25, [sp, 176]
	cmp	x1, x0
	str	q0, [sp, 160]
	beq	.L62
	mov	x0, x3
	mov	x2, x26
	bl	memmove
	mov	x3, x0
.L62:
	ldp	x1, x0, [sp, 128]
	add	x3, x3, x26
	str	x3, [sp, 168]
	stp	xzr, xzr, [sp, 192]
	str	xzr, [sp, 208]
	subs	x25, x0, x1
	beq	.L82
	mov	x0, 9223372036854775804
	cmp	x25, x0
	bhi	.L117
	mov	x0, x25
.LEHB6:
	bl	_Znwm
.LEHE6:
	mov	x3, x0
	ldp	x1, x0, [sp, 128]
	sub	x26, x0, x1
.L63:
	dup	v0.2d, x3
	add	x25, x3, x25
	str	x25, [sp, 208]
	cmp	x1, x0
	str	q0, [sp, 192]
	beq	.L65
	mov	x0, x3
	mov	x2, x26
	bl	memmove
	mov	x3, x0
.L65:
	add	x3, x3, x26
	mov	w2, w22
	add	x1, sp, 192
	add	x0, sp, 160
	str	x3, [sp, 200]
.LEHB7:
	bl	_Z5benchSt6vectorIfSaIfEES1_i
.LEHE7:
	ldr	x0, [sp, 192]
	fadd	d8, d8, d0
	cbz	x0, .L66
	ldr	x1, [sp, 208]
	sub	x1, x1, x0
	bl	_ZdlPvm
.L66:
	ldr	x0, [sp, 160]
	cbz	x0, .L67
	ldr	x1, [sp, 176]
	sub	x1, x1, x0
	bl	_ZdlPvm
.L67:
	mov	x1, x23
	mov	x0, x20
	mov	x2, 1
.LEHB8:
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	add	w19, w19, 1
	mov	x0, x20
	mov	w1, w19
	bl	_ZNSolsEi
	adrp	x1, .LC2
	mov	x25, x0
	add	x1, x1, :lo12:.LC2
	mov	x2, 1
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	mov	w1, w21
	mov	x0, x25
	bl	_ZNSolsEi
	bl	_ZNSo5flushEv
.L53:
	cmp	w19, w21
	bge	.L59
	ldp	x1, x0, [sp, 96]
	stp	xzr, xzr, [sp, 160]
	str	xzr, [sp, 176]
	subs	x25, x0, x1
	bne	.L118
	mov	x26, 0
	mov	x3, 0
	b	.L60
	.p2align 2,,3
.L82:
	mov	x26, 0
	mov	x3, 0
	b	.L63
	.p2align 2,,3
.L59:
	adrp	x19, _ZSt4cout
	add	x19, x19, :lo12:_ZSt4cout
	adrp	x1, .LC3
	mov	x0, x19
	add	x1, x1, :lo12:.LC3
	mov	x2, 12
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	scvtf	d0, w24
	mov	x0, x19
	fdiv	d0, d8, d0
	bl	_ZNSo9_M_insertIdEERSoT_
	mov	x19, x0
	ldr	x0, [x0]
	ldr	x0, [x0, -24]
	add	x0, x19, x0
	ldr	x20, [x0, 240]
	cbz	x20, .L119
	ldrb	w0, [x20, 56]
	cbz	w0, .L69
	ldrb	w1, [x20, 67]
.L70:
	mov	x0, x19
	bl	_ZNSo3putEc
	bl	_ZNSo5flushEv
	ldr	x0, [sp, 128]
	cbz	x0, .L71
	ldr	x1, [sp, 144]
	sub	x1, x1, x0
	bl	_ZdlPvm
.L71:
	ldr	x0, [sp, 96]
	cbz	x0, .L94
	ldr	x1, [sp, 112]
	sub	x1, x1, x0
	bl	_ZdlPvm
.L94:
	mov	w0, 0
	ldr	d8, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 224
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_restore 72
	.cfi_def_cfa_offset 0
	ret
.L69:
	.cfi_restore_state
	mov	x0, x20
	bl	_ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x2, [x20]
	mov	x0, x20
	mov	w1, 10
	ldr	x2, [x2, 48]
	blr	x2
.LEHE8:
	and	w1, w0, 255
	b	.L70
.L117:
.LEHB9:
	bl	_ZSt17__throw_bad_allocv
.LEHE9:
.L116:
.LEHB10:
	bl	_ZSt17__throw_bad_allocv
.L119:
	bl	_ZSt16__throw_bad_castv
.LEHE10:
.L85:
	ldr	x1, [sp, 192]
	mov	x19, x0
	cbz	x1, .L75
	ldr	x2, [sp, 208]
	mov	x0, x1
	sub	x1, x2, x1
	bl	_ZdlPvm
.L75:
	ldr	x0, [sp, 160]
	cbz	x0, .L77
	ldr	x1, [sp, 176]
	sub	x1, x1, x0
	bl	_ZdlPvm
.L77:
	ldr	x0, [sp, 128]
	cbz	x0, .L78
	ldr	x1, [sp, 144]
	sub	x1, x1, x0
	bl	_ZdlPvm
.L78:
	ldr	x0, [sp, 96]
	cbz	x0, .L79
	ldr	x1, [sp, 112]
	sub	x1, x1, x0
	bl	_ZdlPvm
.L79:
	mov	x0, x19
.LEHB11:
	bl	_Unwind_Resume
.LEHE11:
.L84:
	mov	x19, x0
	b	.L75
.L83:
	mov	x19, x0
	b	.L77
	.cfi_endproc
.LFE2224:
	.section	.gcc_except_table
.LLSDA2224:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2224-.LLSDACSB2224
.LLSDACSB2224:
	.uleb128 .LEHB5-.LFB2224
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L83-.LFB2224
	.uleb128 0
	.uleb128 .LEHB6-.LFB2224
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L84-.LFB2224
	.uleb128 0
	.uleb128 .LEHB7-.LFB2224
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L85-.LFB2224
	.uleb128 0
	.uleb128 .LEHB8-.LFB2224
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L83-.LFB2224
	.uleb128 0
	.uleb128 .LEHB9-.LFB2224
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L84-.LFB2224
	.uleb128 0
	.uleb128 .LEHB10-.LFB2224
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L83-.LFB2224
	.uleb128 0
	.uleb128 .LEHB11-.LFB2224
	.uleb128 .LEHE11-.LEHB11
	.uleb128 0
	.uleb128 0
.LLSDACSE2224:
	.section	.text.startup
	.size	main, .-main
	.align	2
	.p2align 4,,11
	.type	_GLOBAL__sub_I__Z7sv_maddSt6vectorIfSaIfEES1_, %function
_GLOBAL__sub_I__Z7sv_maddSt6vectorIfSaIfEES1_:
.LFB2880:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	str	x19, [sp, 16]
	.cfi_offset 19, -16
	adrp	x19, .LANCHOR0
	add	x19, x19, :lo12:.LANCHOR0
	mov	x0, x19
	bl	_ZNSt8ios_base4InitC1Ev
	mov	x1, x19
	adrp	x2, __dso_handle
	ldr	x19, [sp, 16]
	add	x2, x2, :lo12:__dso_handle
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_def_cfa_offset 0
	adrp	x0, _ZNSt8ios_base4InitD1Ev
	add	x0, x0, :lo12:_ZNSt8ios_base4InitD1Ev
	b	__cxa_atexit
	.cfi_endproc
.LFE2880:
	.size	_GLOBAL__sub_I__Z7sv_maddSt6vectorIfSaIfEES1_, .-_GLOBAL__sub_I__Z7sv_maddSt6vectorIfSaIfEES1_
	.section	.init_array,"aw",%init_array
	.align	3
	.xword	_GLOBAL__sub_I__Z7sv_maddSt6vectorIfSaIfEES1_
	.bss
	.align	3
	.set	.LANCHOR0,. + 0
	.type	_ZStL8__ioinit, %object
	.size	_ZStL8__ioinit, 1
_ZStL8__ioinit:
	.zero	1
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align	3
	.type	DW.ref.__gxx_personality_v0, %object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.xword	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 10.0.0 20191104 (experimental)"
	.section	.note.GNU-stack,"",@progbits
