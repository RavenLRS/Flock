FlockOne
========

This is a cheap STM32F0 based board with a total DIY cost
around $4 per board. Users with previous soldering experience
should have no problem building it with just a soldering iron.

If you have never soldered SMD components, don't be afraid! All
the components but the LoRa module are very cheap, so this design
is also a good project to get you started with SMD. Just make sure
to use a fine tip, tweezers and copious amounts of flux.

### BOM

- 1x [STM32F030F4P6][1] (10pcs, \~$0.35/pc)
- 1x [Ai-Thinker Ra-02][2] (1pc, \~$2.65/pc)
- 1x [100 nF 0603 Capacitor][3] (**\***) (100pcs, $0.01/pc)
- 2x [4.7 uF 0603 Capacitor][3] (**\***) (100pcs, $0.01/pc)
- 2x [10 kOhm 0603 Resistor][4] (**\***) (100pcs, $0.01/pc)
- 1x [330 Ohm 0603 Resistor][4] (**\***) (100pcs, $0.01/pc)
- 1x [MIC5219-3.3BM5][5] (10pcs, $0.09/pc)
- 1x [0603 LED][6] (**\***) (100pcs, $0.01/pc)

(**\***) Listing with multiple values, select the right one!

### PCB

Use the [FlockOne.zip][FlockOne.zip] file to order the PCB from your
fabrication service of choice. Some popular manufacturers for
hobbyists:

- JLCPCB: ~$7 for 10 boards + shipping (\~$0.70/pc)
- OshPark: ~$2.85 for 3 bords + shipping (\~$0.95/pc)

### Antenna

Any 433Mhz properly tuned antenna should work, there's no reason
to use a high gain one. The RA-02 module includes an IPEX 
connector, so if you plan to use an antenna with a different 
connector (e.g. SMA) you should buy a pigtail too. Make sure to 
pick an ommnidirectional one. These are some examples:

- [433MHz Spring Antenna][ant1] (1pc, \~$0.58/pc)


[1]: http://s.click.aliexpress.com/e/bBTYCsGo
[2]: http://s.click.aliexpress.com/e/YACp66M
[3]: http://s.click.aliexpress.com/e/ljKo0PW
[4]: http://s.click.aliexpress.com/e/jFzlBWo
[5]: http://s.click.aliexpress.com/e/bK4Lefbi
[6]: http://s.click.aliexpress.com/e/bfQQhzdW
[FlockOne.zip]: gerbers/FlockOne.zip
[ant1]: https://www.aliexpress.com/item/433MHz-spring-antenna-3dBi-wireless-module-digital-transmit-antenna-IPEX-U-FL-interface-for-Rola-Module/32904294792.html
