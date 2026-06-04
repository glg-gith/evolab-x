/*
 * r.cpp
 *
 *  Created on: 25 juin 2012
 *      Author: golgauth
 */



#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <string.h>
#include <sstream>


///** Custom round math MACRO returning an integer */
//#define RND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


using namespace std;

void to_r_script(char *in_txt_name_stoch, char *in_txt_name_ode, char *out_rs_name, char *out_ri_name)
{
	ifstream f_in_stoch;
	f_in_stoch.open(in_txt_name_stoch);

	ifstream f_in_ode;
	f_in_ode.open(in_txt_name_ode);

	ofstream f_out;
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

	int nb_plots = 0;

	try
	{

		s_rt = "vt <- c(";


		//*************  Write vectors for STOCH  *****************//
		s_rS = "vS <- c("; s_rI = "vI <- c("; s_rT = "vT <- c(";
		s_rg0 = "vg0 <- c("; s_rg1 = "vg1 <- c("; s_rg2 = "vg2 <- c("; s_rg3 = "vg3 <- c(";
		s_rldI = "vldI <- c("; s_rldT = "vldT <- c(";

		while (i < 23 && !f_in_stoch.eof()) { getline(f_in_stoch, line); i++;};

		f_in_stoch >> s_t >> s_S >> s_I >> s_T >> s_g0 >> s_g1 >> s_g2 >> s_g3 >> s_ldI >> s_ldT;
		while (!f_in_stoch.eof()) { // keep reading until end-of-file

			s_rt += s_t + ","; s_rS += s_S + ","; s_rI += s_I + ","; s_rT += s_T + ",";
			s_rg0 += s_g0 + ","; s_rg1 += s_g1 + ","; s_rg2 += s_g2 + ","; s_rg3 += s_g3 + ",";
			s_rldI += s_ldI + ","; s_rldT += s_ldT + ",";

			nb_plots++;

			// Goto end of line
			do {
				c = f_in_stoch.get();
				if (c == '\r') c = f_in_stoch.get();
			} while (!f_in_stoch.eof() && c != '\n');

			if (!f_in_stoch.eof())
			{
				// Get strings of interest
				f_in_stoch >> s_t >> s_S >> s_I >> s_T >> s_g0 >> s_g1 >> s_g2 >> s_g3 >> s_ldI >> s_ldT;
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

		cout << "Read '" << nb_plots << "' plots for stoch !" << endl;



		//***************  Write vectors for ODE  *****************//

		// Write vectors

		string s_I0, s_I1, s_I2, s_I3, s_T0, s_T1, s_T2, s_T3, s_rS, s_rI, s_rT;
		string s_ldI, s_ldT, s_rldI, s_rldT;

		i = 0;

		nb_plots = 0;

		bool new_line = true;

		s_rt = "vt_ode <- c(";
		s_rS = "vS_ode <- c("; s_rI = "vI_ode <- c("; s_rT = "vT_ode <- c(";
		s_rg3 = "vg0_ode <- c("; s_rg1 = "vg1_ode <- c("; s_rg2 = "vg2_ode <- c("; s_rg0 = "vg3_ode <- c(";
		s_rldI = "vldI_ode <- c("; s_rldT = "vldT_ode <- c(";


		double t_cur = 0.0;
		int t_next = 0;

		f_in_ode >> s_t >> s_S >> s_I0 >> s_I2 >> s_I1 >> s_I3 >> s_T0 >> s_T2 >> s_T1 >> s_T3 >> s_ldI >> s_ldT;
		while (!f_in_ode.eof()) { // keep reading until end-of-file

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
				s_rg0 += s_I0 + " + " + s_T0 + ",";
				s_rg1 += s_I1 + " + " + s_T1 + ",";
				s_rg2 += s_I2 + " + " + s_T2 + ",";
				s_rg3 += s_I3 + " + " + s_T3 + ",";
				s_rldI += s_ldI + ","; s_rldT += s_ldT + ",";

				nb_plots++;

				new_line = (i % 10 == 0);

				t_next++;

			} else {
				new_line = false;
			}

			// Goto end of line
			do {
				c = f_in_ode.get();
				if (c == '\r') c = f_in_ode.get();
			} while (!f_in_ode.eof() && c != '\n');

			if (!f_in_ode.eof()) //&& (i % modulus == 0))
			{
				// Get strings of interest
//				f_in >> s_t >> s_S >> s_I >> s_T >> s_g0 >> s_g1 >> s_g2 >> s_g3 >> s_ldI >> s_ldT;
				f_in_ode >> s_t >> s_S >> s_I0 >> s_I2 >> s_I1 >> s_I3 >> s_T0 >> s_T2 >> s_T1 >> s_T3 >> s_ldI >> s_ldT;
				// Avoid long lines for Copy/Paste in R-Console compliance
				if (new_line) {
					s_rt += "\n";
					s_rS += "\n"; s_rI += "\n"; s_rT += "\n";
					s_rg0 += "\n"; s_rg1 += "\n"; s_rg2 += "\n"; s_rg3 += "\n";
					s_rldI += "\n"; s_rldT += "\n";
				}

			}

			i++;

		}

//		if (s_rt[s_rS.length()-1] == '\n') {
//			////s_rt[s_rt.length()-2] = ')';
//			s_rS[s_rS.length()-3] = ')';
//			s_rI[s_rI.length()-3] = ')';
//			s_rT[s_rT.length()-3] = ')';
//		} else if (s_rt[s_rS.length()-1] == '\r') {
//			////s_rt[s_rt.length()-2] = ')';
//			s_rS[s_rS.length()-2] = ')';
//			s_rI[s_rI.length()-2] = ')';
//			s_rT[s_rT.length()-2] = ')';
//		} else {
//			/////s_rt[s_rt.length()-1] = ')';
//			s_rS[s_rS.length()-1] = ')';
//			s_rI[s_rI.length()-1] = ')';
//			s_rT[s_rT.length()-1] = ')';
//		}
		s_rt = s_rt.substr(0, s_rt.find_last_of(",")); s_rt += ")";
		s_rS = s_rS.substr(0, s_rS.find_last_of(",")); s_rS += ")";
		s_rI = s_rI.substr(0, s_rI.find_last_of(",")); s_rI += ")";
		s_rT = s_rT.substr(0, s_rT.find_last_of(",")); s_rT += ")";


//		s_rg0[s_rg0.length()-1] = ')';
//		s_rg1[s_rg1.length()-1] = ')';
//		s_rg2[s_rg2.length()-1] = ')';
//		s_rg3[s_rg3.length()-1] = ')';
		s_rg0 = s_rg0.substr(0, s_rg0.find_last_of(",")); s_rg0 += ")";
		s_rg1 = s_rg1.substr(0, s_rg1.find_last_of(",")); s_rg1 += ")";
		s_rg2 = s_rg2.substr(0, s_rg2.find_last_of(",")); s_rg2 += ")";
		s_rg3 = s_rg3.substr(0, s_rg3.find_last_of(",")); s_rg3 += ")";

//		s_rldI[s_rldI.length()-1] = ')';
//		s_rldT[s_rldT.length()-1] = ')';
		s_rldI = s_rldI.substr(0, s_rldI.find_last_of(",")); s_rldI += ")";
		s_rldT = s_rldT.substr(0, s_rldT.find_last_of(",")); s_rldT += ")";

		//cout << s_rt << endl << s_rS << endl << s_rI << endl << s_rT << endl;
		//cout << s_rg0 << endl << s_rg1 << endl << s_rg2 << endl << s_rg3 << endl;
		f_out << s_rt << "\n" << s_rS << "\n" << s_rI << "\n" << s_rT << "\n";
		f_out << s_rg0 << "\n" << s_rg1 << "\n" << s_rg2 << "\n" << s_rg3 << "\n";
		f_out << s_rldI << "\n" << s_rldT << "\n";

		cout << "Read '" << nb_plots << "' plots for ode !" << endl;



		// Plot
		s = "\n#-------------------------- DISPLAY ----------------------------#\n";

		// Saving the resulting plot image
		if (strcmp(out_ri_name, "") != 0)
		{
			s += "\n\
# Save render\n\
bmp(\n\
\t\"" + (string)out_ri_name + "\"\n\
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
xlim=c(min(vt),max(vt)), ylim=c(min(c(vS, vI, vT, vS_ode, vI_ode, vT_ode)), max(c(vS, vI, vT, vS_ode, vI_ode, vT_ode))))\n\
rect(par(\"usr\")[1], par(\"usr\")[3], par(\"usr\")[2], par(\"usr\")[4], col=\"lightgrey\")\n\
title(\"Hosts Versus Time\", font.main=4, col.main=\"darkgrey\")\n\
\n\
par(col=\"black\", font=1)\n\
lines(vt, vS, col=\"blue\", lwd=2)\n\
lines(vt, vI, col=\"red\", lwd=2)\n\
lines(vt, vT, col=\"green\", lwd=2)\n\
\n\
lines(vt_ode, vS_ode, col=\"blue\", lwd=2)\n\
lines(vt_ode, vI_ode, col=\"red\", lwd=2)\n\
lines(vt_ode, vT_ode, col=\"green\", lwd=2)\n\
\n\
\n\
par(col=\"white\", font=4)\n\
lnames <- c(\"S\", \"I\", \"T\")\n\
legend(\"topright\", lnames, col = c(\"blue\", \"red\", \"green\"), lty = 1)\n\
\n\
\n\
# Linkage Disequilibrium (LD)\n\
par(plt = c(0.1, 0.9, 0.2, 0.8), cex.axis = 1, cex=1, bg = \"grey\", font.lab=2, col.lab=\"darkgrey\", pch=19)\n\
plot(vt, vldI, type=\"l\", xlab=\"Discrete Time\", ylab=\"LD\", \n\t\
xlim=c(min(vt),max(vt)), ylim=c(min(c(vldI, vldT, vldI_ode, vldT_ode)), max(c(vldI, vldT, vldI_ode, vldT_ode))))\n\
rect(par(\"usr\")[1], par(\"usr\")[3], par(\"usr\")[2], par(\"usr\")[4], col=\"lightgrey\")\n\
title(\"LD Versus Time\", font.main=4, col.main=\"darkgrey\")\n\
\n\
par(col=\"black\", font=1)\n\
lines(vt, vldI, col=\"red\", lwd=2)\n\
lines(vt, vldT, col=\"green\", lwd=2)\n\
\n\
lines(vt_ode, vldI_ode, col=\"red\", lwd=2)\n\
lines(vt_ode, vldT_ode, col=\"green\", lwd=2)\n\
\n\
par(col=\"white\", font=4)\n\
lnames <- c(\"LDI\", \"LDT\")\n\
legend(\"topright\", lnames, col = c(\"red\", \"green\"), lty = 1)\n\
\n\
\n\
# Parasites\n\
par(plt = c(0.1, 0.9, 0.2, 0.8), cex.axis = 1, cex=1, bg = \"grey\", font.lab=2, col.lab=\"darkgrey\", pch=19)\n\
plot(vt, vg0, type=\"l\", xlab=\"Discrete Time\", ylab=\"Nb Individuals\", \n\t\
xlim=c(min(vt),max(vt)), ylim=c(min(c(vg0, vg1, vg2, vg3, vg0_ode, vg1_ode, vg2_ode, vg3_ode)), max(c(vg0, vg1, vg2, vg3, vg0_ode, vg1_ode, vg2_ode, vg3_ode))))\n\
rect(par(\"usr\")[1], par(\"usr\")[3], par(\"usr\")[2], par(\"usr\")[4], col=\"lightgrey\")\n\
title(\"Parasites Versus Time\", font.main=4, col.main=\"darkgrey\")\n\
\n\
par(col=\"black\", font=1)\n\
lines(vt, vg0, col=\"blue\", lwd=2)\n\
lines(vt, vg1, col=\"red\", lwd=2)\n\
lines(vt, vg2, col=\"green\", lwd=2)\n\
lines(vt, vg3, col=\"yellow\", lwd=2)\n\
\n\
lines(vt_ode, vg0_ode, col=\"blue\", lwd=2)\n\
lines(vt_ode, vg1_ode, col=\"red\", lwd=2)\n\
lines(vt_ode, vg2_ode, col=\"green\", lwd=2)\n\
lines(vt_ode, vg3_ode, col=\"yellow\", lwd=2)\n\
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

	f_in_stoch.close();
	f_in_ode.close();
	f_out.flush();
	f_out.close();
}





