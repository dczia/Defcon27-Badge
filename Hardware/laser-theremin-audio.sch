EESchema Schematic File Version 4
LIBS:laser-theremin-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DCZia_audio_amp:Amplifier_Audio_LM4861 U6
U 1 1 5D01FDA2
P 4055 2880
F 0 "U6" H 4205 3395 50  0000 C CNN
F 1 "LM4861" H 4205 3304 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 4055 2880 50  0001 C CNN
F 3 "" H 4055 2880 50  0001 C CNN
	1    4055 2880
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 5D01FDA8
P 2485 2480
F 0 "R13" H 2355 2480 50  0000 C CNN
F 1 "10k" V 2485 2485 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2415 2480 50  0001 C CNN
F 3 "~" H 2485 2480 50  0001 C CNN
	1    2485 2480
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5D01FDB0
P 2485 2330
AR Path="/5D01FDB0" Ref="#PWR?"  Part="1" 
AR Path="/5D01A7D6/5D01FDB0" Ref="#PWR0116"  Part="1" 
F 0 "#PWR0116" H 2485 2180 50  0001 C CNN
F 1 "+3V3" H 2500 2503 50  0000 C CNN
F 2 "" H 2485 2330 50  0001 C CNN
F 3 "" H 2485 2330 50  0001 C CNN
	1    2485 2330
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5D01FDB6
P 4655 2730
F 0 "#PWR0117" H 4655 2480 50  0001 C CNN
F 1 "GND" V 4650 2535 50  0000 C CNN
F 2 "" H 4655 2730 50  0001 C CNN
F 3 "" H 4655 2730 50  0001 C CNN
	1    4655 2730
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5D01FDBC
P 4215 3135
AR Path="/5D01FDBC" Ref="R?"  Part="1" 
AR Path="/5D01A7D6/5D01FDBC" Ref="R31"  Part="1" 
F 0 "R31" V 4125 3135 50  0000 C CNN
F 1 "20k" V 4210 3140 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4145 3135 50  0001 C CNN
F 3 "~" H 4215 3135 50  0001 C CNN
	1    4215 3135
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4655 3135 4365 3135
Wire Wire Line
	4065 3135 3755 3135
Wire Wire Line
	3755 3135 3755 2930
$Comp
L Device:C_Small C16
U 1 1 5D01FDC5
P 5525 2790
F 0 "C16" H 5630 2835 50  0000 L CNN
F 1 "0.1uF" H 5630 2765 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5525 2790 50  0001 C CNN
F 3 "~" H 5525 2790 50  0001 C CNN
	1    5525 2790
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C15
U 1 1 5D01FDCB
P 3300 2730
F 0 "C15" V 3185 2765 50  0000 C CNN
F 1 "1uF" V 3185 2610 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3300 2730 50  0001 C CNN
F 3 "~" H 3300 2730 50  0001 C CNN
	1    3300 2730
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3755 2730 3600 2730
$Comp
L Device:C_Small C?
U 1 1 5D01FDD8
P 2970 2930
AR Path="/5D01FDD8" Ref="C?"  Part="1" 
AR Path="/5D01A7D6/5D01FDD8" Ref="C14"  Part="1" 
F 0 "C14" V 2850 2930 50  0000 C CNN
F 1 "0.33uF" V 2780 2930 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 2970 2930 50  0001 C CNN
F 3 "~" H 2970 2930 50  0001 C CNN
	1    2970 2930
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3600 2830 3755 2830
Wire Wire Line
	3600 2830 3600 2730
$Comp
L Device:R R20
U 1 1 5D01FDE2
P 3605 2930
F 0 "R20" V 3515 2930 50  0000 C CNN
F 1 "20k" V 3600 2935 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 3535 2930 50  0001 C CNN
F 3 "~" H 3605 2930 50  0001 C CNN
	1    3605 2930
	0    -1   -1   0   
$EndComp
Connection ~ 3755 2930
$Comp
L Device:C_Small C11
U 1 1 5D01FDE9
P 2720 3140
F 0 "C11" H 2540 3180 50  0000 C CNN
F 1 "10nF" H 2555 3110 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 2720 3140 50  0001 C CNN
F 3 "~" H 2720 3140 50  0001 C CNN
	1    2720 3140
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0118
U 1 1 5D01FDF0
P 4655 2830
F 0 "#PWR0118" H 4655 2680 50  0001 C CNN
F 1 "+3V3" V 4655 3040 50  0000 C CNN
F 2 "" H 4655 2830 50  0001 C CNN
F 3 "" H 4655 2830 50  0001 C CNN
	1    4655 2830
	0    1    1    0   
