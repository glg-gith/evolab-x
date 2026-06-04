#ifndef DIFFEQ_SIM_H
#define DIFFEQ_SIM_H

#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_errno.h>
#include <iostream>
#include <vector>


using namespace std;


class DiffEq_Sim {

	private:
		double t_w;
		int cnt;

    public:
        double _t;      //initial time
        double _h;      //time step
        double _tmax;   //max time
        double _hmin;

//        gsl_odeiv_control* c;
//        gsl_odeiv_step*    s;

    public:
        DiffEq_Sim(double t, double tmax, double h, double hmin) {
            cout << "Init !" << endl;

            t_w = 1.0;

            _t    = t;      //initial time
            _tmax = tmax;
            _h    = h;      //time step
            _hmin = hmin;
        };

        virtual ~DiffEq_Sim() {};

        int nbins;
        double* y;

//        void printY() { for(int i=0; i < nbins; i++) { cout << y[i] << " ";} cout << endl; }
        virtual void printToFile() {}
        
        vector<double> get_state() {
            vector<double> C;
            C.assign(y, y + nbins);
            return C;
        }

        double get_time() { return _t; }


        virtual void initialize() {}
        virtual void derivative(const double y[], double dydt[]){}

        static int function(double t, double const y[], double dydt[], void *params) {
            DiffEq_Sim* model = static_cast <DiffEq_Sim*> (params);
            model->derivative(y, dydt);
            return GSL_SUCCESS;
        }


        int run_simulation() {
        	cout << "Start sim !" << endl;
            gsl_odeiv_evolve*  e = gsl_odeiv_evolve_alloc(nbins);
            gsl_odeiv_control* c = gsl_odeiv_control_y_new(4e-17, 0);     // "derivate()" called 100000x
//            gsl_odeiv_control* c = gsl_odeiv_control_y_new(4e-12, 0);   // solver ("evolve_apply") called 1200x
            gsl_odeiv_step*    s = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, nbins);
//            gsl_odeiv_step*    s = gsl_odeiv_step_alloc(gsl_odeiv_step_rk4, nbins);
            gsl_odeiv_system sys = {function, NULL, nbins, this };

            cnt = 0;
            while (_t < _tmax) {  //convergence check here
                int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &_t, _tmax, &_h, y);

//                if (_t > t_w) {
//                	//printY();
//                	t_w += 0.1;
//                }
//
                printToFile();

                cnt++;


                if (status != GSL_SUCCESS) { return status; }
            }
        	cout << "End sim ! > Solver called '" << cnt << "' times." << endl;
            return 0;
        }

       int step_simulation( double stepsize ) {
            gsl_odeiv_evolve*  e = gsl_odeiv_evolve_alloc(nbins);
            gsl_odeiv_control* c = gsl_odeiv_control_y_new(1e-5, 0);
            gsl_odeiv_step*    s = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, nbins);
            gsl_odeiv_system sys = {function, NULL, nbins, this };

            double tstop = _t + stepsize;
            while (_t < tstop) {
                int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &_t, tstop, &_h, y);
                if (status != GSL_SUCCESS) { return status; }
            }
            return 0;
        }

        /*
        double* advance_simulation(double I_lim) {
            while (t < 2000) {  //convergence check here
                int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, t1, &h, y);
                if (y[1] < I_lim) { return y; }
            }
            printY();
            return y;
        }
*/
};




//class DiffEq_Sim {
//    private:
//        double t;      //initial time
//        double h;      //time step
//        double tmax;   //max time
//        double hmin;
//
//    public:
//        DiffEq_Sim() {
//            cout << "Init !" << endl;
//            t    = 0.0;      //initial time
//            h    = 0.1;     //time step
//            tmax = 2000;
//            hmin = 0.2;
//
////            nbins = 1000;
////            y = (double*)malloc(nbins * sizeof(double));
//
//        };
//
//        virtual ~DiffEq_Sim() {};
//
//        int nbins;
//        double* y;
//
//        void printY() { for(int i=0; i < nbins; i++) { cout << y[i] << " ";} cout << endl; }
//
//        vector<double> get_state() {
//            vector<double> C;
//            C.assign(y, y + nbins);
//            return C;
//        }
//
//        double get_time() { return t; }
//
//
//        virtual void initialize() {}
//        virtual void derivative(const double y[], double dydt[]){}
//
//        static int function(double t, double const y[], double dydt[], void *params) {
//            DiffEq_Sim* model = static_cast <DiffEq_Sim*> (params);
//            model->derivative(y, dydt);
//            return GSL_SUCCESS;
//        }
//
//
//        int run_simulation() {
//        	cout << "Start sim !" << endl;
//            gsl_odeiv_evolve*  e = gsl_odeiv_evolve_alloc(nbins);
//            gsl_odeiv_control* c = gsl_odeiv_control_y_new(1e-20, 0);
//            gsl_odeiv_step*    s = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, nbins);
//            gsl_odeiv_system sys = {function, NULL, nbins, this };
//            while (t < tmax) {  //convergence check here
//                int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, tmax, &h, y);
//                printY();
//                if (status != GSL_SUCCESS) { return status; }
//            }
//        	cout << "End sim !" << endl;
//            return 0;
//        }
//
//       int step_simulation( double stepsize ) {
//            gsl_odeiv_evolve*  e = gsl_odeiv_evolve_alloc(nbins);
//            gsl_odeiv_control* c = gsl_odeiv_control_y_new(1e-5, 0);
//            gsl_odeiv_step*    s = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, nbins);
//            gsl_odeiv_system sys = {function, NULL, nbins, this };
//
//            double tstop = t+stepsize;
//            while (t < tstop) {
//                int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, tstop, &h, y);
//                if (status != GSL_SUCCESS) { return status; }
//            }
//            return 0;
//        }
//
//        /*
//        double* advance_simulation(double I_lim) {
//            while (t < 2000) {  //convergence check here
//                int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, t1, &h, y);
//                if (y[1] < I_lim) { return y; }
//            }
//            printY();
//            return y;
//        }
//*/
//};

#endif
