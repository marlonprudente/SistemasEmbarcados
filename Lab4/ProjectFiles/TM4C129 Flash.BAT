SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\Git\cmd;C:\Program Files (x86)\GitExtensions\;C:\Program Files\PuTTY\;C:\Users\User\AppData\Local\Microsoft\WindowsApps;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=TM4C1294NCPDT
SET CPU_VENDOR=Texas Instruments
SET UV2_TARGET=TM4C129 Flash
SET CPU_CLOCK=0x07270E00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\exemplotiva.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\uart_console_f.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\accel.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\buzzer.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\buttons.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\cfaf128x128x16.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\mic.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\opt.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\rgb.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\servo.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\joy.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\temp.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\uart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\pwm.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\ondas.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\rtx_conf_cm.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\flash\startup_tm4c129._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash\system_tm4c129.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Flash\base_project.lnp"
