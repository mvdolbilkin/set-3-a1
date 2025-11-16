import pandas as pd
import matplotlib.pyplot as plt
import math

# --- Константы ---
# Точное значение площади для построения эталонной линии
EXACT_AREA = math.pi / 4 + 1.25 * math.asin(0.8) - 1

# Имя файла с данными
DATA_FILE = 'results.csv'
# Имена файлов для сохранения графиков
PLOT_AREA_FILENAME = 'plot_area.png'
PLOT_ERROR_FILENAME = 'plot_error.png'

def create_plots():
    """
    Читает данные из CSV и строит два графика:
    1. Зависимость приближенной площади от N.
    2. Зависимость относительной ошибки от N.
    """
    # Загрузка данных из файла
    try:
        data = pd.read_csv(DATA_FILE)
    except FileNotFoundError:
        print(f"Ошибка: Файл '{DATA_FILE}' не найден. Сначала запустите C++ программу для генерации данных.")
        return

    # --- График 1: Приближенная площадь vs N ---
    plt.figure(figsize=(12, 7))
    plt.plot(data['N'], data['approx_area_wide'], label='Приближенная площадь (широкая область)', alpha=0.7)
    plt.plot(data['N'], data['approx_area_narrow'], label='Приближенная площадь (узкая область)', alpha=0.7)
    
    # Добавляем горизонтальную линию для точного значения
    plt.axhline(y=EXACT_AREA, color='r', linestyle='--', label=f'Точная площадь ({EXACT_AREA:.4f})')
    
    plt.title('График 1: Зависимость приближенной площади от числа точек (N)')
    plt.xlabel('Количество точек (N)')
    plt.ylabel('Приближенная площадь')
    plt.legend()
    plt.grid(True)
    plt.ylim(bottom=min(data['approx_area_narrow'].min(), data['approx_area_wide'].min()) * 0.95)
    
    # Сохранение графика в файл
    plt.savefig(PLOT_AREA_FILENAME)
    print(f"График площадей сохранен в файл {PLOT_AREA_FILENAME}")

    # --- График 2: Относительная ошибка vs N ---
    plt.figure(figsize=(12, 7))
    plt.plot(data['N'], data['relative_error_wide'], label='Отн. ошибка (широкая область)', alpha=0.8)
    plt.plot(data['N'], data['relative_error_narrow'], label='Отн. ошибка (узкая область)', alpha=0.8)
    
    plt.title('График 2: Зависимость относительной ошибки от числа точек (N)')
    plt.xlabel('Количество точек (N)')
    plt.ylabel('Относительная ошибка')
    plt.yscale('log')
    
    # --- ИЗМЕНЕНИЕ ---
    # Устанавливаем жесткие границы для оси Y, как вы и просили
    plt.ylim(0.0001, 1)
    
    plt.legend()
    plt.grid(True, which="both", ls="--")
    
    # Сохранение графика в файл
    plt.savefig(PLOT_ERROR_FILENAME)
    print(f"График ошибок сохранен в файл {PLOT_ERROR_FILENAME}")


if __name__ == '__main__':
    create_plots()