$EndComp
Wire Wire Line
	4655 2930 4655 3135
$Comp
L power:+3V3 #PWR0119
U 1 1 5D01FDF7
P 5525 2690
F 0 "#PWR0119" H 5525 2540 50  0001 C CNN
F 1 "+3V3" H 5515 2835 50  0000 C CNN
F 2 "" H 5525 2690 50  0001 C CNN
F 3 "" H 5525 2690 50  0001 C CNN
	1    5525 2690
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D01FDFD
P 5525 2890
AR Path="/5D01FDFD" Ref="#PWR?"  Part="1" 
AR Path="/5D01A7D6/5D01FDFD" Ref="#PWR0120"  Part="1" 
F 0 "#PWR0120" H 5525 2640 50  0001 C CNN
F 1 "GND" H 5525 2745 50  0000 C CNN
F 2 "" H 5525 2890 50  0001 C CNN
F 3 "" H 5525 2890 50  0001 C CNN
	1    5525 2890
	1    0    0    -1  
$EndComp
Text Notes 8725 6890 0    50   ~ 10
mediumrehr Audio
Text Notes 9405 6850 0    25   ~ 5
TM
$Comp
L Device:R R12
U 1 1 5D01FE0E
P 2430 2930
F 0 "R12" V 2340 2930 50  0000 C CNN
F 1 "1k" V 2425 2935 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2360 2930 50  0001 C CNN
F 3 "~" H 2430 2930 50  0001 C CNN
	1    2430 2930
	0    1    1    0   
$EndComp
$Comp
L OPL_Connector:AUDIO-JACK-SMD-8P_PJ-3536-L6S_ J?
U 1 1 5D01FE1A
P 4835 4125
AR Path="/5D01FE1A" Ref="J?"  Part="1" 
AR Path="/5D01A7D6/5D01FE1A" Ref="J4"  Part="1" 
F 0 "J4" H 4875 3655 50  0000 R CNN
F 1 "Audio Out" H 4875 3730 50  0000 R CNN
F 2 "OPL_Connector:AUDIO8P-SMD-3.5-14.5X6X5MM" H 5085 4225 50  0001 C CNN
F 3 "~" H 5085 4225 50  0001 C CNN
	1    4835 4125
	-1   0    0    -1  
$EndComp
Connection ~ 4655 2930
Wire Wire Line
	4655 2630 5105 2630
Wire Wire Line
	4655 2930 5105 2930
Text GLabel 5105 2630 2    50   Input ~ 0
A+
Text GLabel 5105 2930 2    50   Input ~ 0
A-
$Comp
L Connector_Generic:Conn_01x01 J?
U 1 1 5D041E5B
P 8065 3560
AR Path="/5D041E5B" Ref="J?"  Part="1" 
AR Path="/5D01A7D6/5D041E5B" Ref="J6"  Part="1" 
F 0 "J6" H 8225 3565 50  0000 R CNN
F 1 "Conn_01x01" V 7938 3472 50  0001 R CNN
F 2 "TestPoint:TestPoint_Pad_2.5x2.5mm" H 8065 3560 50  0001 C CNN
F 3 "~" H 8065 3560 50  0001 C CNN
	1    8065 3560
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J?
U 1 1 5D041E61
P 8065 4160
AR Path="/5D041E61" Ref="J?"  Part="1" 
AR Path="/5D01A7D6/5D041E61" Ref="J8"  Part="1" 
F 0 "J8" H 8225 4165 50  0000 R CNN
F 1 "Conn_01x01" V 7938 4072 50  0001 R CNN
F 2 "TestPoint:TestPoint_Pad_2.5x2.5mm" H 8065 4160 50  0001 C CNN
F 3 "~" H 8065 4160 50  0001 C CNN
	1    8065 4160
	1    0    0    -1  
$EndComp
$Comp
L Device:Speaker LS?
U 1 1 5D041E6A
P 8290 3810
AR Path="/5D041E6A" Ref="LS?"  Part="1" 
AR Path="/5D01A7D6/5D041E6A" Ref="LS1"  Part="1" 
F 0 "LS1" H 8595 3585 50  0000 R CNN
F 1 "Speaker" H 8765 3650 50  0000 R CNN
F 2 "lib_fp:Speaker" H 8290 3610 50  0001 C CNN
F 3 "~" H 8280 3760 50  0001 C CNN
	1    8290 3810
	1    0    0    -1  
