#ifndef BEZIER_H_
#define BEZIER_H_
#include "../Types.h"
#include <cmath>
#include <vector>

template <template <typename> class P, typename T, std::size_t NumControlPoints>
class Bezier {
public:
    Bezier(const std::vector<P<T>> &control_points):control_points(control_points) {
        static_assert(NumControlPoints == 2 || NumControlPoints == 3, "Invalid number of control points");
    }

    ~Bezier() {}

    Bezier(const Bezier &) = delete;
    Bezier& operator = (const Bezier &) = delete;

    Bezier(Bezier &&) = delete;
    Bezier &operator = (Bezier &&) = delete;

    std::vector<P<T>> Fit(float it  = 0.01) {
        std::vector<P<T>> result;
        std::size_t size = control_points.size();

        for(std::size_t p = 0; p < size; p+= NumControlPoints - 1){
            auto p0 = control_points[p];
            auto p1 = control_points [(p + 1) % size];
            auto p2 = control_points [(p + 2) % size];
            auto p3 =  control_points[(p + 3) % size];

            for(float t = 0; t <= 1; t += it){
                P<T> point = CalculateBezier(p0, p1, p2, p3, t);
                result.push_back(point);
            }
        }
    }

 T CalculateBezier(T p0, T p1, T p2, T p3, double t) {
        if (NumControlPoints == 2) {
            // Quadratic Bezier
            double u = 1 - t;
            double tt = t * t;
            double uu = u * u;

            return uu * p0 + 2 * u * t * p1 + tt * p2;
        } else if (NumControlPoints == 3) {
            // Cubic Bezier
            double u = 1 - t;
            double tt = t * t;
            double uu = u * u;
            double uuu = uu * u;
            double ttt = tt * t;

            return uuu * p0 + 3 * uu * t * p1 + 3 * u * tt * p2 + ttt * p3;
        }

        throw std::runtime_error("Unsuppored number of control points !");

}


private:
    std::vector<P<T>> control_points;
};

#endif
