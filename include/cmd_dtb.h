/* -------------------------------------------------------------
 *
 *  file:        cmd_dtb.cpp
 *
 *  description: command line interpreter
 *               DTB base functions
 *
 *  author:      Beat Meier
 *  modified:    21.4.2014
 *
 *  rev:
 *
 * -------------------------------------------------------------
 */

// =======================================================================
//  connection, communication, startup commands
// =======================================================================
HELP_CAT("dtb");

CMD_REG(scan, "", "Get infos of all connected DTBs")
CMD_REG(open, "[<name>]", "open a DTB (with name)")
CMD_REG(close, "", "close DTB connection")
CMD_REG(rpclink, "", "link all DTB functions")
CMD_REG(welcome, "", "blink with LEDs")
CMD_REG(setled, "<mask>", "set atb LEDs")
CMD_REG(log, "<text>", "writes text to log file")
CMD_REG(upgrade, "<filename>", "upgrade DTB")
CMD_REG(rpcinfo, "", "list all DTB functions")
CMD_REG(info, "", "show detailed DTB info")
CMD_REG(ver, "", "shows DTB software version number")
CMD_REG(version, "", "shows DTB software version")
CMD_REG(boardid, "", "get board id")
CMD_REG(init, "", "inits the testboard")
CMD_REG(flush, "", "flushes usb buffer")
CMD_REG(clear, "", "clears usb data buffer")


// =======================================================================
//  delay commands
// =======================================================================

CMD_REG(udelay, "<us>", "waits <us> microseconds")
CMD_REG(mdelay, "<ms>", "waits <ms> milliseconds")


// =======================================================================
//  test board commands
// =======================================================================

CMD_REG(clksrc, "<source>", "Select clock source (1=ext, 0=int)")
CMD_REG(clkok, "clkok", "Check if ext clock is present")

CMD_REG(clklvl, "<level>", "clk signal level")
CMD_REG(sdalvl, "<level>", "sda signel level")
CMD_REG(ctrlvl, "<level>", "ctr signel level")
CMD_REG(tinlvl, "<level>", "tin signel level")
CMD_REG(lvds, "", "LVDS inputs")
CMD_REG(lcds, "", "LCDS inputs")
// CMD_REG(tout, "", "")
// CMD_REG(trigout, "", "")
CMD_REG(pon, "", "switch ROC power on")
CMD_REG(poff, "", "switch ROC power off")
CMD_REG(va, "<mV>", "set VA in mV")
CMD_REG(vd, "<mV>", "set VD in mV")
CMD_REG(ia, "<mA>", "set IA in mA")
CMD_REG(id, "<mA>", "set ID in mA")
CMD_REG(getva, "", "get VA in V")
CMD_REG(getvd, "", "get VD in V")
CMD_REG(getia, "", "get IA in mA")
CMD_REG(getid, "", "get ID in mA")
CMD_REG(hvon, "", "switch HV on")
CMD_REG(hvoff, "", "switch HV off")
CMD_REG(status, "", "shows testboard status")
CMD_REG(d1, "<signal>", "assign signal to D1 output")
CMD_REG(d2, "<signal>", "assign signal to D2 outout")
CMD_REG(a1, "<signal>", "assign analog signal to A1 output")
CMD_REG(a2, "<signal>", "assign analog signal to A2 outout")
CMD_REG(probeadc, "<signal>", "assign analog signal to ADC")


// === DAQ ==================================================================

HELP_CAT("daq")

CMD_REG(dopen, "<buffer size>", "Open DAQ and allocate memory")
CMD_REG(dclose, "", "Close DAQ")
CMD_REG(dstart, "", "Enable DAQ")
CMD_REG(dstop, "", "Disable DAQ")
CMD_REG(dsize, "", "Show DAQ buffer fill state")
CMD_REG(dread, "", "Read Daq buffer and show as raw data")


// === ROC4sens =============================================================

HELP_CAT("r4s")

CMD_REG(cal, "<x> <y>", "Set calibrate to pixel (x/y)")

CMD_REG(tspan, "<t>", "Set measurement span in 5 ns steps")
CMD_REG(tcal, "<t>", "Set CAL position in 5 ns steps")
CMD_REG(thold, "<t>", "Set HOLD position in 5 ns steps")

CMD_REG(adcdel, "<t>", "Set ADC sampling point in 5ns steps")
CMD_REG(daqena, "<slow, enable>", "Enable DAQ")

CMD_REG(go, "", "Start ROC4sens measurement sequence")
CMD_REG(goloop, "", "Start readout loop. Stop by pressing any key")
CMD_REG(running, "", "Check if measurement sequence running")

CMD_REG(ibiasio, "<mV>", "Set Roc4sens Ibiasio")
CMD_REG(voffset, "<mV>", "Set Roc4sens Voffset")
CMD_REG(vfb, "<mV>", "Set Roc4sens Vfb")
CMD_REG(vn2, "<mV>", "Set Roc4sens Vn2")
CMD_REG(vn1, "<mV>", "Set Roc4sens Vn1")
CMD_REG(vn0, "<mV>", "Set Roc4sens Vn0")
CMD_REG(vcascn, "<mV>", "Set Roc4sens Vcasc_n")
CMD_REG(vbiasd, "<mV>", "Set Roc4sens Vbias_d")

CMD_REG(ibiasro, "<mV>", "Set Roc4sens Ibiasro")
CMD_REG(vcal, "<mV>", "Set Roc4sens Vcal")
CMD_REG(vbiasr, "<mV>", "Set Roc4sens Vbias_r")
CMD_REG(vprefb, "<mV>", "Set Roc4sens Vprefb")
CMD_REG(vcascp, "<mV>", "Set Roc4sens Vcasc_p")
CMD_REG(vp0, "<mV>", "Set Roc4sens Vp0")
CMD_REG(vp1, "<mV>", "Set Roc4sens Vp1")
CMD_REG(vp2, "<mV>", "Set Roc4sens Vp2")

CMD_REG(v18, "<mV>", "Set Roc4sens V18")
CMD_REG(vanan, "<mV>", "Set Roc4sens Vana_n")
CMD_REG(vddio, "<mV>", "Set Roc4sens Vddio")
CMD_REG(vanap, "<mV>", "Set Roc4sens Vana_p")
CMD_REG(vdig, "<mV>", "Set Roc4sens Vdig")

CMD_REG(i2c, "<addr> <cmd> <hdata> <ldata>", "Tests I2C communication")