$EndComp
Wire Wire Line
	8090 3810 7865 3810
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 5D05D238
P 8375 2995
AR Path="/5D05D238" Ref="J?"  Part="1" 
AR Path="/5D01A7D6/5D05D238" Ref="J9"  Part="1" 
F 0 "J9" H 8380 2710 50  0000 C CNN
F 1 "Rack1" V 8460 2945 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8375 2995 50  0001 C CNN
F 3 "~" H 8375 2995 50  0001 C CNN
	1    8375 2995
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 5D05D23E
P 8065 2895
AR Path="/5D05D23E" Ref="J?"  Part="1" 
AR Path="/5D01A7D6/5D05D23E" Ref="J5"  Part="1" 
F 0 "J5" H 8065 3085 50  0000 C CNN
F 1 "Rack2" V 8155 2845 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8065 2895 50  0001 C CNN
F 3 "~" H 8065 2895 50  0001 C CNN
	1    8065 2895
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D?
U 1 1 5D05D244
P 8715 2995
AR Path="/5D05D244" Ref="D?"  Part="1" 
AR Path="/5D01A7D6/5D05D244" Ref="D12"  Part="1" 
F 0 "D12" H 8705 2900 50  0000 C CNN
F 1 "D_Schottky" H 8715 3120 50  0001 C CNN
F 2 "lib_fp:SOD-123" H 8715 2995 50  0001 C CNN
F 3 "~" H 8715 2995 50  0001 C CNN
	1    8715 2995
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D05D24B
P 8575 3095
AR Path="/5D05D24B" Ref="#PWR?"  Part="1" 
AR Path="/5D01A7D6/5D05D24B" Ref="#PWR0121"  Part="1" 
F 0 "#PWR0121" H 8575 2845 50  0001 C CNN
F 1 "GND" H 8575 2955 50  0000 C CNN
F 2 "" H 8575 3095 50  0001 C CNN
F 3 "" H 8575 3095 50  0001 C CNN
	1    8575 3095
	1    0    0    -1  
$EndComp
Text GLabel 7715 3095 0    50   Input ~ 0
A+
Wire Wire Line
	7715 3095 7865 3095
Text GLabel 7715 2795 0    50   Input ~ 0
A-
Wire Wire Line
	7715 2795 7865 2795
$Comp
L power:GND #PWR?
U 1 1 5D05D255
P 7825 2945
AR Path="/5D05D255" Ref="#PWR?"  Part="1" 
AR Path="/5D01A7D6/5D05D255" Ref="#PWR0136"  Part="1" 
F 0 "#PWR0136" H 7825 2695 50  0001 C CNN
F 1 "GND" V 7825 2750 50  0000 C CNN
F 2 "" H 7825 2945 50  0001 C CNN
F 3 "" H 7825 2945 50  0001 C CNN
	1    7825 2945
	0    1    1    0   
$EndComp
Wire Wire Line
	8565 2995 8575 2995
Wire Wire Line
	8575 2795 9075 2795
Wire Wire Line
	7865 2995 7865 2945
Wire Wire Line
	7825 2945 7865 2945
Connection ~ 7865 2945
Wire Wire Line
	7865 2945 7865 2895
Wire Notes Line
	7445 2665 9425 2665
Wire Notes Line
	9425 2665 9425 3285
Wire Notes Line
	9425 3285 7445 3285
Wire Notes Line
	7445 3285 7445 2665
Text Notes 7430 2650 0    50   ~ 10
Eurorack Expansion
Text GLabel 7760 3810 0    50   Input ~ 0
A+
Text GLabel 7760 3910 0    50   Input ~ 0
A-
Wire Wire Line
	7865 3560 7865 3810
Wire Wire Line
	7865 3910 7865 4160
Wire Wire Line
	7865 3910 8090 3910
Wire Wire Line
	7760 3910 7865 3910
Connection ~ 7865 3910
Wire Wire Line
	7760 3810 7865 3810
Connection ~ 7865 3810
Wire Notes Line
	7445 3435 9425 3435
Wire Notes Line
	9425 3435 9425 4295
