## HuEverReset
InGame-Reset for PC Engine with support for TurboEverdrive v1-v2.5

_*Untested!*_ - Please consider waiting until everything is confirmed working.

### v20221109 / ArcadeTV

---

#### Description:
This enables an InGame-Reset by pressing a button-combo on the controller that also works with TurboEverdrive to get back to the menu.
It is designed for the white japanese PC Engine console but can be easily adapted to any PC Engine system if you know which pins of the controller-input-socket to connect to the pcb. Optionally you can use a piezo speaker for acoustic feedback upon triggering a reset.

---

#### Parts
![HuEverReset pcb](https://github.com/ArcadeTV/HuEverReset/blob/main/HuEverReset_brd.png?raw=true)

| Part | Value            | Device           | Package             |
| :--- | :--------------- | :--------------- | :------------------ |
| CB1  | 100nF            | C-USC0805        | C0805               |
| C1   | 220uF            | CPOL-USC/6032-28R| C/6032-28R          |
| D1   | RB168VWM-40TR    | RB168VWM-40TR    | RB168VWM40TR        |
| IC1  | ATTINY84V-10SSUR | ATTINY84V-10SSUR | SOIC127P600X175-14N |
| IC2  | KA78R05CTU_NL    | KA78R05          | TO-220-4            |
| R1   | 10K              | R-US_R0805       | R0805               |
| R2   | 10K              | R-US_R0805       | R0805               |


Optionally, a piezo speaker like the [KPM-1205A](https://datasheetspdf.com/pdf/868392/Ningbo/KPM-1205A/1). A 220 Ohms resistor is recommended on the positive leg for attenuation.

---

#### Installation:
See the [wiki](https://github.com/ArcadeTV/HuEverReset/wiki)

---

#### Technical:
Since a TurboEverdrive v1 to v2.5 is not listening to a hard reset (/RESET), implementing an IngameReset that is triggered upon pressing a button-combo on the controller is not possible. This mod solves that problem by replacing the 7805 regulator with a KA78R05 that has a switchable output. So, instead of pulling the /RESET line of the PC Engine LOW, this mod aims to cut the power of the system for 1 second and turning it back on. This ensures that the menu of the TurboEverdrive is displayed. The pcb consists of 2 different layouts including a little POWER BOARD for making the replacement of the power-regulator a breeze.

---

#### Microcontroller:
```
ATtiny84 ----------------
                      _____
             VCC  01-|°    |-14 GND
/Jmp: Use Speaker 02-|     |-13 /ENABLE (MiniDin 7)
      Speaker (+) 03-|     |-12 D3: Run, Left (MiniDin 5)
             N.C. 04-|     |-11 D2: Select, Down (MiniDin 4)
             N.C. 05-|     |-10 D1: II, Right (MiniDin 3)
             N.C. 06-|     |-09 /SELECT (MiniDin 6)
           /RESET 07-|_____|-08 D0: I, Up (MiniDin 2)

| Fuses: 
| -----------
| low:   0xE2
| high:  0xD7
| ext.:  0xFF
| lock:  0xFF
```
Please mind that the ATtiny84 microcontroller needs to be programmed
with the provided hex file. If you use the ISP header on the pcb, please do so while the board is unpopulated except for IC1.

---

#### Jumper J1
Enable or Disable the use of a piezo speaker (buzzer).
```
Closed: use
Open: ignore
```
If you connect a piezo speaker for acoustic feedback when a reset is triggered, close J1. You may want to use a resistor on the positive leg of the speaker (e.g. 220 Ohms) if you find it too loud.

---

#### PCB
Use the provided gerber files to order your pcb from your favorite manufacturer. 
I recommend jlcpcb. When ordering from jlcpcb, fill out the order-form as follows:

```
Different Design: 2
Delivery Format: Panel by Customer
PCB Thickness: 1.0 mm
```

**GERBER** files can be downloaded from the `releases` tab: 

https://github.com/ArcadeTV/HuEverReset/releases/latest

Eagle source files for editing also exist in this repository.

---

#### Thanks

NeoRame, Astrocade & micro
