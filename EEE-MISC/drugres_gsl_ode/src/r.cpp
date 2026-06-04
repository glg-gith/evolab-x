/*
 * r.cpp
 *
 *  Created on: 3 octobre 2013
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
//	string s_t, s_S, s_I, s_T, s_rt, s_rS, s_rI, s_rT;
//	string s_g0, s_g1, s_g2, s_g3, s_rg0, s_rg1, s_rg2, s_rg3;
//	string s_ldI, s_ldT, s_rldI, s_rldT;

	string s_t, s_S, s_I0, s_I1, s_I2, s_I3, s_T0, s_T1, s_T2, s_T3, s_rt, s_rS, s_rI, s_rT;

	string line;
	char c;
	int i = 0;

	bool new_line = true;

	try
	{

		// Write vectors

		s_rt = "vt <- c("; s_rS = "vS <- c("; s_rI = "vI <- c("; s_rT = "vT <- c(";
//		s_rg0 = "vg0 <- c("; s_rg1 = "vg1 <- c("; s_rg2 = "vg2 <- c("; s_rg3 = "vg3 <- c(";
//		s_rldI = "vldI <- c("; s_rldT = "vldT <- c(";

		///while (i < 23 && !f_in.eof()) { getline(f_in, line); i++;};

		double t_cur = 0.0, t_next = 0.0;

//		f_in >> s_t >> s_S >> s_I >> s_T >> s_g0 >> s_g1 >> s_g2 >> s_g3 >> s_ldI >> s_ldT;
		f_in >> s_t >> s_S >> s_I0 >> s_I1 >> s_I2 >> s_I3 >> s_T0 >> s_T1 >> s_T2 >> s_T3;
		while (!f_in.eof()) { // keep reading until end-of-file

			//
			stringstream ss(s_t);
			ss >> t_cur;
			//cout << t_cur << endl;
			if (t_cur >= t_next) {

	//			s_rt += s_t + ","; s_rS += s_S + ","; s_rI += s_I + ","; s_rT += s_T + ",";
	//			s_rg0 += s_g0 + ","; s_rg1 += s_g1 + ","; s_rg2 += s_g2 + ","; s_rg3 += s_g3 + ",";
	//			s_rldI += s_ldI + ","; s_rldT += s_ldT + ",";

				s_rt += s_t + ",";
				s_rS += s_S + ",";
				s_rI += s_I0 + " + " + s_I1 + " + " + s_I2 + " + " + s_I3 + ",";
				s_rT += s_T0 + " + " + s_T1 + " + " + s_T2 + " + " + s_T3 + ",";

				new_line = (i % 10 == 0);

				t_next = t_cur + 1.0;

			} else {
				new_line = false;
			}

			// Goto end of line
			do {
				c = f_in.get();
				if (c == '\r') c = f_in.get();
			} while (!f_in.eof() && c != '\n');

			if (!f_in.eof()) //&& (i % modulus == 0))
			{
				// Get strings of interest
//				f_in >> s_t >> s_S >> s_I >> s_T >> s_g0 >> s_g1 >> s_g2 >> s_g3 >> s_ldI >> s_ldT;
				f_in >> s_t >> s_S >> s_I0 >> s_I1 >> s_I2 >> s_I3 >> s_T0 >> s_T1 >> s_T2 >> s_T3;
				// Avoid long lines for Copy/Paste in R-Console compliance
				if (new_line) {
					s_rt += "\n"; s_rS += "\n"; s_rI += "\n"; s_rT += "\n";
//					s_rg0 += "\n"; s_rg1 += "\n"; s_rg2 += "\n"; s_rg3 += "\n";
//					s_rldI += "\n"; s_rldT += "\n";
				}

			}

			i++;

		}

		if (s_rt[s_rt.length()-1] == '\n') {
			s_rt[s_rt.length()-2] = ')';
			s_rS[s_rS.length()-2] = ')';
			s_rI[s_rI.length()-2] = ')';
			s_rT[s_rT.length()-2] = ')';
		} else {
			s_rt[s_rt.length()-1] = ')';
			s_rS[s_rS.length()-1] = ')';
			s_rI[s_rI.length()-1] = ')';
			s_rT[s_rT.length()-1] = ')';
		}

//		s_rg0[s_rg0.length()-1] = ')';
//		s_rg1[s_rg1.length()-1] = ')';
//		s_rg2[s_rg2.length()-1] = ')';
//		s_rg3[s_rg3.length()-1] = ')';
//
//		s_rldI[s_rldI.length()-1] = ')';
//		s_rldT[s_rldT.length()-1] = ')';

		//cout << s_rt << endl << s_rS << endl << s_rI << endl << s_rT << endl;
		//cout << s_rg0 << endl << s_rg1 << endl << s_rg2 << endl << s_rg3 << endl;
		f_out << s_rt << "\n" << s_rS << "\n" << s_rI << "\n" << s_rT << "\n";
//		f_out << s_rg0 << "\n" << s_rg1 << "\n" << s_rg2 << "\n" << s_rg3 << "\n";
//		f_out << s_rldI << "\n" << s_rldT << "\n";


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
par(col=\"white\", font=4)\n\
lnames <- c(\"S\", \"I\", \"T\")\n\
legend(\"topright\", lnames, col = c(\"blue\", \"red\", \"green\"), lty = 1)\n\
\n\
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





