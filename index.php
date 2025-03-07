<?php

// This is the initial gathering of the data regarding the COA project
// Assembler, which is for simple risc
// Simple Risc properties

$header = <<<'EOT'
<!DOCTYPE>
<html>
<head>
<title>Assembler for simple risc</title>
<link rel="stylesheet" href="main.css" type="text/css">
<meta http-equiv="author" content="sai">
</head>
EOT;

$body_start = <<<'EOT'

<body>
	<h1>Simple Risc</h1>
	<h3>Basic Characteristics</h3>
	<ol>
	<li>It has 32-bit Instrunction register</li>
	<li>It has 21 instrunctions</li>
	<li>It has 2 flag bits</li>
	<li>It has 16 register (r0-r15)</li>
	<li>It has mainly two Data Instruction</li>
	<ul>
	<li>Register (R) Type Instruction</li>
	<li>Immediate (I) type instruction</li>
	</ul>
	<li>It can take at max three operands for one instruction</li>
	<ul>
	<li>One Destination Register and two Source Register</li>
	<li>One Destination Register and One Source Register</li>
	<li>One Destination Register and One immediate</li>
	<li>One Destination Register and One Source Register</li>
	<li>One Destination Register and One source Register and One Immediate</li>
	<li>No Operand</li>
	</ul>
	<li>The immediate instruction can have modifiers like u,h</li>
	</ol>
EOT;
require "table_opcode.php";
$body_mid = <<<'EOT'
<h2>Instruction Encoding</h3>
<p>These are the rules that have to be followed inorder to encode the instructions</p>
<p>The Instruction Register is 32bits</p>
<ol>
<li>The Opcode takes 5bits at the MSB of the instruction Register</li>
<li>The 6th MSB takes R/I bit value>
<ul>
<li>6th MSB is 0, meaning Register Type instrucntion, the format is 7th - 10th MSB takes the Destinatioin Register value, and The next four bits for first Source register and the next four for the second Register</p>
<li>6th MSB is 1, meaning Immediate type instruction, the format is 7th - 10th MSB is Destination Registerm, the Next 4 bits for the source Register, and next 18 bits for the Immediate value, and in these 18bits, first two bits are for the modifier and the last 16bits for the immediate value</li>
<p>Modifier Values and there significance
<table>
<tr>
    <th>15th MSB</th>
    <th>16th MSB</th>
    <th>Meaning</th>
</tr>
<tr>
<td>0</td>
<td>0</td>
<td>Default(2's complement)</td>
</tr>
<tr>
<td>0</td>
<td>1</td>
<td>u(unsigned)<td>
</tr><tr>
<td>1</td>
<td>0</td>
<td>h(higher order)<td>
</tr>

</table>
</ul>
<li>The 4bit value taken by registers is there own register number, meaning r1 - 0001, r15 - 1111</li>
</ol>
<h4>0-Address Instruction Encoding Format</h4>
Instructions:- <i>nop, ret </i>
<p>The first five bits are for the Opcode</p>
<p>The next 27 bits are empty meaning 0</p>
<h4>1-Address Instruction Encdoing</h4>
Instructions:- <i>call, b, beq, bgt</i>
<p>The first five bits are for the Opcode</p>
<p>The next 27 bits are for the relative address</p>
<p>Meaning that PC(program counter) + offset*4, where offset is the 27bits immediate value</p>
<h4>3-Address Instruction Encoding Format</h4>
Instructions:- <i>add, sub, mul, div, mod, and, or, lsl, lsr, asr</i>
<p>The first five bits for the opcode</p>
<p>The 6th bit to specify whether instruction is R-Type or I-Type</p>
<p>If R-Type, 6th bit will be 0, then 7th - 10th bit will be Rd, 11th-14th will be Rs1 and 15th-19th will Rs2</p>
<p>here Rd will be Destination Register, Rs1 is first source register and Rs2 is second source register</p>
<p>If I-Type, 6th bit will be 1, then 7th - 10th bit will be Rd, 11th-14th bit will be Rs1 and 15th - 16th bit will be modifier, and next 16 bits will be immediate value</p>
<h4>2-Address Instructions Encoding format</h4>
Instructions: <i>cmp, not, mov</i>
<p>The first five bits will be opcode</p>
<p>For not and mov, the Rs1 place holder that is 11th -14th will be zero, for both R-type and I-type, in R-type Rd and Rs2 will be used, and in I-Type Rd and Immediate(last 18bits will be used)</p>
<p>For cmp, this opcode affects the flag, and influences the branch conditionals instructions, and the result of this is not stored in any destination register, hence only Rs1 and Rs2 will be used for R-Type, where I-Type it uses Rs1 and Immediate (last 18bits), now Rd will be zero for both cases</p>
<h4>ld and st instruction encoding</h4>
instruction:- <i>ld Rd, imm[rs1]</i>
<p>The format for this ld and st is, the first five bits are opcode, and Rd will be 7th to 10th bits, Rs1 will be 11th - 14th bits, and last 18 bits for the immediate value including modifier</p>
EOT;

$body_main = <<<'EOT'
<pre>
Things To take care of:
0.Relative Addressing and ORG
1. Valid Opcode Verification
2. Valid operands for corresponding opcodes verificatrion
3. Mapping operands with there codes
4. Valid registers addressing modes
5. Modifiers considersation
6. Labels considerations, mapping with instruction address
7. Generation of binary file and then hexa file


Primary build up:
0. Verify the limit of immediate value, in MOV it can be 32bits 8hex but in other immediate it has to 16bits meaning 4hex
1. Create a function to convert decimal to binary
2. Create a function to convert hexa to binary
3. Create a mapping criteria for instruction line and Labels
4. Create a function to identify whether it is r or t type
5. Create a thing to tokenise thaem

Rules
1. The immediate value to be passed should be written in lower characters only
</pre>
EOT;
$body_end = "</body>";
$end_tag = "</html>";
echo $header .
    $body_start .
    $table .
    $body_mid .
    $body_main .
    $table1 .
    $body_end .
    $end_tag;
