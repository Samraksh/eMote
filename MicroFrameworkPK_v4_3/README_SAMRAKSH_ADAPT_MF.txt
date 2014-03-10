MF v1.01 Refresh Highlights:
•	Sensor: API for ADAPT core board Gyroscope added, Samraksh.eMote.ADAPT.Gyroscope
•	Power: First implementation of Krait power management driver.
o	Default idle mode is Wait-For-Interrupt.
o	Default sleep mode is L2 retention sleep.
•	App Note: Added application note for Basic IO operations on ADAPT in C#.
•	Timer: Fix platform definition to enable HAL CPU_MillisecondsToTicks conversion
•	Timer: Significant Timer code re-write. Fixes time inconsistency issues and simplifies code.
•	MF: Namespace changes (cleanups). Samraksh_SPOT_Hardware is now just “eMote”. Any old references will need to be modified. Please see user manual.
o	For instance, the Accelerometer can now be accessed for basic functionality with “Samraksh.eMote.Accelerometer” while the full driver is accessible under “Samraksh.eMote.ADAPT.Accelerometer”
•	MF: Fixed interpreter problem.
•	GPIO: Fixed issue where stale debug code causing spurious GPIO action.
•	GPIO: Fixed issue where certain GPIO pins could not be read.
•	GPIO: Added code addressing issue where GPIO interrupts not firing.
