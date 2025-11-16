#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <fstream>

struct Circle {
    double x, y, r;
};

bool is_inside_intersection(double px, double py, const std::vector<Circle>& circles) {
    for (const auto& c : circles) {
        if (std::pow(px - c.x, 2) + std::pow(py - c.y, 2) > std::pow(c.r, 2)) {
            return false;
        }
    }
    return true;
}

double monte_carlo_simulation(long long n_points, const std::vector<double>& box, const std::vector<Circle>& circles, std::mt19937& gen) {
    double x_min = box[0];
    double y_min = box[1];
    double x_max = box[2];
    double y_max = box[3];
    double box_area = (x_max - x_min) * (y_max - y_min);
    long long points_inside = 0;

    std::uniform_real_distribution<> dis_x(x_min, x_max);
    std::uniform_real_distribution<> dis_y(y_min, y_max);

    for (long long i = 0; i < n_points; ++i) {
        if (is_inside_intersection(dis_x(gen), dis_y(gen), circles)) {
            points_inside++;
        }
    }
    
    if (n_points == 0) return 0.0;

    return (static_cast<double>(points_inside) / n_points) * box_area;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::vector<Circle> circles = {
        {1.0, 1.0, 1.0},
        {1.5, 2.0, std::sqrt(5.0/4.0)},
        {2.0, 1.5, std::sqrt(5.0/4.0)}
    };

    const double exact_area = M_PI / 4.0 + 1.25 * std::asin(0.8) - 1.0;

    std::vector<double> wide_box = {0.0, 0.0, 3.2, 3.2};
    std::vector<double> narrow_box = {1.0, 1.0, 2.0, 2.0};

    std::ofstream results_file("results.csv");
    results_file << "N,approx_area_wide,relative_error_wide,approx_area_narrow,relative_error_narrow\n";

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int n = 100; n <= 100000; n += 500) {
        double approx_area_wide = monte_carlo_simulation(n, wide_box, circles, gen);
        double error_wide = std::abs(approx_area_wide - exact_area) / exact_area;

        double approx_area_narrow = monte_carlo_simulation(n, narrow_box, circles, gen);
        double error_narrow = std::abs(approx_area_narrow - exact_area) / exact_area;

        results_file << n << ","
                     << approx_area_wide << ","
                     << error_wide << ","
                     << approx_area_narrow << ","
                     << error_narrow << "\n";
    }

    results_file.close();
    std::cout << "Данные сохранены в файл results.csv" << std::endl;

    return 0;
}
