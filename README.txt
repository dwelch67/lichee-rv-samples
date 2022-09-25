
Got some Sipeed Lichee RV boards for less than $20 after seeing an 
Ubuntu and other support new risc-v boards.

So far I do not need a dock board.  You can use the lichee rv board
as is without a dock.  You should solder the four pin header on for
the uart.  And you will need an sd card, you dont even need a few gig
but small ones are harder to get.  I am using an 8GByte one I had
laying around.

Of course I have zero interest in running linux, but this, in theory,
provides more support.

As usual with sipeed, allwinner, etc you are likely in for some pain.

The night I stayed up late this site

https://linux-sunxi.org/Sipeed_Lichee_RV

Was down, eventually I found stuff at debian and it was most useful.

https://wiki.debian.org/InstallingDebianOn/Sipeed/LicheeRV
in particular
https://andreas.welcomes-you.com/boot-sw-debian-risc-v-lichee-rv/
NOTE THAT THE DD COMMANDS USE /DEV/SDC NOT /DEV/SDX make sure you
check all the lines to point at your sd card.

So with the files I have provided

sudo dd if=/dev/zero of=/dev/sdX bs=1M count=200
sudo dd if=boot0_sdcard_sun20iw1p1.bin of=/dev/sdX bs=8192 seek=16
sudo dd if=u-boot.toc1 of=/dev/sdX bs=512 seek=32800

You can figure out what device your sd card is and use that instead
of sdX. (no number like sdg1 but sdg)

Dont need the other steps as this is what we need to get to uboot.

You can find the D1-H_User_Manual_V1.0.pdf and in there 

0x02500000 is the base address for a 16550 uart clone.

You want a usb to 3.3V uart breakout board, let you figure that out

my /etc/minicom/minirc.usb0 file looks like this

pu port             /dev/ttyUSB0
pu minit            
pu mhangup          
pu rtscts           No 
pu sound            No 

most important are the prot and rtscts disable (no flow control)

sudo minicom usb0

I know it is ttyUSB0 for my device because of dmesg.  Can unplug it
dmesg -c then plug it in then dmesg to insure that is the uart you look
to use, can create other minirc files above and just change the filename
and /dev/ttyUSBxxx number to match what you have on your machine.

Yes life is much easier running as root instead of sudo everything.

dmesg | grep ttyUSB0
[   16.151210] usb 3-2.4.2: ch341-uart converter now attached to ttyUSB0

You want to solder the four pin header onto the board or in some way
twist wires, whatever to get uart tx to rx and rx to tx between the
lichee rv board and your usb uart board.

Now you can hopefully see this.

...

Hit any key to stop autoboot:  0 
switch to partitions #0, OK
mmc0 is current device
** No partition table - mmc 0 **
Couldn't find partition mmc 0:1
starting USB...
Bus usb@4101000: USB EHCI 1.00
Bus usb@4101400: USB OHCI 1.0
Bus usb@4200000: USB EHCI 1.00
Bus usb@4200400: USB OHCI 1.0
scanning bus usb@4101000 for devices... 1 USB Device(s) found
scanning bus usb@4101400 for devices... 1 USB Device(s) found
scanning bus usb@4200000 for devices... 1 USB Device(s) found
scanning bus usb@4200400 for devices... 1 USB Device(s) found
       scanning usb for storage devices... 0 Storage Device(s) found

Device 0: unknown device
No ethernet found.
No ethernet found.
=> 

So if I 

=> mw 0x02500000 0x55
U=> 

It adds the U to the uart output.  The information we have so far looks
good.

from booting and printenv this has 512MBytes (0x20000000 bytes) of dram
and I think it starts at 0x40000000

we can pick something in the middle there (0x50000000)

some u-boot commands to note

loads     - load S-Record file over serial line
loadx     - load binary file over serial line (xmodem mode)
go        - start application at address 'addr'

I have provided a build_riscv64 script to build your own gnu toolchain
from sources.

If you do this

export PREFIX=/opt/gnuriscv64

then use sudo to run the script so that it gets write permissions,
otherwise you can make it something local

export PREFIX=/home/myusername/gnuriscv64

PATH=/opt/gnuriscv64/bin:$PATH
(or whatever your install path is)

Then you can build these.  They will work with an rv32 compiler as well
but we need a 64 bit one for something later.  

build notmain.srec in hello00 directory.

Put that somewhere easy to get to from minicom (if you run as root,
which makes life easier, sudo copy the file to /root.  Run minicom
from the root dir then ctrl-a then s then up arrow to ascii download
then enter a couple more times then notmain.srec enter then wait for
the uboot prompt).

