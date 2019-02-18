
// mask the LEDs to show the appropriated words in the appropriated language.

#if LANGUAGE == 0 // English

#elif LANGUAGE == 1 // German
if (minutes >= 25) { // choose "VOR" as off minute twenty-five
	hours++;
}
hours %= 12;
minutes -= minutes % 5;

if (minutes == 5 || minutes == 25 || minutes == 35 || minutes == 55) {
	marker->mark(7, 0, 4); // FUENF
}
else if (minutes == 10 || minutes == 50) {
	marker->mark(0, 1, 4); // ZEHN
}
else if (minutes == 15 || minutes == 45) {
	marker->mark(4, 2, 7); // VIERTEL
}
else if (minutes == 20 || minutes == 40) {
	marker->mark(4, 1, 7); // ZWANZIG
}
if (minutes == 0) {
	marker->mark(8, 9, 3); // UHR
}
else if (minutes == 25 || minutes == 30 || minutes == 35) {
	marker->mark(0, 4, 4); // HALB
}
if (minutes == 5 || minutes == 10 || minutes == 15 || minutes == 20 || minutes == 35) {
	marker->mark(2, 3, 4); // NACH
}
else if (minutes == 25 || minutes == 40 || minutes == 45 || minutes == 50 || minutes == 55) {
	marker->mark(6, 3, 3); // VOR
}

if (hours == 0) {
	marker->mark(5, 4, 5); // ZWOELF
}
else if (hours == 1) {
	if (minutes == 0)
		marker->mark(2, 5, 3); // EIN
	else
		marker->mark(2, 5, 4); // EINS
}
else if (hours == 2) {
	marker->mark(0, 5, 4); // ZWEI
}
else if (hours == 3) {
	marker->mark(1, 6, 4); // DREI
}
else if (hours == 4) {
	marker->mark(7, 7, 4); // VIER
}
else if (hours == 5) {
	marker->mark(7, 6, 4); // FUENF
}
else if (hours == 6) {
	marker->mark(1, 9, 5); // SECHS
}
else if (hours == 7) {
	marker->mark(5, 5, 6); // SIEBEN
}
else if (hours == 8) {
	marker->mark(1, 8, 4); // ACHT
}
else if (hours == 9) {
	marker->mark(3, 7, 4); // NEUN
}
else if (hours == 10) {
	marker->mark(5, 8, 4); // ZEHN
}
else if (hours == 11) {
	marker->mark(0, 7, 3); // ELF
}
#endif