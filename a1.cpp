#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <fstream> // Для работы с файлами

// --- Параметры задачи из условия ---
// Эти параметры соответствуют фигуре из основного задания
struct Circle {
    double x, y, r;
};

// Функция для проверки, находится ли точка внутри пересечения всех трех окружностей
bool is_inside_intersection(double px, double py, const std::vector<Circle>& circles) {
    for (const auto& c : circles) {
        if (std::pow(px - c.x, 2) + std::pow(py - c.y, 2) > std::pow(c.r, 2)) {
            return false;
        }
    }
    return true;
}

// --- Алгоритм Монте-Карло ---
double monte_carlo_simulation(long long n_points, const std::vector<double>& box, const std::vector<Circle>& circles, std::mt19937& gen) {
    double x_min = box[0];
    double y_min = box[1];
    double x_max = box[2];
    double y_max = box[3];
    double box_area = (x_max - x_min) * (y_max - y_min);
    long long points_inside = 0;

    // Генераторы случайных чисел для X и Y
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
    // Ускорение стандартного ввода-вывода
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // --- Определение параметров задачи ---
    // Окружности из основного условия для проведения эксперимента
    std::vector<Circle> circles = {
        {1.0, 1.0, 1.0},
        {1.5, 2.0, std::sqrt(5.0/4.0)},
        {2.0, 1.5, std::sqrt(5.0/4.0)}
    };

    // Точное значение площади, вычисленное аналитически
    const double exact_area = M_PI / 4.0 + 1.25 * std::asin(0.8) - 1.0;

    // Прямоугольные области (широкая и узкая)
    std::vector<double> wide_box = {0.0, 0.0, 3.2, 3.2};
    std::vector<double> narrow_box = {1.0, 1.0, 2.0, 2.0};

    // --- Проведение эксперимента и запись в файл ---
    std::ofstream results_file("results.csv");
    // Записываем заголовок в CSV файл
    results_file << "N,approx_area_wide,relative_error_wide,approx_area_narrow,relative_error_narrow\n";

    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());

    // Цикл для N от 100 до 100,000 с шагом 500
    for (int n = 100; n <= 100000; n += 500) {
        // Симуляция для широкой области
        double approx_area_wide = monte_carlo_simulation(n, wide_box, circles, gen);
        double error_wide = std::abs(approx_area_wide - exact_area) / exact_area;

        // Симуляция для узкой области
        double approx_area_narrow = monte_carlo_simulation(n, narrow_box, circles, gen);
        double error_narrow = std::abs(approx_area_narrow - exact_area) / exact_area;

        // Запись результатов в файл
        results_file << n << ","
                     << approx_area_wide << ","
                     << error_wide << ","
                     << approx_area_narrow << ","
                     << error_narrow << "\n";
    }

    results_file.close();
    std::cout << "Эксперимент завершен. Данные сохранены в файл results.csv" << std::endl;

    return 0;
}
