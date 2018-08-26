import sys, dev_sys, digio

while 1:
	ledOut(LED_STATUS, 1)
	sys.wait(500)
	ledOut(LED_STATUS, 0)
	sys.wait(500)