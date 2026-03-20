
#include <iostream>
#include <iomanip>
#include <new>
using namespace std;

struct _capacitor {
    double *time;
    double *voltage;
    double *current;
    double C;
};

typedef struct _capacitor Capacitor;

int main() 
{
    // Given values
    double dt = 1e-10;
    double t_final = 5e-6;
    int nsteps = (int)(t_final / dt);   // should be 50000
    int N = nsteps + 1;

    double R = 1000.0;
    double C = 100e-12;
    double Iconst = 1e-2;
    double V0 = 10.0;

    // capacitor for constant current
    Capacitor cap_cc;
    cap_cc.C = C;
    cap_cc.time = new double[N];
    cap_cc.voltage = new double[N];
    cap_cc.current = new double[N];

    // capacitor for constant voltage
    Capacitor cap_cv;
    cap_cv.C = C;
    cap_cv.time = new double[N];
    cap_cv.voltage = new double[N];
    cap_cv.current = new double[N];

    // fill the time arrays
    for (int i = 0; i < N; i++) {
        cap_cc.time[i] = i * dt;
        cap_cv.time[i] = i * dt;
    }

    // Constant current simulation
    cap_cc.voltage[0] = 0.0;
    cap_cc.current[0] = Iconst;
    for (int i = 0; i < nsteps; i++) {
        cap_cc.current[i] = Iconst; 
        cap_cc.voltage[i+1] = cap_cc.voltage[i] + (cap_cc.current[i] * dt) / cap_cc.C;
        cap_cc.current[i+1] = Iconst;
    }

    // Constant voltage simulation
    cap_cv.voltage[0] = 0.0;
    cap_cv.current[0] = V0 / R;

    double RC = R * C;

    for (int i = 0; i < nsteps; i++) {
        cap_cv.current[i+1] = cap_cv.current[i] - (cap_cv.current[i] / RC) * dt;
        cap_cv.voltage[i+1] = cap_cv.voltage[i] + (cap_cv.current[i] * dt) / cap_cv.C;
    }

    // Print every 200 steps
    cout << scientific << setprecision(4);
    cout << "step\t time\t\t CC_V\t CC_I\t\t CV_V\t CV_I\n";

    for (int i = 0; i <= nsteps; i += 200) {
        cout << i << "\t"
             << cap_cc.time[i] << "\t"
             << cap_cc.voltage[i] << "\t"
             << cap_cc.current[i] << "\t"
             << cap_cv.voltage[i] << "\t"
             << cap_cv.current[i] 
             << endl;
    }

    // end result
    cout << "\nFinal constant current values:\n";
    cout << "Vc = " << cap_cc.voltage[nsteps] 
         << " , I = " << cap_cc.current[nsteps] << endl;

    cout << "\nFinal constant voltage values:\n";
    cout << "Vc = " << cap_cv.voltage[nsteps] 
         << " , I = " << cap_cv.current[nsteps] 
         << " (V0 = " << V0 << ")" << endl;

    // free memory
    delete[] cap_cc.time;
    delete[] cap_cc.voltage;
    delete[] cap_cc.current;

    delete[] cap_cv.time;
    delete[] cap_cv.voltage;
    delete[] cap_cv.current;

    return 0;
}