Wire Notes Line
	9425 4295 7445 4295
Wire Notes Line
	7445 4295 7445 3435
Text Notes 7430 3410 0    50   ~ 10
Speaker
Text Notes 8485 3880 0    50   ~ 0
Note: Speaker is flying \nleads, soldered to pads
Wire Wire Line
	2410 2630 2485 2630
Text HLabel 2410 2630 0    50   Input ~ 0
AUDIO_SHTDN
Text HLabel 2185 2930 0    50   Input ~ 0
PWM_IN
$Comp
L power:GND #PWR0137
U 1 1 5D0C8388
P 2720 3240
F 0 "#PWR0137" H 2720 2990 50  0001 C CNN
F 1 "GND" H 2720 3080 50  0000 C CNN
F 2 "" H 2720 3240 50  0001 C CNN
F 3 "" H 2720 3240 50  0001 C CNN
	1    2720 3240
	1    0    0    -1  
$EndComp
Wire Wire Line
	2580 2930 2720 2930
Wire Wire Line
	2720 3040 2720 2930
Connection ~ 2720 2930
Wire Wire Line
	2720 2930 2870 2930
Wire Wire Line
	2185 2930 2280 2930
Wire Wire Line
	3070 2930 3285 2930
Wire Wire Line
	3400 2730 3600 2730
Connection ~ 3600 2730
$Comp
L power:GND #PWR0138
U 1 1 5D0D0F6E
P 3200 2730
F 0 "#PWR0138" H 3200 2480 50  0001 C CNN
F 1 "GND" V 3200 2520 50  0000 C CNN
F 2 "" H 3200 2730 50  0001 C CNN
F 3 "" H 3200 2730 50  0001 C CNN
	1    3200 2730
	0    1    1    0   
$EndComp
Text HLabel 2325 4375 0    50   Input ~ 0
HP_DETECT
Wire Wire Line
	4335 4175 3285 4175
Wire Wire Line
	3285 4175 3285 3975
Connection ~ 3285 2930
Wire Wire Line
	3285 2930 3455 2930
Connection ~ 3285 3975
Wire Wire Line
	3285 3975 3285 2930
$Comp
L power:GND #PWR0141
U 1 1 5D0DB772
P 4335 4475
F 0 "#PWR0141" H 4335 4225 50  0001 C CNN
F 1 "GND" H 4335 4325 50  0000 C CNN
F 2 "" H 4335 4475 50  0001 C CNN
F 3 "" H 4335 4475 50  0001 C CNN
	1    4335 4475
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5D0E0F17
P 2490 4225
AR Path="/5D0E0F17" Ref="R?"  Part="1" 
AR Path="/5D01A7D6/5D0E0F17" Ref="R14"  Part="1" 
F 0 "R14" H 2360 4225 50  0000 C CNN
F 1 "10k" V 2485 4230 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2420 4225 50  0001 C CNN
F 3 "~" H 2490 4225 50  0001 C CNN
	1    2490 4225
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5D0EA3B8
P 2490 4075
AR Path="/5D0EA3B8" Ref="#PWR?"  Part="1" 
AR Path="/5D01A7D6/5D0EA3B8" Ref="#PWR0142"  Part="1" 
F 0 "#PWR0142" H 2490 3925 50  0001 C CNN
F 1 "+3V3" H 2465 4225 50  0000 C CNN
F 2 "" H 2490 4075 50  0001 C CNN
F 3 "" H 2490 4075 50  0001 C CNN
	1    2490 4075
	1    0    0    -1  
$EndComp
Wire Wire Line
	2325 4375 2490 4375
Connection ~ 2490 4375
Wire Wire Line
	2490 4375 4335 4375
Text Notes 2390 4665 0    50   ~ 0
HIGH - Headphones Inserted\nLOW - Use Speaker
Text HLabel 9075 2795 2    50   Input ~ 0
GATE
Text HLabel 9150 2895 2    50   Input ~ 0
CV
Wire Wire Line
	9150 2895 8575 2895
Text HLabel 8865 2995 2    50   Input ~ 0
RACK_PWR
Wire Wire Line
	3285 3975 4335 3975
Connection ~ 2485 2630
Wire Wire Line
	2485 2630 3755 2630
NoConn ~ 4335 3725
NoConn ~ 4335 3875
NoConn ~ 4335 4075
NoConn ~ 4335 4275
$EndSCHEMATC
