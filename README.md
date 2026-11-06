# REST API C++17 - индивидуальный вариант 4

Учебный REST-сервер на C++17 с SQLite, Bearer-авторизацией и RBAC. В проект интегрирован индивидуальный расчётный модуль **Dynamic Pricing Core**: оптимизатор динамического ценообразования.

## Что реализовано

- REST API на `cpp-httplib`;
- SQLite-хранилище пользователей, сессий, расчётов и логов;
- роли `user`, `admin`, `owner`;
- хеширование паролей и Bearer-токены;
- CORS, единый JSON-формат ошибок и healthcheck;
- индивидуальный вариант 4 с полной валидацией входных данных;
- сохранение успешных расчётов в таблицу `works`;
- отдельные тесты расчётного модуля.

## Вариант 4: Dynamic Pricing Core

Модуль рассчитывает оптимальную цену по формулам:

```text
scarcity_multiplier = current_stock < 10 ? 1.25 : 1.0
optimized_price = base_price * demand_surge_factor * scarcity_multiplier
```

Затем цена корректируется нижними границами:

```text
competitor_floor = competitor_price * 0.95
margin_floor = base_price * (1.0 + min_allowable_margin / 100.0)
```

Подробное описание алгоритма, ограничений, API и демонстрации находится в [docs/VARIANT_4.md](docs/VARIANT_4.md).

## Быстрый запуск

```bash
sudo apt update
sudo apt install -y build-essential cmake sqlite3 libsqlite3-dev
cmake -S . -B build
cmake --build build -j2
./build/rest_server
```

Сервер: `http://localhost:8080`.

Проверка:

```bash
curl http://localhost:8080/health
```

## API индивидуального задания

```text
POST /api/v1.1/works/calc-individual
GET  /api/v1.1/works/history-individual?limit=10
```

Для расчёта нужен Bearer-токен. Контрольный запрос и ожидаемый результат приведены в [docs/VARIANT_4.md](docs/VARIANT_4.md).

## Тестирование модуля

```bash
g++ -std=c++17 -Wall -Wextra -pedantic tests/test_variant4.cpp -I. -o /tmp/test_variant4
/tmp/test_variant4
```

## Структура индивидуального решения

- `src/modules/individual/variant_mine.hpp` - реализация варианта 4;
- `src/modules/individual/task.hpp` - подключение модуля и REST-маршруты;
- `tests/test_variant4.cpp` - контрольные и ошибочные сценарии;
- `frontend/index.html` - браузерная коллекция запросов;
- `data/tasks.txt` - полное ТЗ с вариантами.

Автор: **Гукасян Ваграм Вачаганович**.
