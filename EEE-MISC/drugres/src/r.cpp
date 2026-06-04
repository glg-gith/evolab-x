/*
 * r.cpp
 *
 *  Created on: 25 juin 2012
 *      Author: golgauth
 */



#include <iostream>	// For cin, cout, cerr, clog
#include <fstream>	// For ofstream
#include <iomanip>	// For setw
#include <cmath>	// For log, pow
#include <cstring>	// For strcomp
#include <string.h>
#include <sstream>

using namespace std;

void to_r_script(char *out_rs_name, char *in_txt_name, char *out_img_name)
{
	ifstream f_in;
	ofstream f_out;
	f_in.open(in_txt_name);
	f_out.open(out_rs_name);

	//fprintf(stdout, "\n/*===========================  <R-Vectors>  ===========================*/\n\n");

	// Read lines
	string s;
	string s_t, s_S, s_I, s_T, s_rt, s_rS, s_rI, s_rT;
	string s_g0, s_g1, s_g2, s_g3, s_rg0, s_rg1, s_rg2, s_rg3;
	string s_ldI, s_ldT, s_rldI, s_rldT;
	string line;
	char c;
	int i = 0;

	try
	{

		// Write vectors

		s_rt = "vt <- c("; s_rS = "vS <- c("; s_rI = "vI <- c("; s_rT = "vT <- c(";
		s_rg0 = "vg0 <- c("; s_rg1 = "vg1 <- c("; s_rg2 = "vg2 <- c("; s_rg3 = "vg3 <- c(";
		s_rldI = "vldI <- c("; s_rldT = "vldT <- c(";

		while (i < 23 && !f_in.eof()) { getline(f_in, line); i++;};

		f_in >> s_t >> s_S >> s_I >> s_T >> s_g0 >> s_g1 >> s_g2 >> s_g3 >> s_ldI >> s_ldT;
		while (!f_in.eof()) { // keep reading until end-of-file

			s_rt += s_t + ","; s_rS += s_S + ","; s_rI += s_I + ","; s_rT += s_T + ",";
			s_rg0 += s_g0 + ","; s_rg1 += s_g1 + ","; s_rg2 += s_g2 + ","; s_rg3 += s_g3 + ",";
			s_rldI += s_ldI + ","; s_rldT += s_ldT + ",";

			// Goto end of line
			do {
				c = f_in.get();
				if (c == '\r') c = f_in.get();
			} while (!f_in.eof() && c != '\n');

			if (!f_in.eof())
			{
				// Get strings of interest
				f_in >> s_t >> s_S >> s_I >> s_T >> s_g0 >> s_g1 >> s_g2 >> s_g3 >> s_ldI >> s_ldT;
				// Avoid long lines for Copy/Paste in R-Console compliance
				if (i%10 == 0) {
					s_rt += "\n"; s_rS += "\n"; s_rI += "\n"; s_rT += "\n";
					s_rg0 += "\n"; s_rg1 += "\n"; s_rg2 += "\n"; s_rg3 += "\n";
					s_rldI += "\n"; s_rldT += "\n";
				}
			}

			i++;

		}
		s_rt[s_rt.length()-1] = ')';
		s_rS[s_rS.length()-1] = ')';
		s_rI[s_rI.length()-1] = ')';
		s_rT[s_rT.length()-1] = ')';

		s_rg0[s_rg0.length()-1] = ')';
		s_rg1[s_rg1.length()-1] = ')';
		s_rg2[s_rg2.length()-1] = ')';
		s_rg3[s_rg3.length()-1] = ')';

		s_rldI[s_rldI.length()-1] = ')';
		s_rldT[s_rldT.length()-1] = ')';

		//cout << s_rt << endl << s_rS << endl << s_rI << endl << s_rT << endl;
		//cout << s_rg0 << endl << s_rg1 << endl << s_rg2 << endl << s_rg3 << endl;
		f_out << s_rt << "\n" << s_rS << "\n" << s_rI << "\n" << s_rT << "\n";
		f_out << s_rg0 << "\n" << s_rg1 << "\n" << s_rg2 << "\n" << s_rg3 << "\n";
		f_out << s_rldI << "\n" << s_rldT << "\n";


		// Plot
		s = "\n#-------------------------- DISPLAY ----------------------------#\n";

		// Saving the resulting plot image
		if (strcmp(out_img_name, "") != 0)
		{
			s += "\n\
# Save render\n\
bmp(\n\
\t\"" + (string)out_img_name + "\"\n\
\t, width=1200, height=700, bg = \"white\"\n\
)\n\
\n";
		}


		s += "\n\
par(mfrow = c(2, 2))\n\
\n\
# Hosts\n\
par(plt = c(0.1, 0.9, 0.2, 0.8), cex.axis = 1, cex=1, bg = \"grey\", font.lab=2, col.lab=\"darkgrey\", pch=19)\n\
plot(vt, vS, type=\"l\", xlab=\"Discrete Time\", ylab=\"Nb Individuals\", \n\t\
xlim=c(min(vt),max(vt)), ylim=c(min(c(vS, vI, vT)), max(c(vS, vI, vT))))\n\
rect(par(\"usr\")[1], par(\"usr\")[3], par(\"usr\")[2], par(\"usr\")[4], col=\"lightgrey\")\n\
title(\"Hosts Versus Time\", font.main=4, col.main=\"darkgrey\")\n\
\n\
par(col=\"black\", font=1)\n\
lines(vt, vS, col=\"blue\", lwd=2)\n\
lines(vt, vI, col=\"red\", lwd=2)\n\
lines(vt, vT, col=\"green\", lwd=2)\n\
\n\
lm.vS = lm(vS ~ vt + I(vt^2) + I(vt^3) + I(vt^4) + I(vt^5))\n\
lm.vI = lm(vI ~ vt + I(vt^2) + I(vt^3) + I(vt^4) + I(vt^5))\n\
lm.vT = lm(vT ~ vt + I(vt^2) + I(vt^3) + I(vt^4) + I(vt^5))\n\
curve(lm.vS$coefficients[1]+lm.vS$coefficients[2]*x+lm.vS$coefficients[3]*x^2+\n\t\
lm.vS$coefficients[4]*x^3+lm.vS$coefficients[5]*x^4+lm.vS$coefficients[6]*x^5,add=T)\n\
curve(lm.vI$coefficients[1]+lm.vI$coefficients[2]*x+lm.vI$coefficients[3]*x^2+\n\t\
lm.vI$coefficients[4]*x^3+lm.vI$coefficients[5]*x^4+lm.vI$coefficients[6]*x^5,add=T)\n\
curve(lm.vT$coefficients[1]+lm.vT$coefficients[2]*x+lm.vT$coefficients[3]*x^2+\n\t\
lm.vT$coefficients[4]*x^3+lm.vT$coefficients[5]*x^4+lm.vT$coefficients[6]*x^5,add=T)\n\
\n\
par(col=\"white\", font=4)\n\
lnames <- c(\"S\", \"I\", \"T\")\n\
legend(\"topright\", lnames, col = c(\"blue\", \"red\", \"green\"), lty = 1)\n\
\n\
\n\
# Linkage Disequilibrium (LD)\n\
par(plt = c(0.1, 0.9, 0.2, 0.8), cex.axis = 1, cex=1, bg = \"grey\", font.lab=2, col.lab=\"darkgrey\", pch=19)\n\
plot(vt, vldI, type=\"l\", xlab=\"Discrete Time\", ylab=\"LD\", \n\t\
xlim=c(min(vt),max(vt)), ylim=c(min(c(vldI, vldT)), max(c(vldI, vldT))))\n\
rect(par(\"usr\")[1], par(\"usr\")[3], par(\"usr\")[2], par(\"usr\")[4], col=\"lightgrey\")\n\
title(\"LD Versus Time\", font.main=4, col.main=\"darkgrey\")\n\
\n\
par(col=\"black\", font=1)\n\
lines(vt, vldI, col=\"red\", lwd=2)\n\
lines(vt, vldT, col=\"green\", lwd=2)\n\
\n\
par(col=\"white\", font=4)\n\
lnames <- c(\"LDI\", \"LDT\")\n\
legend(\"topright\", lnames, col = c(\"red\", \"green\"), lty = 0.5)\n\
\n\
\n\
# Parasites\n\
par(plt = c(0.1, 0.9, 0.2, 0.8), cex.axis = 1, cex=1, bg = \"grey\", font.lab=2, col.lab=\"darkgrey\", pch=19)\n\
plot(vt, vg0, type=\"l\", xlab=\"Discrete Time\", ylab=\"Nb Individuals\", \n\t\
xlim=c(min(vt),max(vt)), ylim=c(min(c(vg0, vg1, vg2, vg3)), max(c(vg0, vg1, vg2, vg3))))\n\
rect(par(\"usr\")[1], par(\"usr\")[3], par(\"usr\")[2], par(\"usr\")[4], col=\"lightgrey\")\n\
title(\"Parasites Versus Time\", font.main=4, col.main=\"darkgrey\")\n\
\n\
par(col=\"black\", font=1)\n\
lines(vt, vg0, col=\"blue\", lwd=2)\n\
lines(vt, vg1, col=\"red\", lwd=2)\n\
lines(vt, vg2, col=\"green\", lwd=2)\n\
lines(vt, vg3, col=\"yellow\", lwd=2)\n\
\n\
par(col=\"white\", font=4)\n\
lnames <- c(\"ab\", \"Ab\", \"aB\", \"AB\")\n\
legend(\"topright\", lnames, col = c(\"blue\", \"red\", \"green\", \"yellow\"), lty = 1)\n\
";

		f_out << s << "\n";
	}
	catch ( char * stri )
	{
		cout << stri << endl;
	}

	f_in.close();
	f_out.flush();
	f_out.close();
}





