Dev environment setup for SmartFusion2:

- Install LiberoSoc 11.7
- Install LiberoSoc 11.7_sp3 or whatever latest version 
(note: microsemi sp are incremental updates to base verion. Hence base version needs to be istalled first)

- Install Programmer_debug_11.7
- Install Programmer_debug_11.7.sp2

- Install Microsemi-softconsole-v4.0.0.13

Checkout SmartFusion2 branch from MF



SF2 Familiy Complete Memory Address Map:

* Some current (April 2015) dev kit memory map possibilities are
 * --Type-------Device-----------address start---address end----size---Dbus--RAM IC-------SF2--Comment---------------
 * --eNVM-------M2S010-----------0x60000000------0x6007FFFF-----256KB---------------------010------------------------
 * --eNVM-------M2S090-----------0x60000000------0x6007FFFF-----512KB---------------------090------------------------
 * --eSRAM------M2Sxxx-----------0x20000000------0x2000FFFF-----64KB----------------------xxx--All have same amount--
 * --eSRAM------M2Sxxx-----------0x20000000------0x20013FFF-----80KB----------------------xxx--If ECC/SECDED not used
 * --Fabric-----M2S010-----------0x30000000------0x6007FFFF-----400Kb---------------------010--note-K bits-----------
 * --Fabric-----M2S090-----------0x30000000------0x6007FFFF-----2074Kb--------------------090--note-K bits-----------
 * --LPDDR------STARTER-KIT------0xA0000000------0xA3FFFFFF-----64MB---16--MT46H32M16-----050------------------------
 * --LPDDR------484-STARTER-KIT--0xA0000000------0xA3FFFFFF-----64MB---16--MT46H32M16-----010------------------------
 * --LPDDR------SEC-EVAL-KIT-----0xA0000000------0xA3FFFFFF-----64MB---16--MT46H32M16LF---090--Security eval kit-----
 * --DDR3-------ADevKit----------0xA0000000------0xBFFFFFFF-----1GB----32--MT41K256M8DA---150------------------------
 * --Some older physical memory map possibilities are
 * --Type-------location---------address start---address end----size---Dbus---RAM IC------SF2--Comment--------------
 * --LPDDR------EVAL KIT---------0xA0000000------0xA3FFFFFF-----64MB-=-16--MT46H32M16LF---025--Eval Kit--------------
 * --DDR3-------DevKit-----------0xA0000000------0xAFFFFFFF-----512MB--16--MT41K256M8DA---050------------------------ 


FAQ:

1.Is there any more RAM than the 64KB eSRAM on the M3?
The fpga fabric has about ~260KB or 2074Kb of LSRAM interspersed. This can be accessed from M3, but needs careful planning with the fpga binary being loaded. So potentially, this is another RAM area that can be used.

2. when we do flash freeze is LSRAM lost?
Both the eSRAM and LSRAM are retained. but no modification possible until exit from freeze.

3. what is latency of using RAM ?
one cycle delay for both read and write in non-pipelined mode. pipelined read there is no delay, output is available for next instruction. (which ram are we talking about here, LS or eS?)

4. SECDED/non-SECDED?
if you use in dont use it SECDED mode you get 80K, if you use SECDED mode you get 64K. Its the same physical address space.