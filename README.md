## About

Very simple compiler for a small Rust programming language subset:

    - Integers and Booleans (0/1)
    - let / let mut (assignments)
    - if-then / if-then-else statements
    - while statements
    - logical operators (&&, ||)
    - relational operators (==, !=, etc.)
    - arithmetic expressions
    - only one function fn main()
  
## Compile: 

`$ make clean all`

## Iterative Euclid algorithm for Greatest Common Divisor:

For example:

`$ ./compiler tests/euclidean.txt`

## Example Output

```
tests/euclidean.txt

====================
SOURCE CODE
====================
// Euclids GCD Algorithm

fn main() {
  let a = 21;
  let b = 105;

  // println!("Please insert value of 'a': ");
  read_line(a);

  // println!("Please insert value of 'b': ");
  read_line(b);

  while b != 0 {
    t = b;
  	b = a % b;
  	a = t;
  }

  // println!("The result is: ");
  println!(a);
  // println!(b);
}
====================
ABSTRACT SYNTAX TREE
====================
<main>
    <assignment>
        <variable a>
        <number 21>
    <assignment>
        <variable b>
        <number 105>
    <read_line>
        <symbol a>
    <read_line>
        <symbol b>
    <while_statement>
        <cond>
            <notequal>
                <symbol b = 105>
                <number 0>
        <then>
            <assignment>
                <variable t>
                <symbol b = 105>
            <assignment>
                <variable b>
                <mod>
                    <symbol a = 21>
                    <symbol b = 105>
            <assignment>
                <variable a>
                <symbol t = 105>
    <println!>
        <symbol a>
====================
THREE-ADDRESS CODE (IR)
====================
_main:
	t0 := 21
	s0 := t0
	t0 := 105
	s1 := t0
	READ s0
	READ s1
_L0:
	t0 := s1
	t1 := 0
	IF t0 != t1 GOTO _L1
	GOTO _L2
_L1:
	t2 := s1
	s2 := t2
	t1 := s0
	t2 := s1
	t0 := t1 % t2
	s1 := t0
	t0 := s2
	s0 := t0
	GOTO _L0
_L2:
	PRINT s0
	EXIT
====================
MIPS ASSEMBLY
====================
	.text
_main:
	li $t0, 21
	move $s0, $t0
	li $t0, 105
	move $s1, $t0
	li $v0, 5
	syscall
	add $s0, $v0, $zero
	li $v0, 5
	syscall
	add $s1, $v0, $zero
_L0:
	move $t0, $s1
	li $t1, 0
	bne $t0, $t1, _L1
	j _L2
_L1:
	move $t2, $s1
	move $s2, $t2
	move $t1, $s0
	move $t2, $s1
	rem $t0, $t1, $t2
	move $s1, $t0
	move $t0, $s2
	move $s0, $t0
	j _L0
_L2:
	li $v0, 1
	add $a0, $s0, $zero
	syscall
	li $v0, 10
	syscall

[+] MIPS assembly generated to file: 'mips_program.asm'
```


