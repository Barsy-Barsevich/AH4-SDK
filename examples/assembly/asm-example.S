
.globl task, mysmul
.type task, @function
.type mysmul, @function

.section .text


// Software multiplication "32*32=32"
// a0 = a0*a1
mysmul:
	addi	sp,sp,-12
	sw	t0,0(sp)
	sw	t1,4(sp)
	sw	t2,8(sp)
	li	t0,1
	li	t1,0
	j	_mysmul_2
_mysmul_1:
	slli	t0,t0,1
	beq	t0,zero,_mysmul_end
	slli	a1,a1,1
_mysmul_2:
	and	t2,t0,a0
	beq	t2,zero,_mysmul_1
	add	t1,t1,a1
	j	_mysmul_1
_mysmul_end:
	mv	a0,t1
	lw	t0,0(sp)
	lw	t1,4(sp)
	lw	t2,8(sp)
	addi	sp,sp,12
	ret


// Simple mathematical function
// sw == a0
// out == a0 (according RISC-V specification)
//
// out == (k*a + b) + c
// k == sw[15:12]
// a == sw[11:8]
// b == sw[7:4]
// c == sw[3:0]

task:
	// store registers values
	addi	sp,sp,-12
	sw	t0,0(sp)
	sw	t1,4(sp)
	sw	t2,8(sp)

// (1) t0 <= k; t1 <= a
	lbu	t0,1(a0)
	andi	t1,t0,0x0F
	srai	t0,t0,4
// (2) t1 <= k*a
	mv	t2,a0
	mv	a0,t1
	mv	a1,t0
	mv	t0,ra
	jal	mysmul
	mv	ra,t0
	mv	t1,a0
	mv	a0,t2
	//mul	t1,t1,t0
// (3) t0 <= b; t1 <= c
	lbu	t0,0(a0)
	andi	t2,t0,0x0F
	srai	t0,t0,4
// (4) a0 <= k*a+b+c
	add	t1,t1,t0
	add	a0,t1,t2

	// load registers
	lw	t0,0(sp)
	lw	t1,4(sp)
	lw	t2,4(sp)
	addi	sp,sp,12
	ret

