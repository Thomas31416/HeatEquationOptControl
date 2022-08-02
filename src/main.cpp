#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include "cg.hpp"
#include "matrix/heat.hpp"
#include "matrix/id.hpp"
#include "matrix/int.hpp"
#include "mesh.hpp"

#define MESH "mesh.msh"
#define BORDER {1, 2, 3, 4}
#define CONTROL {2, 4, 6, 8, 14}

#define TOL_CTRL 1.0e-4
#define TOL_HEAT 1.0e-4

#define ALPHA 1.0e-5

using namespace std;

int main(int argc, char *argv[]) {
    try {
        ifstream mesh_file(MESH);

        if (mesh_file.fail())
            throw runtime_error(strerror(errno));

        Mesh mesh(mesh_file, BORDER, CONTROL);
        mesh_file.close();

        size_t n = ceil(sqrt(mesh.triangles().size()));

        VectorMut u((n + 1) * mesh.control_vertices().size());
        VectorMut y((n + 1) * mesh.vertices().size());
        VectorMut b((n + 1) * mesh.control_vertices().size());

        IntTimeCtrl intTimeCtrl(n, mesh);
        Heat heat(n, mesh, TOL_HEAT);
        HeatAdj heatAdj(n, mesh, TOL_HEAT);
        Id id((n + 1) * mesh.control_vertices().size());

        CG cg((n + 1) * mesh.control_vertices().size());
        size_t n_it;

        cout << "*** Computing right hand side for optimal control optimality condition…" << endl;
        y = 1.0;
        b += intTimeCtrl * heatAdj * y;

        cout << "*** Starting conjugate gradient for optimal control…" << endl;
        n_it = cg.solve(intTimeCtrl * (heatAdj * heat + ALPHA * id), u, b, TOL_CTRL);
        cout << "*** Conjugate gradient converged in " << n_it << " iterations." << endl;

        cout << "*** Writing `control-c++.dat'…" << endl;
        ofstream out_ctrl("control-c++.dat");

        if (out_ctrl.fail())
            throw runtime_error(strerror(errno));

        for (size_t k = 0; k < n + 1; ++k) {
            const Vector &u_slice = u.slice(mesh.control_vertices().size(), k);

            for (auto t : mesh.control_triangles()) {
                const Triangle &triangle = mesh.triangles()[t];

                for (auto i : {0, 1, 2, 0}) {
                    size_t vertex = triangle.vertices()[i];
                    out_ctrl << k << " "
                             << mesh.vertices()[vertex] << " "
                             << u_slice[mesh.vertices()[vertex].control_id()] << endl;
                }

                out_ctrl << endl << endl;
            }
        }

        out_ctrl.close();

        cout << "*** Solving heat equation for optimal control…" << endl;
        y = heat * u;

        cout << "*** Writing `heat-c++.dat'…" << endl;
        ofstream out_heat("heat-c++.dat");

        if (out_heat.fail())
            throw runtime_error(strerror(errno));

        for (size_t k = 0; k < n + 1; ++k) {
            const Vector &y_slice = y.slice(mesh.vertices().size(), k);

            for (auto triangle : mesh.triangles()) {
                for (auto i : {0, 1, 2, 0}) {
                    size_t vertex = triangle.vertices()[i];
                    out_heat << k << " "
                             << mesh.vertices()[vertex] << " "
                             << y_slice[vertex] << endl;
                }

                out_heat << endl << endl;
            }
        }

        out_heat.close();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