then 

go 0x50000000

from the uboot prompt to start the program.

You get the 0123456701234567 output from the uart.  

After a minute or two it stops though and reboots, so perhaps there is
a watchdog timer setup.


The schematic shows an led on PC1 and P033 and the docs say GPIO65, but
there is no 65 it is PA, PB, PC...So lets try PC1.

gpio base is 0x02000000
pc config is offset 0x60
pc data is offset 0x70

=> md 0x02000060 1
02000060: ffffffff                             ....
=> 

so maybe that is true maybe all the pc pins are disabled, we want
an output for 1 so 

mw 0x02000060 0xFFFFFF1F

=> mw 0x02000060 0xFFFFFF1F
=> md 0x02000060 1         
02000060: ffffff1f                             ....
=> 

mw 0x02000070 0x00000002

and that turns an led on.

Now we can blink it

hello01  same as hello00 loads the srec file and go 0x50000000 to run
it.

It blinks the led then the thing resets for some reason.

I will tell you why in a minute, but hello02 is the same as hello01
but it is built for address 0x45000.

And we can load the srec and run it just like the hello01 version.


REMOVE the sd card and with or witout the fel button it goes into this
mode.

https://github.com/xboot/xfel

Clone it and build it

sudo ./xfel version
AWUSBFEX ID=0x00185900(D1/F133) dflag=0x44 dlength=0x08 scratchpad=0x00045000

If we take notmain.bin (not srec, but the .bin) from hello02

sudo ./xfel write 0x45000 notmain.bin
sudo ./xfel exec 0x45000

And the led blinks...at a slower rate, expected.

And it does not stop running after a minute or two, no watchdog or
whatever was setup by the other code.

They say that you can have problems with leakage from the uart,
I guess I am lucky but when you want to try this again turn off the
board completely.  I have a usb hub with pushbutton power per port.
Arguably I could put the uart there too.  If needed then power off
or unplug the uart usb as well to not keep the part powered from the
uart pin.  For now I do not have uart in this fel mode so can unplug
the uart board in general.

If you have/leave the sd card in then you do want to power cycle the
board with the fel button pressed.  Then use xfel.


https://github.com/smaeul/sun20i_d1_spl

This is what we want the riscv64 for because the makefile does not like
the 32 bit.

make CROSS_COMPILE=riscv64-none-elf- p=sun20iw1p1 mmc

Then get the file nboot/boot0_sdcard_sun20iw1p1.bin, actually the
file I provided in the base directory of this repo I built it was not
the one that the debian howto person provided.

If we do not write uboot

sudo dd if=/dev/zero of=/dev/sdX bs=1M count=200
sudo dd if=boot0_sdcard_sun20iw1p1.bin of=/dev/sdX bs=8192 seek=16

[322]card no is 0
[324]sdcard 0 line count 4
[326][mmc]: mmc driver ver 2021-04-2 16:45
[336][mmc]: Wrong media type 0x0
[339][mmc]: ***Try SD card 0***
[360][mmc]: HSSDR52/SDR25 4 bit
[363][mmc]: 50000000 Hz
[365][mmc]: 7580 MB
[367][mmc]: ***SD/MMC 0 init OK!!!***
[373]error:bad magic.
[377]error:bad magic.
[379]Loading boot-pkg fail(error=4)

It gets that far so we would now need to know how to make a binary
that no doubt lives here

sudo dd if=u-boot.toc1 of=/dev/sdX bs=512 seek=32800

That makes this loader happy.

So digging through this sun boot code and such

I believe dram starts at 0x40000000
I believe the use 0x41000000 as a workspace
0x4A000000 is where uboot starts.
There is a header and then some number of items after that, need
at least one and one named u-boot and thats the program.

hello03/mkheader.c

Puts the header on so that 

sudo dd if=/dev/zero of=/dev/sdX bs=1M count=200
sudo dd if=boot0_sdcard_sun20iw1p1.bin of=/dev/sdX bs=8192 seek=16
sudo dd if=notmain.header of=/dev/sdX bs=512 seek=32800

works.








make CROSS_COMPILE=riscv64-none-elf- p=sun20iw1p1 clean
make CROSS_COMPILE=riscv64-none-elf- p=sun20iw1p1 mmc

sudo dd if=/dev/zero of=/dev/sdf bs=1M count=200
sudo dd if=../boot0_sdcard_sun20iw1p1.bin of=/dev/sdf bs=8192 seek=16
sudo dd if=../notmain.header of=/dev/sdf bs=512 seek=32800

