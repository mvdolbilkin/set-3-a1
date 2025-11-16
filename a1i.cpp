#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <algorithm>

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

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::vector<Circle> circles(3);
    for (int i = 0; i < 3; ++i) {
        std::cin >> circles[i].x >> circles[i].y >> circles[i].r;
    }
    double min_x = circles[0].x - circles[0].r;
    double max_x = circles[0].x + circles[0].r;
    double min_y = circles[0].y - circles[0].r;
    double max_y = circles[0].y + circles[0].r;

    for (int i = 1; i < 3; ++i) {
        min_x = std::min(min_x, circles[i].x - circles[i].r);
        max_x = std::max(max_x, circles[i].x + circles[i].r);
        min_y = std::min(min_y, circles[i].y - circles[i].r);
        max_y = std::max(max_y, circles[i].y + circles[i].r);
    }
    const long long n_points = 2000000;
    double box_area = (max_x - min_x) * (max_y - min_y);
    long long points_inside = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_x(min_x, max_x);
    std::uniform_real_distribution<> dis_y(min_y, max_y);

    for (long long i = 0; i < n_points; ++i) {
        double rand_x = dis_x(gen);
        double rand_y = dis_y(gen);
        if (is_inside_intersection(rand_x, rand_y, circles)) {
            points_inside++;
        }
    }

    double estimated_area = (static_cast<double>(points_inside) / n_points) * box_area;
    std::cout << std::fixed << std::setprecision(20) << estimated_area << std::endl;
    return 0;
}
