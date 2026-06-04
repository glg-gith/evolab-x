#ifndef DIFFEQ_SIM_H
#define DIFFEQ_SIM_H

#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_errno.h>
#include <iostream>
#include <vector>


using namespace std;


class DiffEq_Sim {

	private:
		int _cnt;
		double _wstep;	// write step
		double _t_w;	// write step

    public:
        double _t;      // initial time
        double _h;      // time step
        double _tmax;   // max time
        double _hmin;

//        gsl_odeiv_control* c;
//        gsl_odeiv_step*    s;

    public:
        DiffEq_Sim(double t, double tmax, double h, double hmin, double wstep) {
            cout << "Init !" << endl;

            _t    = t;      //initial time
            _tmax = tmax;
            _h    = h;      //time step
            _hmin = hmin;

            _t_w = 0.0;
            _wstep = wstep;
        };

        virtual ~DiffEq_Sim() {};

        int _nbins;
        double* _y;

//        void printY() { for(int i=0; i < nbins; i++) { cout << y[i] << " ";} cout << endl; }
        virtual void printToFile() {}
        
        vector<double> get_state() {
            vector<double> C;
            C.assign(_y, _y + _nbins);
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
            gsl_odeiv_evolve*  e = gsl_odeiv_evolve_alloc(_nbins);
            gsl_odeiv_control* c = gsl_odeiv_control_y_new(4e-17, 0);     // "derivate()" called 100000x
//            gsl_odeiv_control* c = gsl_odeiv_control_y_new(4e-12, 0);   // solver ("evolve_apply") called 1200x
            gsl_odeiv_step*    s = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, _nbins);
//            gsl_odeiv_step*    s = gsl_odeiv_step_alloc(gsl_odeiv_step_rk4, nbins);
            gsl_odeiv_system sys = {function, NULL, _nbins, this };

            _cnt = 0;
            while (_t < _tmax) {  //convergence check here
                int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &_t, _tmax, &_h, _y);

                if (_t >= _t_w) {
                	printToFile();
                	_t_w += _wstep;
                }

                _cnt++;

                if (status != GSL_SUCCESS) { return status; }
            }
        	cout << "End sim ! > Solver called '" << _cnt << "' times." << endl;
            return 0;
        }

       int step_simulation( double stepsize ) {
            gsl_odeiv_evolve*  e = gsl_odeiv_evolve_alloc(_nbins);
            gsl_odeiv_control* c = gsl_odeiv_control_y_new(1e-5, 0);
            gsl_odeiv_step*    s = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, _nbins);
            gsl_odeiv_system sys = {function, NULL, _nbins, this };

            double tstop = _t + stepsize;
            while (_t < tstop) {
                int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &_t, tstop, &_h, _y);
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


#endif
