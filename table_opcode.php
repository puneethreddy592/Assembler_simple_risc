<?php

$table = <<<'EOT'
<table>
	<tr>
	<th>Type of Operation</th>
	<th>Opcode</th>
	<th>Max No of Operands</th>
	<th>Code</th>
	<th>Code in decimal</th>
	</tr>
	<tr>
    <td>Data Transfer/Movement</td>
    <td>mov</td>
    <td></td>
    <td>01001</td>
    <td>9</td>
</tr>
<tr>
    <td>Data Transfer/Movement</td>
    <td>ld</td>
    <td></td>
    <td>01110</td>
    <td>14</td>
</tr>
<tr>
    <td>Data Transfer/Movement</td>
    <td>st</td>
    <td></td>
    <td>01111</td>
    <td>15</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>add</td>
    <td></td>
    <td>00000</td>
    <td>0</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>sub</td>
    <td></td>
    <td>00001</td>
    <td>1</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>mul</td>
    <td></td>
    <td>00010</td>
    <td>2</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>div</td>
    <td></td>
    <td>00011</td>
    <td>3</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>mod</td>
    <td></td>
    <td>00100</td>
    <td>4</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>cmp</td>
    <td></td>
    <td>00101</td>
    <td>5</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>and</td>
    <td></td>
    <td>00110</td>
    <td>6</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>or</td>
    <td></td>
    <td>00111</td>
    <td>7</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>not</td>
    <td></td>
    <td>01000</td>
    <td>8</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>lsl</td>
    <td></td>
    <td>01010</td>
    <td>10</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>lsr</td>
    <td></td>
    <td>01011</td>
    <td>11</td>
</tr>
<tr>
    <td>Data Processing</td>
    <td>asr</td>
    <td></td>
    <td>01100</td>
    <td>12</td>
</tr>
<tr>
    <td>Branch</td>
    <td>b</td>
    <td></td>
    <td>10010</td>
    <td>18</td>
</tr>
<tr>
    <td>Branch</td>
    <td>call</td>
    <td></td>
    <td>10011</td>
    <td>19</td>
</tr>
<tr>
    <td>Branch</td>
    <td>ret</td>
    <td></td>
    <td>10100</td>
    <td>20</td>
</tr>
<tr>
    <td>Branch</td>
    <td>beq</td>
    <td></td>
    <td>10000</td>
    <td>16</td>
</tr>
<tr>
    <td>Branch</td>
    <td>bgt</td>
    <td></td>
    <td>10001</td>
    <td>17</td>
</tr>
<tr>
    <td>Special</td>
    <td>nop</td>
    <td></td>
    <td>01101</td>
    <td>13</td>
</tr>
</table>
EOT;

$table1 = <<<'EOT'
<table border="1">
    <tr>
        <th>Inst.</th>
        <th>Code</th>
        <th>Format</th>
        <th>Inst.</th>
        <th>Code</th>
        <th>Format</th>
    </tr>
    <tr>
        <td>add</td>
        <td>00000</td>
        <td>add rd, rs1, (rs2/imm)</td>
        <td>lsl</td>
        <td>01010</td>
        <td>lsl rd, rs1, (rs2/imm)</td>
    </tr>
    <tr>
        <td>sub</td>
        <td>00001</td>
        <td>sub rd, rs1, (rs2/imm)</td>
        <td>lsr</td>
        <td>01011</td>
        <td>lsr rd, rs1, (rs2/imm)</td>
    </tr>
    <tr>
        <td>mul</td>
        <td>00010</td>
        <td>mul rd, rs1, (rs2/imm)</td>
        <td>asr</td>
        <td>01100</td>
        <td>asr rd, rs1, (rs2/imm)</td>
    </tr>
    <tr>
        <td>div</td>
        <td>00011</td>
        <td>div rd, rs1, (rs2/imm)</td>
        <td>nop</td>
        <td>01101</td>
        <td>nop</td>
    </tr>
    <tr>
        <td>mod</td>
        <td>00100</td>
        <td>mod rd, rs1, (rs2/imm)</td>
        <td>ld</td>
        <td>01110</td>
        <td>ld rd.imm[rs1]</td>
    </tr>
    <tr>
        <td>cmp</td>
        <td>00101</td>
        <td>cmp rs1, (rs2/imm)</td>
        <td>st</td>
        <td>01111</td>
        <td>st rd.imm[rs1]</td>
    </tr>
    <tr>
        <td>and</td>
        <td>00110</td>
        <td>and rd, rs1, (rs2/imm)</td>
        <td>beq</td>
        <td>10000</td>
        <td>beq offset</td>
    </tr>
    <tr>
        <td>or</td>
        <td>00111</td>
        <td>or rd, rs1, (rs2/imm)</td>
        <td>bgt</td>
        <td>10001</td>
        <td>bgt offset</td>
    </tr>
    <tr>
        <td>not</td>
        <td>01000</td>
        <td>not rd, (rs2/imm)</td>
        <td>b</td>
        <td>10010</td>
        <td>b offset</td>
    </tr>
    <tr>
        <td>mov</td>
        <td>01001</td>
        <td>mov rd, (rs2/imm)</td>
        <td>call</td>
        <td>10011</td>
        <td>call offset</td>
    </tr>
    <tr>
        <td></td>
        <td></td>
        <td></td>
        <td>ret</td>
        <td>10100</td>
        <td>ret</td>
    </tr>
</table>

EOT;
?>
