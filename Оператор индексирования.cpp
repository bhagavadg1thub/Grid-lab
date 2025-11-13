#include <cassert>

template <typename T> // T - любой тип файлов

class Grid final {
public:
    using value_type = T; 
    using size_type = unsigned; // unsigned - как инт, но только неотрицательные
    
private:
    T* data;           // Указатель на массив
    size_type y_size;  // Количество строк
    size_type x_size;  // Количество столбцов

     // Вспомогательный класс для оператора [] - однострочная матрица
    class Proxy {
    private:
        T* row; // ссылка на начало строки
        size_type size; // длина строки
    public:
        Proxy(T* row, size_type size) : row(row), size(size) {}
        
        T& operator[](size_type x_idx) {
            return row[x_idx];
        }
        
        T operator[](size_type x_idx) const {
            return row[x_idx];
        }
    };

public:
    // 1. Конструктор с одним параметром 
    Grid(T const &t) : data(new T[1]), y_size(1), x_size(1) {
        data[0] = t;
    }

// Пример: Grid<int> grid1(42); - создает сетку 1x1 с числом 42
    
    // 2. Конструктор с двумя размерными параметрами 
    Grid(size_type y_size, size_type x_size) 
    : data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {
        // Внутри эелементов хранится мусор
    }

// Пример: Grid<float> grid2(2, 3); - создает сетку 2x3 с абы какими элементами 
    
    // 3. Конструктор с тремя параметрами - размеры и значениЕ (одно, во всех одинаковое)
    Grid(size_type y_size, size_type x_size, T const &t) 
    : data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
    }

// Пример: Grid<string> grid3(2, 2, "hello"); - создает сетку 2x2, где каждая клетка заполнена "hello"
    
    // РАЗ - Деструкт ор
    ~Grid() {
        delete[] data;
    }
    
    // ДВА - Конструктор копирования (создает новую сетку и копирует существующую)
    Grid(Grid const &other) // Принял константную ссылку на другой объект Grid
    : data(new T[other.y_size * other.x_size]), // тут же выделяем память под новую data
      y_size(other.y_size), x_size(other.x_size) { // скопировали размеры из other
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = other.data[i]; // копируем
        }
    }

// Пример: Grid<int> copy = original; - original и copy - два РАЗНЫХ объекта с одинаковыми данными
    
    // ТРИ - Оператор присваивания копированием (сносит существующую сетку и перезаписывает)
    Grid& operator=(Grid const &other) { // принимает ссылку на присваиваемый Grid, отдает ссылку на текущий Grid
        if (this != &other) { // Если два Grid'a не равны
            delete[] data; // сносим все
            
            y_size = other.y_size; 
            x_size = other.x_size;
            data = new T[y_size * x_size]; 
            for (size_type i = 0; i < y_size * x_size; ++i) {
                data[i] = other.data[i]; // Снова копируем размеры, выделяем новую память, поштучно копируем данные 
            }
        }
        return *this; // Возвращаем ссылку на рассматриваемый грид
    }

// Пример: gridB = gridA; - gridB становится копией gridA
    
    // ЧЕТЫРЕ - Конструктор перемещения
    Grid(Grid &&other) // Принял ссылку на временный объект other 
    : data(other.data), y_size(other.y_size), x_size(other.x_size) { // Выделили память и скопировали размер и ссылку на данные
        other.data = nullptr; // обнулили ссылку на данные в старом гриде
        other.y_size = 0; // размер обнулили
        other.x_size = 0; // размер обнулили
    }

// Пример: Grid<int> gridMoved = VremennyGrid();  - Вместо копирования элементов просто "забрали" память временного объекта
    
    // ПЯТЬ - Оператор присваивания перемещением - правило пяти закрыли
    Grid& operator=(Grid &&other) {
        if (this != &other) {
            delete[] data;
            
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }

// Пример: existingGrid = Grid<int>(3, 3, 99); - Старая память existingGrid освобождена, забрали память временного объекта
    
    // Методы доступа к размерам
    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
    
    // Операторы доступа к элементам через круглые скобки
    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }
    
    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }
    
    // Оператор присваивания для заполнения всех элементов
    Grid<T>& operator=(T const &t) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
        return *this;
    }

    // Оператор индексирования [] для двойного доступа
    Proxy operator[](size_type y_idx) { // принимает в квадратных скобках значение y_idx
        return Proxy(&data[y_idx * x_size], x_size); // по формуле считает номер элемента, отвечающий за начало этой строки. Возвращает ссылку на него
    }
    
    const Proxy operator[](size_type y_idx) const { // то же самое, только возвращает значение
        return Proxy(&data[y_idx * x_size], x_size);
    }
};
// Тестовый код из задания
int main() {
    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());
    
    using gsize_t = Grid<float>::size_type;
    
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);
    
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;
    
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(1.0f == g(y_idx, x_idx));
    
    return 0;
}
