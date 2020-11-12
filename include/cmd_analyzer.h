/* -------------------------------------------------------------
 *
 *  file:        cmd_analyzer.cpp
 *
 *  description: command line interpreter
 *               experimental function
 *
 *  author:      Beat Meier
 *  modified:    25.7.2017
 *
 *  rev:
 *
 * -------------------------------------------------------------
 */

CMD_REG(seqstart, "", "Start own sequence")
CMD_REG(getimg, "", "Read an image from r4s")
CMD_REG(seqreadout, "", "Load measure -> readout sequence row wise")
CMD_REG(seqcolreadout, "", "Load measure -> readout sequence column wise")
CMD_REG(seqcalscan, "", "Load calibrate scan sequence")
CMD_REG(seqtest, "", "Load test sequence")
CMD_REG(xscan, "", "Scan through columns")
CMD_REG(yscan, "", "Scan through rows")
CMD_REG(dacscan, "", "Do a DAC scan")

CMD_REG(gui, "", "Start graphical user interface");
CMD_REG(takedata, "<#events>", "Take data");
