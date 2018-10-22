# Лабораторная работа №6

## Задание

Использовать `std::thread` или `std::async`/`std::future` для параллельного вычисления значений хэш-функции [SHA-256](https://ru.wikipedia.org/wiki/SHA-2) определённого вида.

Для отображения значения хэш-функций часто используют запись в шестнадцатиричном (**HEX**) формате. Результатом работы функции **SHA-256** является массив из 32 байт (`32 * 8 = 256`), который может быть представлен в виде 64-x символьной **HEX**-строки:
```
SHA256("test") = "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08";
SHA256("1234") = "03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4";
```

Особенностью современных криптографических хэш-функций является вычислительная сложность получения:
- коллизий (разных сообщений, для которых значение хэш-функции совпадает);
- прообразов (исходных сообщений из имеющегося значения хэш-функции).

На текущий момент самым эффективным решением данных задач является метод полного перебора.

Требуется:
- организовать перебор случайных входных данных (прообразов) для функции **SHA-256** с целью получения значения хэш-функции, удовлетворяющего следующему виду:
`XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX0000`, где `X`- произвольный **HEX**-символ, а `0` - ноль, т.е. значение хэш-функции должно заканчиваться на `N=4`нулей, а остальные символы могут быть произвольными;
- вычисления проводить в `M` потоках, где `M`- максильное число одновременно выполняемых потоков, поддерживаемых используемым оборудованием;
- при обнаружении значения хэш-функции требуемого вида вывести его в `Log` (консоль+файл) и продолжить вычисления;
- для логгирования использовать библиотеку `boost::log` (документацию и примеры использования можно найти [тут](https://www.boost.org/doc/libs/1_63_0/libs/log/doc/html/log/tutorial.html#log.tutorial.trivial));
- для формирования случайных входных данных использовать псевдослучайный генератор `std::rand`;


## Рекомендации

Подключение библиотеки `boost::log` осуществить с помощью пакетного менеджера **Hunter** (см. [пример](https://docs.hunter.sh/en/latest/packages/pkg/Boost.html)).

Для вычислений значений хэш-функции использовать библиотеку [picosha2](https://github.com/okdshin/PicoSHA2).

Количество одновременно выполняемых потоков для конкретной аппаратной конфигурации может быть получено с помощью функции `std::thread::hardware_concurrency`.

Пример получения значения хэш-функции в **HEX**-виде из последовательности байт:
```cpp
#include <picosha2.h>

std::vector<unsigned char> data_vector{...}; // вектор
std::string hash = picosha2::hash256_hex_string(data_vector);

std::array<unsigned char, 4> data_array{...}; // массив
std::string hash = picosha2::hash256_hex_string(data_array);

unsigned char data_c_array[N] = {...}; // c-массив
std::string hash = picosha2::hash256_hex_string(data_c_array, data_c_array + N);
```

Пример тривиального логирования с помощью библиотеки `boost::log`:
```cpp
#include <boost/log/trivial.hpp>

int main(int, char*[])
{
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
}
```

Пример настройки логгирования в файл:
```cpp
logging::add_file_log("log.log"); // тривиальный вывод в файл

logging::add_file_log // расширенная настройка
(
    keywords::file_name = "log_%N.log",
    keywords::rotation_size = 10 * 1024 * 1024, 
    keywords::time_based_rotation = sinks::file::rotation_at_time_point{0, 0, 0},
    keywords::format = "[%TimeStamp%]: %Message%"
);
```

Пример установки фильтрации логгирования:
```cpp
logging::core::get()->set_filter
(
    logging::trivial::severity >= logging::trivial::info
);
```
