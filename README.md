## HuEverReset
InGame-Reset fpr PC Engine with support for TurboEverdrive v1-v2.5

### v20221107 / ArcadeTV

![HuEverReset pcb](https://github.com/ArcadeTV/HuEverReset/blob/main/HuEverReset_brd.png?raw=true)

| Part | Value            | Device           | Package             |
| ---- | ---------------- | ---------------- | ------------------- |
| CB1  | 100nF            | C-USC0805        | C0805               |
| C1   | 220uF            | CPOL-USC/6032-28R| C/6032-28R          |
| D1   | RB168VWM-40TR    | RB168VWM-40TR    | RB168VWM40TR        |
| IC1  | ATTINY84V-10SSUR | ATTINY84V-10SSUR | SOIC127P600X175-14N |
| R1   | 10K              | R-US_R0805       | R0805               |
| R2   | 10K              | R-US_R0805       | R0805               |

---


#### Description:
Since a TurboEverdrive v1 to v2.5 is not listening to a hard reset (/RESET), implementing an IngameReset that is triggered upon pressing a button-combo on the controller is not possible. This mod solves that problem by replacing the 7805 regulator with a KA78R05 that has a switchable output. So, instead of pulling the /RESET line of the PC Engine LOW, this mod aims to cut the power of the system for 1 second and turning it back on. This ensures that the menu of the TurboEverdrive is displayed.

#### Microcontroller:
Please mind that the ATtiny84 microcontroller needs to be programmed
with the provided hex file.

#### PCB
Use the provided gerber files to order your pcb from your favorite manufacturer. 
I recommend jlcpcb. When ordering from jlcpcb, fill out the order-form as follows:

```
Different Design: 2
Delivery Format: Panel by Customer
PCB Thickness: 1.0 mm
```

Eagle source files for editing also exist in this repository.

---

#### Thanks

NeoRame, Astrocade & micro
