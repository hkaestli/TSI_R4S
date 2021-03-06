vd 2200 mV
id 200  mA
va 2000 mV
ia 300  mA

ibiasio  370 mV @ 4.7kOhm 130uA
voffset  460
vfb     1100
vn2      750
vn1      730
vn0      740
vcascn   800
vbiasd  2047

ibiasro  525 mV @ 22kOhm 20uA
vcal     1700
vbiasr  1800
vprefb   700 
vcascp  1100
vp0      820
vp1      725
vp2      685

v18     1800
vanan   1800
vddio   1800
vanap   1250
vdig    1800

adcdel 7
daqena 3

cal 2 2

tspan 3000
tcal   2508
thold  2610

d1 5 trigger
d2 0
a2 6
a1 1 roc4sens data out

seqcolreadout

pon
mdelay 500
getia
getid
go
